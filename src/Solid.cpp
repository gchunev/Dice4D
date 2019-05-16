
#include "Solid.h"

ISolid::ISolid()
{
	position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	friction = 1.0f;
	mass = FLT_MAX;
	invMass = 0.0f;
	I = FLT_MAX;
	invI = 0.0f;
}

void ISolid::Impress(MemoryChunk& chunk) const
{
	chunk.SetVec4("position", position);
	chunk.SetFloat("friction", friction);
	chunk.SetFloat("mass", mass);
	chunk.SetFloat("invMass", invMass);
	chunk.SetFloat("I", I);
	chunk.SetFloat("invI", invI);
}

void ISolid::Comprehend(MemoryChunk& chunk) const
{

}

void ISolid::Evoke(const MemoryChunk& chunk)
{
	position = chunk.GetVec4("position");
	friction = chunk.GetFloat("friction");
	mass = chunk.GetFloat("mass");
	invMass = chunk.GetFloat("invMass");
	I = chunk.GetFloat("I");
	invI = chunk.GetFloat("invI");
}

void ISolid::Commence(const MemoryChunk& chunk)
{
	position = chunk.GetVec4("position");
	friction = chunk.GetFloat("friction");
	mass = chunk.GetFloat("mass");
	invMass = chunk.GetFloat("invMass");
	I = chunk.GetFloat("I");
	invI = chunk.GetFloat("invI");
}
