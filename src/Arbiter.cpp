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

#include "Arbiter.h"
#include "Body.h"
#include "HalfSpace.h"
#include "World.h"
#include "Mesh.h"
#include "GraphicsUtils.h"

Contact::Contact()
	: Pn(0.0f), Pt(0.0f)
{

}

ArbiterKey::ArbiterKey(Body* b1, Body* b2)
{
	if (b1 < b2)
	{
		body1 = b1; body2 = b2;
	}
	else
	{
		body1 = b2; body2 = b1;
	}
}

ArbiterKey::ArbiterKey(HalfSpace* s1, Body* b2)
{
	body1 = s1;
	body2 = b2;
}

ArbiterParameters::ArbiterParameters()
	: contactDetectionEpsilon(0.0f)
	, velocityNormThreshold(0.0f)
	, angularVelocityNormThreshold(0.0f)
	, minNumOfContactsForStability(0)
{

}

ArbiterParameters::ArbiterParameters(float contactDetectionEpsilon, float velocityNormThreshold, float angularVelocityNormThreshold, float restitutionCoefficient, int minNumOfContactsForStability)
	: contactDetectionEpsilon(contactDetectionEpsilon)
	, velocityNormThreshold(velocityNormThreshold)
	, angularVelocityNormThreshold(angularVelocityNormThreshold)
	, restitutionCoefficient(restitutionCoefficient)
	, minNumOfContactsForStability(minNumOfContactsForStability)
{

}

Arbiter::Arbiter(const ArbiterParameters& parameters, Body* b1, Body* b2)
	: parameters(parameters)
{
	if (b1 < b2)
	{
		body1 = b1;
		body2 = b2;
	}
	else
	{
		body1 = b2;
		body2 = b1;
	}

	numContacts = Collide(contacts, static_cast<Body*>(body1), static_cast<Body*>(body2));

	body1->IncrementContactCount(numContacts);
	body2->IncrementContactCount(numContacts);

	friction = sqrtf(body1->friction * body2->friction);
}

Arbiter::Arbiter(const ArbiterParameters& parameters, HalfSpace* s1, Body* b2)
	: parameters(parameters)
	, body1(s1)
	, body2(b2)
{
	numContacts = Collide(contacts, s1, b2, parameters.contactDetectionEpsilon);

	body1->IncrementContactCount(numContacts);
	body2->IncrementContactCount(numContacts);

	friction = sqrtf(body1->friction * body2->friction);
}

void Arbiter::Update(Contact* newContacts, int numNewContacts)
{
	Contact mergedContacts[MAX_POINTS];

	for (int i = 0; i < numNewContacts; ++i)
	{
		Contact* cNew = newContacts + i;
		int k = -1;

		for (int j = 0; j < numContacts; ++j)
		{
			Contact* cOld = contacts + j;
			if (cNew->feature.value == cOld->feature.value)
			{
				k = j;
				break;
			}
		}

		if (k > -1)
		{
			Contact* c = mergedContacts + i;
			Contact* cOld = contacts + k;
			*c = *cNew;
			if (World::warmStarting)
			{
				c->Pn = cOld->Pn;
				c->Pt = cOld->Pt;
			}
			else
			{
				c->Pn = 0.0f;
				c->Pt = glm::vec4(0.0f);
			}
		}
		else
		{
			mergedContacts[i] = newContacts[i];
		}
	}

	for (int i = 0; i < numNewContacts; ++i)
	{
		contacts[i] = mergedContacts[i];
	}

	numContacts = numNewContacts;
}

void Arbiter::PreStep(float inv_dt)
{
	ISolid* b1 = body1;
	ISolid* b2 = body2;

	for (int i = 0; i < numContacts; ++i)
	{
		Contact* c = contacts + i;

		const glm::vec4 R1 = c->position - b1->position;
		const glm::vec4 R2 = c->position - b2->position;

		const glm::vec4 DV = b2->GetInstantaneousLinearVelocityAtPoint(R2) - b1->GetInstantaneousLinearVelocityAtPoint(R1);
		const float DVnLength = glm::dot(DV, c->normal);

		c->bias = -DVnLength * parameters.restitutionCoefficient;

		// Precompute normal and tangent masses
		const float R1n = glm::dot(R1, c->normal);
		const float R2n = glm::dot(R2, c->normal);
		float kNormal = b1->invMass + b2->invMass;
		kNormal += b1->invI * (glm::dot(R1, R1) - R1n * R1n) + b2->invI * (glm::dot(R2, R2) - R2n * R2n);
		c->massNormal = 1.0f / kNormal;

		const glm::vec4 R1t = R1 - R1n * c->normal;
		const glm::vec4 R2t = R2 - R2n * c->normal;
		float kTangent = b1->invMass + b2->invMass;
		kTangent += b1->invI * (glm::dot(R1, R1) - glm::dot(R1t, R1t)) + b2->invI * (glm::dot(R2, R2) - glm::dot(R2t, R2t));
		c->massTangent = 1.0f /  kTangent;

		const glm::vec4 P = c->Pn * c->normal + c->Pt;

		// Apply the accumulated normal and tangent impulses
		b1->UpdateLinearVelocity(-b1->invMass * P);
		b1->UpdateAngularVelocity(glm::tensor_cross(R1, -b1->invI * P));

		b2->UpdateLinearVelocity(b2->invMass * P);
		b2->UpdateAngularVelocity(glm::tensor_cross(R2, b2->invI * P));
	}
}

void Arbiter::ApplyImpulse()
{
	ISolid* b1 = body1;
	ISolid* b2 = body2;

	// Compute and apply the corrective tangent impulse
	for (int i = 0; i < numContacts; ++i)
	{
		Contact* c = contacts + i;

		c->r1 = c->position - b1->position;
		c->r2 = c->position - b2->position;

		// Relative velocity at contact
		const glm::vec4 DV = b2->GetInstantaneousLinearVelocityAtPoint(c->r2) - b1->GetInstantaneousLinearVelocityAtPoint(c->r1);
		const float DVnLength = glm::dot(DV, c->normal);
		const glm::vec4 DVn = DVnLength * c->normal;
		const glm::vec4 DVt = DV - DVn;

		glm::vec4 DPt = -c->massTangent * DVt; // To add "drag" - / speedDamping 

		// Compute
		const float maxPtLength = friction * c->Pn;

		// Clamp
		const glm::vec4 oldPt = c->Pt;
		const glm::vec4 newPt = oldPt + DPt;
		const float newPtLength = glm::length(oldPt + DPt);
		const float newPtLengthClamped = glm::clamp(newPtLength, 0.0f, maxPtLength);

		c->Pt = newPtLength > std::numeric_limits<float>::epsilon() ? newPt * newPtLengthClamped / newPtLength : glm::vec4(0.0f);
		DPt = c->Pt - oldPt;

		// Apply
		b1->UpdateLinearVelocity(-b1->invMass * DPt);
		b1->UpdateAngularVelocity(glm::tensor_cross(c->r1, -b1->invI * DPt));

		b2->UpdateLinearVelocity(b2->invMass * DPt);
		b2->UpdateAngularVelocity(glm::tensor_cross(c->r2, b2->invI * DPt));
	}

	// Compute and apply the corrective normal impulse
	for (int i = 0; i < numContacts; ++i)
	{
		Contact* c = contacts + i;

		c->r1 = c->position - b1->position;
		c->r2 = c->position - b2->position;

		// Relative velocity at contact
		const glm::vec4 DV = b2->GetInstantaneousLinearVelocityAtPoint(c->r2) - b1->GetInstantaneousLinearVelocityAtPoint(c->r1);
		const float DVnLength = glm::dot(DV, c->normal);

		// Compute
		const float DPnSignedLength = -c->massNormal * (DVnLength - c->bias);

		// Clamp
		const float oldPnLength = c->Pn;
		c->Pn = glm::max(oldPnLength + DPnSignedLength, 0.0f);
		const glm::vec4 DPn = (c->Pn - oldPnLength) * c->normal;

		// Apply
		b1->UpdateLinearVelocity(-b1->invMass * DPn);
		b1->UpdateAngularVelocity(glm::tensor_cross(c->r1, -b1->invI * DPn));

		b2->UpdateLinearVelocity(b2->invMass * DPn);
		b2->UpdateAngularVelocity(glm::tensor_cross(c->r2, b2->invI * DPn));
	}
}

bool Arbiter::AreAllBodiesAlmostAtRest() const
{
	return
		body1->IsAlmostAtRest(parameters.velocityNormThreshold, parameters.angularVelocityNormThreshold, parameters.minNumOfContactsForStability) &&
		body2->IsAlmostAtRest(parameters.velocityNormThreshold, parameters.angularVelocityNormThreshold, parameters.minNumOfContactsForStability);
}

bool Arbiter::IsContactResting(const Contact& contact) const
{
	const glm::vec4 R1 = contact.position - body1->position;
	const glm::vec4 R2 = contact.position - body2->position;
	const glm::vec4 DV = body2->GetInstantaneousLinearVelocityAtPoint(R2) - body1->GetInstantaneousLinearVelocityAtPoint(R1);
	const float DVnLength = std::abs(glm::dot(DV, contact.normal));

	return DVnLength < parameters.velocityNormThreshold;
}

bool Arbiter::AreAllContactsResting() const
{
	for (int i = 0; i < numContacts; ++i)
	{
		if (!IsContactResting(contacts[i]))
		{
			return false;
		}
	}

	return true;
}

void Arbiter::NullifyContactsImpulseAccumulators()
{
	for (int i = 0; i < numContacts; ++i)
	{
		contacts[i].Pn = 0.0f;
		contacts[i].Pt = glm::vec4(0.0f);
	}
}

void Arbiter::DebugDrawContacts(unsigned int shader, Projector4D* projector)
{
	std::vector<vertexData> vtxData;
	std::vector<unsigned int> idxData;

	ctx.glUniform4f(ctx.glGetUniformLocation(shader, "diffuseColor"), 1.0f, 0.0f, 0.0f, 1.0f);

	vertexData v1;
	for (int i = 0; i < numContacts; ++i)
	{
		v1.position = projector->Project(contacts[i].position);
		vtxData.push_back(v1);
		idxData.push_back(i);
	}

	Mesh points(GL_POINTS, &vtxData, &idxData);
	points.draw(shader);
}
