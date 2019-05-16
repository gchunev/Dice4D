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

#ifndef WORLD_H
#define WORLD_H

#include "Arbiter.h"
#include "Memories.h"
#include "Projector4D.h"

#include <glm/glm.hpp>

#include <vector>
#include <map>

struct World : public IMemorable
{
	World(const glm::vec4& gravity, int iterations);

	void SetArbiterParameters(float contactDetectionEpsilon, float velocityNormThreshold, float angularVelocityNormThreshold, float restitutionCoefficient, int minNumOfContactsForStability);
	void Add(Body* body);
	void Add(HalfSpace* plane);
	void Clear();

	void BroadPhase();

	void Simulate(float dt);

	bool NearEquilibrium() const;

	void Stop();
	void SetSimulationEnabled(bool enable = true);

	void DebugDraw(unsigned int shader, Projector4D* projector);

#ifdef _DEBUG_DUMP_TELEMETRY
	void DumpTelemetry() const;
#endif

	virtual void Impress(MemoryChunk& chunk) const override;
	virtual void Comprehend(MemoryChunk& chunk) const override;
	virtual void Evoke(const MemoryChunk& chunk) override;
	virtual void Commence(const MemoryChunk& chunk) override;

	std::vector<Body*> bodies;
	std::vector<HalfSpace*> planes;
	std::map<ArbiterKey, Arbiter> arbiters;
	ArbiterParameters arbiterParameters;

	glm::vec4 gravity;
	int iterations;
	bool simulationEnabled;

	static bool warmStarting;
	static bool reinitDieParams;
	static bool isReplaying;
#ifdef _DEBUG_DUMP_TELEMETRY
	static unsigned int n;
#endif
};

#endif
