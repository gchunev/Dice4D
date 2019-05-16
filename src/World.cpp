/*
* Copyright (c) 2006-2009 Erin Catto http://www.gphysics.com
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies.
* Erin Catto makes no representations about the suitability 
* of this software for any purpose.  
* It is provided "as is" without express or implied warranty.
*/

#include "World.h"
#include "Body.h"
#include "HalfSpace.h"
#include "Memories.h"

#include "AudioUtils.h"

#include <glm/gtc/matrix_transform.hpp>

#ifdef _DEBUG_DUMP_TELEMETRY
#include <fstream>
#endif

using std::vector;
using std::map;
using std::pair;

typedef map<ArbiterKey, Arbiter>::iterator ArbIter;
typedef pair<ArbiterKey, Arbiter> ArbPair;

bool World::warmStarting = true;
bool World::reinitDieParams = true;
bool World::isReplaying = false;

#ifdef _DEBUG_DUMP_TELEMETRY
unsigned int World::n = 0;
#endif

World::World(const glm::vec4& gravity, int iterations)
	: gravity(gravity)
	, iterations(iterations)
	, simulationEnabled(true)
{}

void World::SetArbiterParameters(float contactDetectionEpsilon, float velocityNormThreshold, float angularVelocityNormThreshold, float restitutionCoefficient, int minNumOfContactsForStability)
{
	arbiterParameters.contactDetectionEpsilon = contactDetectionEpsilon;
	arbiterParameters.velocityNormThreshold = velocityNormThreshold;
	arbiterParameters.angularVelocityNormThreshold = angularVelocityNormThreshold;
	arbiterParameters.restitutionCoefficient = restitutionCoefficient;
	arbiterParameters.minNumOfContactsForStability = minNumOfContactsForStability;
}

void World::Add(Body* body)
{
	bodies.push_back(body);

	GetMemories().Add(body);
}

void World::Add(HalfSpace* plane)
{
	planes.push_back(plane);

	GetMemories().Add(plane);
}

void World::Clear()
{
	bodies.clear();
	planes.clear();
	arbiters.clear();

	isReplaying = false;
	simulationEnabled = true;
#ifdef _DEBUG_DUMP_TELEMETRY
	n = 0;
#endif

	GetMemories().Wipe();
}

void World::BroadPhase()
{
	for (int i = 0; i < (int)bodies.size(); ++i)
	{
		bodies[i]->ResetContactCount();
	}

	// O(n^2) broad-phase
	for (int i = 0; i < (int)bodies.size(); ++i)
	{
		Body* bi = bodies[i];

		for (int j = i + 1; j < (int)bodies.size(); ++j)
		{
			Body* bj = bodies[j];

			if (bi->invMass == 0.0f && bj->invMass == 0.0f)
				continue;

			Arbiter newArb(arbiterParameters, bi, bj);
			ArbiterKey key(bi, bj);

			if (newArb.numContacts > 0)
			{
				ArbIter iter = arbiters.find(key);
				if (iter == arbiters.end())
				{
					arbiters.insert(ArbPair(key, newArb));
				}
				else
				{
					iter->second.Update(newArb.contacts, newArb.numContacts);
				}
			}
			else
			{
				arbiters.erase(key);
			}
		}

		for (int j = 0; j < (int)planes.size(); ++j)
		{
			HalfSpace* pj = planes[j];

			Arbiter newArb(arbiterParameters, pj, bi);
			ArbiterKey key(pj, bi);

			if (newArb.numContacts > 0)
			{
				ArbIter iter = arbiters.find(key);
				if (iter == arbiters.end())
				{
					arbiters.insert(ArbPair(key, newArb));
				}
				else
				{
					iter->second.Update(newArb.contacts, newArb.numContacts);
				}
			}
			else
			{
				arbiters.erase(key);
			}
		}
	}
}

void World::Simulate(float dt)
{
	if (isReplaying || !simulationEnabled || bodies.empty())
	{
		return;
	}

	GetMemories().Impress();

	float inv_dt = dt > 0.0f ? 1.0f / dt : 0.0f;

	// Determine overlapping bodies and update contact points.
	BroadPhase();

	// Check for the need to play a hit sound:
	static int prevNumContacts = 0;
	static Contact prevContacts[Arbiter::MAX_POINTS];
	if (!arbiters.empty())
	{
		for (auto& arbiterIter : arbiters)
		{
			auto& arbiter = arbiterIter.second;
			auto numContacts = arbiter.numContacts;
			auto& contacts = arbiter.contacts;

			bool hasNewHit = false;
			float maxContactHitSpeed = 0.0f;
			for (int i = 0; i < numContacts; i++)
			{
				auto& contact = contacts[i];
				if (i >= prevNumContacts || contact.feature.value != prevContacts[i].feature.value)
				{
					const glm::vec4 R1 = contact.position - arbiter.body1->position;
					const glm::vec4 R2 = contact.position - arbiter.body2->position;
					const glm::vec4 DV = arbiter.body2->GetInstantaneousLinearVelocityAtPoint(R2) - arbiter.body1->GetInstantaneousLinearVelocityAtPoint(R2);
					maxContactHitSpeed = glm::max(maxContactHitSpeed, glm::length(DV));
					hasNewHit = true;
				}
			}

			static float maxVolumeAtSpeed = 15.0f; // ~15 m/s is the free fall speed for a 10 meter fall
			static float minSpeed = 2.0f;
			if (hasNewHit)
			{
				int volume = 0;
				if (maxContactHitSpeed >= minSpeed)
				{
					volume = (int)glm::floor(SDL_MIX_MAXVOLUME * glm::min(1.0f, maxContactHitSpeed / maxVolumeAtSpeed));
				}
				playSoundFromMemory(rollSound, volume);
			}

			prevNumContacts = numContacts;
			for (int i = 0; i < numContacts; i++)
			{
				prevContacts[i] = contacts[i];
			}
		}
	}
	else
	{
		for (int i = 0; i < prevNumContacts; i++)
		{
			prevContacts[i].feature.value = Arbiter::MAX_POINTS + 1;
		}
		prevNumContacts = 0;
	}

	// Integrate forces.
	for (int i = 0; i < (int)bodies.size(); ++i)
	{
		Body* b = bodies[i];

		if (b->invMass == 0.0f)
			continue;

		b->velocity += dt * (gravity + b->invMass * b->force);
		b->angularVelocity += dt * b->invI * b->torque;
	}

	// Perform pre-steps.
	for (ArbIter arb = arbiters.begin(); arb != arbiters.end(); ++arb)
	{
		arb->second.PreStep(inv_dt);
	}

	// Perform iterations
	for (int i = 0; i < iterations; ++i)
	{
		for (ArbIter arb = arbiters.begin(); arb != arbiters.end(); ++arb)
		{
			arb->second.ApplyImpulse();
		}
	}

	// Integrate Velocities
	for (int i = 0; i < (int)bodies.size(); ++i)
	{
		Body* b = bodies[i];

		b->position += b->velocity * dt;
		b->rotation = glm::exponentiate(b->angularVelocity * dt) * b->rotation;

		b->force = glm::vec4(0.0f);
		b->torque = glm::mat4(0.0f);
	}

	if (NearEquilibrium())
	{
		Stop();
	}

#ifdef _DEBUG_DUMP_TELEMETRY
	DumpTelemetry();
#endif

	GetMemories().Comprehend();
}

bool World::NearEquilibrium() const
{
	if (arbiters.size() == 0)
	{
		return false;
	}

	bool allArbiterContactsAreResting = true;
	bool allBodiesAreAlmostAtRest = true;

	for (auto i = arbiters.begin(); i != arbiters.end(); ++i)
	{
		if (!i->second.AreAllContactsResting())
		{
			allArbiterContactsAreResting = false;
			break;
		}

		if (!i->second.AreAllBodiesAlmostAtRest())
		{
			allBodiesAreAlmostAtRest = false;
			break;
		}
	}

	return allArbiterContactsAreResting && allBodiesAreAlmostAtRest;
}

void World::Stop()
{
	for (auto i = arbiters.begin(); i != arbiters.end(); ++i)
	{
		i->second.NullifyContactsImpulseAccumulators();
	}

	for (int i = 0; i < (int)bodies.size(); ++i)
	{
		bodies[i]->NullifyVelocityVectors();
	}

	SetSimulationEnabled(false);
}

void World::SetSimulationEnabled(bool enabled)
{
	simulationEnabled = enabled;
}

void World::DebugDraw(unsigned int shader, Projector4D* projector)
{
	for (auto it : arbiters)
	{
		it.second.DebugDrawContacts(shader, projector);
	}
}

#ifdef _DEBUG_DUMP_TELEMETRY
void World::DumpTelemetry() const
{
	std::ofstream telemetry("dice_telemetry.txt", std::ios::app);

	const float v1 = bodies[0]->velocity.x;
	const float v2 = bodies[0]->velocity.y;
	const float v3 = bodies[0]->velocity.z;
	const float v4 = bodies[0]->velocity.w;
	const float vNorm = glm::length(bodies[0]->velocity);

	const float w1 = bodies[0]->angularVelocity[0][1];
	const float w2 = bodies[0]->angularVelocity[0][2];
	const float w3 = bodies[0]->angularVelocity[0][3];
	const float w4 = bodies[0]->angularVelocity[1][2];
	const float w5 = bodies[0]->angularVelocity[1][3];
	const float w6 = bodies[0]->angularVelocity[2][3];
	const float wNorm = glm::sqrt(
		bodies[0]->angularVelocity[0][1] * bodies[0]->angularVelocity[0][1] +
		bodies[0]->angularVelocity[0][2] * bodies[0]->angularVelocity[0][2] +
		bodies[0]->angularVelocity[0][3] * bodies[0]->angularVelocity[0][3] +
		bodies[0]->angularVelocity[1][2] * bodies[0]->angularVelocity[1][2] +
		bodies[0]->angularVelocity[1][3] * bodies[0]->angularVelocity[1][3] +
		bodies[0]->angularVelocity[2][3] * bodies[0]->angularVelocity[2][3]);

	telemetry	<< n++ << ","
				<< v1 << "," << v2 << "," << v3 << "," << v4 << "," << vNorm << ","
				<< w1 << "," << w2 << "," << w3 << "," << w4 << "," << w5 << "," << w6 << "," << wNorm << std::endl;
}
#endif

void World::Impress(MemoryChunk& chunk) const
{
	chunk.SetArbiters("arbiters", arbiters);
}

void World::Comprehend(MemoryChunk& chunk) const
{
	chunk.SetArbiters("next_arbiters", arbiters);
}

void World::Evoke(const MemoryChunk& chunk)
{
	arbiters = chunk.GetArbiters("next_arbiters");
}

void World::Commence(const MemoryChunk& chunk)
{
	arbiters = chunk.GetArbiters("arbiters");
}
