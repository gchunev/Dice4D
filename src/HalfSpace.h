
#ifndef HALFSPACE_H
#define HALFSPACE_H

#include "Solid.h"

struct HalfSpace : public ISolid
{
	HalfSpace();

	virtual void UpdateLinearVelocity(const glm::vec4& addedVelocity) {};
	virtual void UpdateAngularVelocity(const glm::mat4& addedAngularVelocity) {};
	virtual glm::vec4 GetInstantaneousLinearVelocityAtPoint(const glm::vec4& point) { static glm::vec4 result(0.0f);  return result; };
	virtual void IncrementContactCount(int increment) {};
	virtual void ResetContactCount() {};
	virtual bool IsAlmostAtRest(float vNormThreshold, float wNormThreshold, int minNumOfContactsForStability) const { return true; }
	virtual void NullifyVelocityVectors() { }

	void Set(const glm::vec4& pos, const glm::vec4& dir);

	glm::vec4 normal;
};

#endif
