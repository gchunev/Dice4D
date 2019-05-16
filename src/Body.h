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

#ifndef BODY_H
#define BODY_H

#include "Solid.h"

#include <vector>

struct Body : public ISolid
{
	Body();

	void UpdateGeometry();
	void CheckGeometry();

	virtual void UpdateLinearVelocity(const glm::vec4& addedVelocity);
	virtual void UpdateAngularVelocity(const glm::mat4& addedAngularVelocity);
	virtual glm::vec4 GetInstantaneousLinearVelocityAtPoint(const glm::vec4& point);
	virtual void IncrementContactCount(int increment);
	virtual void ResetContactCount();
	virtual bool IsAlmostAtRest(float vNormThreshold, float wNormThreshold, int minNumOfContactsForStability) const;
	virtual void NullifyVelocityVectors();

	void Set(int dim, float w, float m);
	void AddForce(const glm::vec4& f);

	virtual void Impress(MemoryChunk& chunk) const override;
	virtual void Comprehend(MemoryChunk& chunk) const override;
	virtual void Evoke(const MemoryChunk& chunk) override;
	virtual void Commence(const MemoryChunk& chunk) override;

	int dimensions;
	float width;
	int numOfContacts;

	glm::mat4 rotation;
	glm::vec4 velocity;
	glm::mat4 angularVelocity;

	glm::vec4 force;
	glm::mat4 torque;

	std::vector<glm::vec4> bodyVertices;
	std::vector< std::pair<glm::u8, glm::u8> > bodyEdges;
	std::vector<glm::u8vec4> bodyFaces;
	std::vector< std::pair<glm::u8vec4, glm::u8vec4> > bodyCells;
};

#endif
