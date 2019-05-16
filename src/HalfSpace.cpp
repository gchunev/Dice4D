
#include "HalfSpace.h"

#include <cmath>

#include <glm/gtc/constants.hpp>

HalfSpace::HalfSpace()
	: normal(0.0f, 1.0f, 0.0f, 0.0f)
{

}

void HalfSpace::Set(const glm::vec4& pos, const glm::vec4& dir)
{
	position = pos;
	normal = glm::normalize(dir);
}
