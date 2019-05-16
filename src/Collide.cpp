/*
* Copyright (c) 2006-2007 Erin Catto http://www.gphysics.com
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies.
* Erin Catto makes no representations about the suitability 
* of this software for any purpose.  
* It is provided "as is" without express or implied warranty.
*/

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>

#include "Arbiter.h"
#include "Body.h"
#include "HalfSpace.h"

// Box vertex and edge numbering:
//
//        ^ y
//        |
//        e1
//   v2 ------ v1
//    |        |
// e2 |        | e4  --> x
//    |        |
//   v3 ------ v4
//        e3

enum Axis
{
	FACE_A_X,
	FACE_A_Y,
	FACE_B_X,
	FACE_B_Y
};

enum EdgeNumbers
{
	NO_EDGE = 0,
	EDGE1,
	EDGE2,
	EDGE3,
	EDGE4
};

struct ClipVertex
{
	ClipVertex() { fp.value = 0; }
	glm::vec4 v;
	FeaturePair fp;
};

void Flip(FeaturePair& fp)
{
	std::swap(fp.e.inEdge1, fp.e.inEdge2);
	std::swap(fp.e.outEdge1, fp.e.outEdge2);
}

int ClipSegmentToLine(ClipVertex vOut[2], ClipVertex vIn[2],
					  const glm::vec2& normal, float offset, char clipEdge)
{
	// Start with no output points
	int numOut = 0;

	// Calculate the distance of end points to the line
	float distance0 = glm::dot(glm::vec4(normal, 0.0f, 0.0f), vIn[0].v) - offset;
	float distance1 = glm::dot(glm::vec4(normal, 0.0f, 0.0f), vIn[1].v) - offset;

	// If the points are behind the plane
	if (distance0 <= 0.0f) vOut[numOut++] = vIn[0];
	if (distance1 <= 0.0f) vOut[numOut++] = vIn[1];

	// If the points are on different sides of the plane
	if (distance0 * distance1 < 0.0f)
	{
		// Find intersection point of edge and plane
		float interp = distance0 / (distance0 - distance1);
		vOut[numOut].v = vIn[0].v + interp * (vIn[1].v - vIn[0].v);
		if (distance0 > 0.0f)
		{
			vOut[numOut].fp = vIn[0].fp;
			vOut[numOut].fp.e.inEdge1 = clipEdge;
			vOut[numOut].fp.e.inEdge2 = NO_EDGE;
		}
		else
		{
			vOut[numOut].fp = vIn[1].fp;
			vOut[numOut].fp.e.outEdge1 = clipEdge;
			vOut[numOut].fp.e.outEdge2 = NO_EDGE;
		}
		++numOut;
	}

	return numOut;
}

int IntersectSegmentWithHalfSpace(ClipVertex vOut[2], ClipVertex vIn[2],
	const glm::vec4& normal, float offset, char clipEdge)
{
	// Start with no output points
	int numOut = 0;

	// Calculate the distance of end points to the line
	float distance0 = glm::dot(normal, vIn[0].v) - offset;
	float distance1 = glm::dot(normal, vIn[1].v) - offset;

	// If the points are on different sides of the plane
	if (distance0 * distance1 < 0.0f)
	{
		// If the points are behind the plane
		if (distance0 <= 0.0f) vOut[numOut++] = vIn[0];
		if (distance1 <= 0.0f) vOut[numOut++] = vIn[1];

		// Find intersection point of edge and plane
		float interp = distance0 / (distance0 - distance1);
		vOut[numOut].v = vIn[0].v + interp * (vIn[1].v - vIn[0].v);
		if (distance0 > 0.0f)
		{
			vOut[numOut].fp = vIn[0].fp;
			vOut[numOut].fp.e.inEdge1 = clipEdge;
			vOut[numOut].fp.e.inEdge2 = NO_EDGE;
		}
		else
		{
			vOut[numOut].fp = vIn[1].fp;
			vOut[numOut].fp.e.outEdge1 = clipEdge;
			vOut[numOut].fp.e.outEdge2 = NO_EDGE;
		}
		++numOut;
	}
	else
	{
		// If the points are behind the plane
		if (distance0 <= 0.0f) vOut[numOut++] = vIn[0];
		if (distance1 <= 0.0f) vOut[numOut++] = vIn[1];
	}

	return numOut;
}

static void ComputeIncidentEdge(ClipVertex c[2], const glm::vec2& h, const glm::vec2& pos,
								const glm::mat2& Rot, const glm::vec2& normal)
{
	// The normal is from the reference box. Convert it
	// to the incident boxe's frame and flip sign.
	glm::mat2 RotT = glm::transpose(Rot);
	glm::vec2 n = -(RotT * normal);
	glm::vec2 nAbs = glm::abs(n);

	if (nAbs.x > nAbs.y)
	{
		if (glm::sign(n.x) > 0.0f)
		{
			c[0].v = glm::vec4(h.x, -h.y, 0.0f, 0.0f);
			c[0].fp.e.inEdge2 = EDGE3;
			c[0].fp.e.outEdge2 = EDGE4;

			c[1].v = glm::vec4(h.x, h.y, 0.0f, 0.0f);
			c[1].fp.e.inEdge2 = EDGE4;
			c[1].fp.e.outEdge2 = EDGE1;
		}
		else
		{
			c[0].v = glm::vec4(-h.x, h.y, 0.0f, 0.0f);
			c[0].fp.e.inEdge2 = EDGE1;
			c[0].fp.e.outEdge2 = EDGE2;

			c[1].v = glm::vec4(-h.x, -h.y, 0.0f, 0.0f);
			c[1].fp.e.inEdge2 = EDGE2;
			c[1].fp.e.outEdge2 = EDGE3;
		}
	}
	else
	{
		if (glm::sign(n.y) > 0.0f)
		{
			c[0].v = glm::vec4(h.x, h.y, 0.0f, 0.0f);
			c[0].fp.e.inEdge2 = EDGE4;
			c[0].fp.e.outEdge2 = EDGE1;

			c[1].v = glm::vec4(-h.x, h.y, 0.0f, 0.0f);
			c[1].fp.e.inEdge2 = EDGE1;
			c[1].fp.e.outEdge2 = EDGE2;
		}
		else
		{
			c[0].v = glm::vec4(-h.x, -h.y, 0.0f, 0.0f);
			c[0].fp.e.inEdge2 = EDGE2;
			c[0].fp.e.outEdge2 = EDGE3;

			c[1].v = glm::vec4(h.x, -h.y, 0.0f, 0.0f);
			c[1].fp.e.inEdge2 = EDGE3;
			c[1].fp.e.outEdge2 = EDGE4;
		}
	}

	c[0].v = glm::vec4(pos + Rot * c[0].v, 0.0f, 0.0f);
	c[1].v = glm::vec4(pos + Rot * c[1].v, 0.0f, 0.0f);
}

// The normal points from A to B
int Collide(Contact* contacts, Body* bodyA, Body* bodyB)
{
	// Setup
	glm::vec2 hA = glm::vec2(0.5f * bodyA->width);
	glm::vec2 hB = glm::vec2(0.5f * bodyB->width);

	glm::vec2 posA = bodyA->position;
	glm::vec2 posB = bodyB->position;

	glm::mat2x2 RotA = glm::mat2(bodyA->rotation);
	glm::mat2x2 RotB = glm::mat2(bodyB->rotation);

	glm::mat2 RotAT = glm::transpose(RotA);
	glm::mat2 RotBT = glm::transpose(RotB);

	glm::vec2 a1 = RotA[0], a2 = RotA[1];
	glm::vec2 b1 = RotB[0], b2 = RotB[1];

	glm::vec2 dp = posB - posA;
	glm::vec2 dA = RotAT * dp;
	glm::vec2 dB = RotBT * dp;

	glm::mat2 C = RotAT * RotB;
	glm::mat2 absC = glm::mat2(glm::abs(C[0]), glm::abs(C[1]));
	glm::mat2 absCT = glm::transpose(absC);

	// Box A faces
	glm::vec2 faceA = glm::abs(dA) - hA - absC * hB;
	if (faceA.x > 0.0f || faceA.y > 0.0f)
		return 0;

	// Box B faces
	glm::vec2 faceB = glm::abs(dB) - absCT * hA - hB;
	if (faceB.x > 0.0f || faceB.y > 0.0f)
		return 0;

	// Find best axis
	Axis axis;
	float separation;
	glm::vec2 normal;

	// Box A faces
	axis = FACE_A_X;
	separation = faceA.x;
	normal = dA.x > 0.0f ? RotA[0] : -RotA[0];

	const float relativeTol = 0.95f;
	const float absoluteTol = 0.01f;

	if (faceA.y > relativeTol * separation + absoluteTol * hA.y)
	{
		axis = FACE_A_Y;
		separation = faceA.y;
		normal = dA.y > 0.0f ? RotA[1] : -RotA[1];
	}

	// Box B faces
	if (faceB.x > relativeTol * separation + absoluteTol * hB.x)
	{
		axis = FACE_B_X;
		separation = faceB.x;
		normal = dB.x > 0.0f ? RotB[0] : -RotB[0];
	}

	if (faceB.y > relativeTol * separation + absoluteTol * hB.y)
	{
		axis = FACE_B_Y;
		separation = faceB.y;
		normal = dB.y > 0.0f ? RotB[1] : -RotB[1];
	}

	// Setup clipping plane data based on the separating axis
	glm::vec2 frontNormal, sideNormal;
	ClipVertex incidentEdge[2];
	float front, negSide, posSide;
	char negEdge, posEdge;

	// Compute the clipping lines and the line segment to be clipped.
	switch (axis)
	{
	case FACE_A_X:
		{
			frontNormal = normal;
			front = glm::dot(posA, frontNormal) + hA.x;
			sideNormal = RotA[1];
			float side = glm::dot(posA, sideNormal);
			negSide = -side + hA.y;
			posSide =  side + hA.y;
			negEdge = EDGE3;
			posEdge = EDGE1;
			ComputeIncidentEdge(incidentEdge, hB, posB, RotB, frontNormal);
		}
		break;

	case FACE_A_Y:
		{
			frontNormal = normal;
			front = glm::dot(posA, frontNormal) + hA.y;
			sideNormal = RotA[0];
			float side = glm::dot(posA, sideNormal);
			negSide = -side + hA.x;
			posSide =  side + hA.x;
			negEdge = EDGE2;
			posEdge = EDGE4;
			ComputeIncidentEdge(incidentEdge, hB, posB, RotB, frontNormal);
		}
		break;

	case FACE_B_X:
		{
			frontNormal = -normal;
			front = glm::dot(posB, frontNormal) + hB.x;
			sideNormal = RotB[1];
			float side = glm::dot(posB, sideNormal);
			negSide = -side + hB.y;
			posSide =  side + hB.y;
			negEdge = EDGE3;
			posEdge = EDGE1;
			ComputeIncidentEdge(incidentEdge, hA, posA, RotA, frontNormal);
		}
		break;

	case FACE_B_Y:
		{
			frontNormal = -normal;
			front = glm::dot(posB, frontNormal) + hB.y;
			sideNormal = RotB[0];
			float side = glm::dot(posB, sideNormal);
			negSide = -side + hB.x;
			posSide =  side + hB.x;
			negEdge = EDGE2;
			posEdge = EDGE4;
			ComputeIncidentEdge(incidentEdge, hA, posA, RotA, frontNormal);
		}
		break;
	}

	// clip other face with 5 box planes (1 face plane, 4 edge planes)

	ClipVertex clipPoints1[2];
	ClipVertex clipPoints2[2];
	int np;

	// Clip to box side 1
	np = ClipSegmentToLine(clipPoints1, incidentEdge, -sideNormal, negSide, negEdge);

	if (np < 2)
		return 0;

	// Clip to negative box side 1
	np = ClipSegmentToLine(clipPoints2, clipPoints1,  sideNormal, posSide, posEdge);

	if (np < 2)
		return 0;

	// Now clipPoints2 contains the clipping points.
	// Due to roundoff, it is possible that clipping removes all points.

	int numContacts = 0;
	for (int i = 0; i < 2; ++i)
	{
		float separation = glm::dot(frontNormal, glm::vec2(clipPoints2[i].v)) - front;

		if (separation <= 0)
		{
			contacts[numContacts].separation = separation;
			contacts[numContacts].normal = glm::vec4(normal, 0.0f, 0.0f);
			// slide contact point onto reference face (easy to cull)
			contacts[numContacts].position = clipPoints2[i].v - glm::vec4(separation * frontNormal, 0.0f, 0.0f);
			contacts[numContacts].feature = clipPoints2[i].fp;
			if (axis == FACE_B_X || axis == FACE_B_Y)
				Flip(contacts[numContacts].feature);
			++numContacts;
		}
	}

	return numContacts;
}

int Collide(Contact* contacts, HalfSpace* hSpace, Body* body, float contactDetectionEpsilon)
{
	const glm::vec4 posA = hSpace->position;
	const glm::vec4 posB = body->position;
	const glm::vec4 normal = hSpace->normal;
	const glm::mat4 rotB = body->rotation;

	int numContacts = 0;

	for (int i = 0; i < (int)body->bodyVertices.size(); ++i)
	{
		const glm::vec4 vertex = rotB * body->bodyVertices[i] + posB - posA;
		const float separation = glm::dot(vertex, normal);

		if (separation <= 0 || 0 < separation && separation < contactDetectionEpsilon)
		{
			contacts[numContacts].normal = normal;
			contacts[numContacts].separation = separation;
			contacts[numContacts].position = vertex + posA;
			contacts[numContacts].feature.value = i;
			++numContacts;
		}
	}

	return numContacts;
}

// The normal points from A to B
int CollideV2(Contact* contacts, HalfSpace* hSpace, Body* body)
{
	glm::vec4 posA = hSpace->position;
	glm::vec4 posB = body->position;

	glm::vec4 normal = hSpace->normal;

	glm::mat4 RotB = body->rotation;

	float separation = 0.0f;
	int numContacts = 0;
	for (int i = 0; i < (int)body->bodyEdges.size(); ++i)
	{
		auto& edge = body->bodyEdges[i];

		ClipVertex incidentEdge[2];
		incidentEdge[0].v = (RotB * body->bodyVertices[edge.first] + posB) - posA;
		incidentEdge[1].v = (RotB * body->bodyVertices[edge.second] + posB) - posA;

		ClipVertex clipPoints[2];
		int np = IntersectSegmentWithHalfSpace(clipPoints, incidentEdge, normal, 0.0f, i+1);

		if (np < 2)
			continue;

		separation = glm::dot(clipPoints[0].v, normal);

		contacts[numContacts].normal = normal;
		contacts[numContacts].separation = separation;
		// slide contact point onto reference face (easy to cull)
		contacts[numContacts].position = clipPoints[1].v + posA;
		contacts[numContacts].feature = clipPoints[1].fp;
		++numContacts;
	}

	return numContacts;
}