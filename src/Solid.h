#ifndef SOLID_H
#define SOLID_H

#include <glm/glm.hpp>

#include "Memories.h"

struct ISolid : public IMemorable
{
	ISolid();
	virtual ~ISolid() {}

	virtual void UpdateLinearVelocity(const glm::vec4& addedVelocity) = 0;
	virtual void UpdateAngularVelocity(const glm::mat4& addedAngularVelocity) = 0;
	virtual glm::vec4 GetInstantaneousLinearVelocityAtPoint(const glm::vec4& point) = 0;
	virtual void IncrementContactCount(int increment) = 0;
	virtual void ResetContactCount() = 0;
	virtual bool IsAlmostAtRest(float vNormThreshold, float wNormThreshold, int minNumOfContactsForStability) const = 0;
	virtual void NullifyVelocityVectors() = 0;

	virtual void Impress(MemoryChunk& chunk) const override;
	virtual void Comprehend(MemoryChunk& chunk) const override;
	virtual void Evoke(const MemoryChunk& chunk) override;
	virtual void Commence(const MemoryChunk& chunk) override;

	glm::vec4 position;

	float friction;
	float mass, invMass;
	float I, invI;
};

#endif
