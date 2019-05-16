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

#ifndef ARBITER_H
#define ARBITER_H

struct ISolid;
struct Body;
struct HalfSpace;
class Projector4D;

#include <map>
#include <glm/glm.hpp>

union FeaturePair
{
	struct Edges
	{
		char inEdge1;
		char outEdge1;
		char inEdge2;
		char outEdge2;
	} e;
	int value;
};

struct Contact
{
	Contact();

	glm::vec4 position;
	glm::vec4 normal;
	glm::vec4 r1;
	glm::vec4 r2;
	float separation;
	float Pn;			// Accumulated normal impulse
	glm::vec4 Pt;		// Accumulated tangent impulse
	float massNormal;
	float massTangent;
	float bias;
	FeaturePair feature;
};

struct ArbiterKey
{
	ArbiterKey(Body* b1, Body* b2);
	ArbiterKey(HalfSpace* s1, Body* b2);

	ISolid* body1;
	ISolid* body2;
};

struct ArbiterParameters
{
	ArbiterParameters();
	ArbiterParameters(float contactDetectionEpsilon, float velocityNormThreshold, float angularVelocityNormThreshold, float restitutionCoefficient, int minNumOfContactsForStability);

	// Detect contact if the distance 'd' between two bodies satisfies:
	// d < 0 or 0 < d < contactDetectionEpsilon
	float contactDetectionEpsilon;

	// Assume negligible velocity if the norm of the velocity vector 'v',
	// respectively the norm of the angular velocity tensor 'w', is below
	// this threshold, i.e. if
	// |v| < velocityNormThreshold
	// respectively
	// |w| < angularVelocityNormThreshold
	float velocityNormThreshold;
	float angularVelocityNormThreshold;

	// The restitution coefficient relates the pre-collision relative velocity 'v_pre'
	// with the post-collision relative velocity 'v_post' of the contact point along
	// the contact normal 'n' as follows:
	// v_post * n = -restitutionCoefficient * v_pre * n
	float restitutionCoefficient;

	// The minimum number of contacts needed for the body to be considered "at rest".
	int minNumOfContactsForStability;
};

struct Arbiter
{
	enum {MAX_POINTS = 16}; // For hypercube case

	Arbiter(const ArbiterParameters& parameters, Body* b1, Body* b2);
	Arbiter(const ArbiterParameters& parameters, HalfSpace* s1, Body* b2);

	void Update(Contact* contacts, int numContacts);

	void PreStep(float inv_dt);
	void ApplyImpulse();

	bool AreAllBodiesAlmostAtRest() const;
	bool IsContactResting(const Contact& contact) const;
	bool AreAllContactsResting() const;
	void NullifyContactsImpulseAccumulators();

	void DebugDrawContacts(unsigned int shader, Projector4D* projector);

	Contact contacts[MAX_POINTS];
	int numContacts;

	ISolid* body1;
	ISolid* body2;

	// Combined friction
	float friction;

	ArbiterParameters parameters;
};

// This is used by std::set
inline bool operator < (const ArbiterKey& a1, const ArbiterKey& a2)
{
	if (a1.body1 < a2.body1)
		return true;

	if (a1.body1 == a2.body1 && a1.body2 < a2.body2)
		return true;

	return false;
}

int Collide(Contact* contacts, Body* body1, Body* body2);
int Collide(Contact* contacts, HalfSpace* hSpace, Body* body, float contactDetectionEpsilon);

typedef std::map<ArbiterKey, Arbiter> ArbitersMap;

#endif
