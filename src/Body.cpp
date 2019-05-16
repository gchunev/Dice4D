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

#include "Body.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/integer.hpp>

#include <cassert>

Body::Body()
{
	dimensions = 4;
	width = 1.0f;
	numOfContacts = 0;

	rotation = glm::mat4(1.0f);
	velocity = glm::vec4(0.0f);
	angularVelocity = glm::mat4(0.0f);
	force = glm::vec4(0.0f);
	torque = glm::mat4(0.0f);

	UpdateGeometry();
}

void Body::UpdateGeometry()
{
	bodyVertices.clear();
	bodyEdges.clear();
	bodyFaces.clear();
	bodyCells.clear();

	// Setup Vertices
	int numVertices = (int)glm::pow(2, dimensions);
	bodyVertices.reserve(numVertices);
	bodyVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	for (int d = 0; d < dimensions; d++)
	{
		int numCurrentVertices = bodyVertices.size();

		for (int i = 0; i < numCurrentVertices; i++)
		{
			bodyVertices[i][d] = 0.5f * width;
		}

		for (int i = 0; i < numCurrentVertices; i++)
		{
			glm::vec4 tmp = bodyVertices[i];
			tmp[d] *= -1.0f;
			bodyVertices.push_back(tmp);
		}
	}

	// Setup Edges
	int numEdges = dimensions * (int)glm::pow(2, dimensions - 1);
	bodyEdges.reserve(numEdges);
	for (int d = 0; d < dimensions; d++)
	{
		if (d == 0)
		{
			bodyEdges.push_back(std::pair<glm::u8, glm::u8>(0, 1));
		}
		else
		{
			int numCurrentEdges = bodyEdges.size();

			glm::u8 idxOffset = (glm::u8)glm::pow(2, d);
			for (int i = 0; i < numCurrentEdges; i++)
			{
				std::pair<glm::u8, glm::u8> tmp = bodyEdges[i];
				tmp.first += idxOffset;
				tmp.second += idxOffset;
				bodyEdges.push_back(tmp);
			}

			for (int i = 0, j = 0; i < numCurrentEdges; i++, j += 2)
			{
				std::pair<glm::u8, glm::u8> newEgde1(bodyEdges[i].first, bodyEdges[i + numCurrentEdges].first);
				bodyEdges.push_back(newEgde1);

				std::pair<glm::u8, glm::u8> newEgde2(bodyEdges[i].second, bodyEdges[i + numCurrentEdges].second);
				bodyEdges.push_back(newEgde2);
			}
		}
	}

	// Setup Faces
	int numFaces = (int)glm::round(dimensions * (dimensions - 1) * glm::pow(2, dimensions - 3));
	bodyFaces.reserve(numFaces);
	for (int j = 0; j < dimensions; j++)
	{
		for (int i = j + 1; i < dimensions; i++)
		{
			glm::u8 i1 = 0;
			glm::u8 i2 = (glm::u8)glm::pow(2, j);
			glm::u8 i3 = (glm::u8)glm::pow(2, i);
			glm::u8 i4 = i2 +i3;

			if (dimensions == 2)
			{
				// CCW order:
				bodyFaces.push_back(glm::u8vec4(i1, i2, i4, i3));
			}
			else
			{
				for (int k = 0; k < dimensions; k++)
				{
					if (k == i || k == j)
					{
						continue;
					}

					if (dimensions == 3)
					{
						glm::u8 k0 = (glm::u8)glm::pow(2, k);
						if ((i + j) % 2)
						{
							bodyFaces.push_back(glm::u8vec4(i1, i2, i4, i3));
							bodyFaces.push_back(glm::u8vec4(i1 + k0, i3 + k0, i4 + k0, i2 + k0));
						}
						else
						{
							bodyFaces.push_back(glm::u8vec4(i1, i3, i4, i2));
							bodyFaces.push_back(glm::u8vec4(i1 + k0, i2 + k0, i4 + k0, i3 + k0));
						}
					}
					else
					{
						for (int m = k + 1; m < dimensions; m++)
						{
							if (m == i || m == j || m == k)
							{
								continue;
							}

							glm::u8 k0 = (glm::u8)glm::pow(2, k);
							glm::u8 m0 = (glm::u8)glm::pow(2, m);
							bodyFaces.push_back(glm::u8vec4(i1, i2, i4, i3));
							bodyFaces.push_back(glm::u8vec4(i1 + k0, i2 + k0, i4 + k0, i3 + k0));
							bodyFaces.push_back(glm::u8vec4(i1 + m0, i2 + m0, i4 + m0, i3 + m0));
							bodyFaces.push_back(glm::u8vec4(i1 + k0 + m0, i2 + k0 + m0, i4 + k0 + m0, i3 + k0 + m0));
						}
					}
				}
			}
		}
	}

	// Setup Cells
	int numCells = (int)glm::round((glm::factorial(dimensions) / (6 * glm::factorial(dimensions - 3))) * glm::pow(2, dimensions - 3));
	bodyCells.reserve(numCells);
	for (int k = 0; k < dimensions; k++)
	{
		for (int j = k + 1; j < dimensions; j++)
		{
			for (int i = j + 1; i < dimensions; i++)
			{
				glm::u8 i1 = 0;
				glm::u8 i2 = (glm::u8)glm::pow(2, k);
				glm::u8 i3 = (glm::u8)glm::pow(2, j);
				glm::u8 i4 = i2 + i3;

				glm::u8 i0 = (glm::u8)glm::pow(2, i);
				glm::u8 i5 = i1 + i0;
				glm::u8 i6 = i2 + i0;
				glm::u8 i7 = i3 + i0;
				glm::u8 i8 = i4 + i0;

				if (dimensions == 3)
				{
					bodyCells.push_back(std::pair<glm::u8vec4, glm::u8vec4>(glm::u8vec4(i1, i2, i4, i3), glm::u8vec4(i5, i6, i8, i7)));
				}
				else
				{
					for (int m = 0; m < dimensions; m++)
					{
						if (m == i || m == j || m == k)
						{
							continue;
						}

						glm::u8 m0 = (glm::u8)glm::pow(2, m);
						if ((i + j + k) % 2)
						{
							bodyCells.push_back(std::pair<glm::u8vec4, glm::u8vec4>(glm::u8vec4(i1, i2, i4, i3), glm::u8vec4(i5, i6, i8, i7)));
							bodyCells.push_back(std::pair<glm::u8vec4, glm::u8vec4>(glm::u8vec4(i1 + m0, i3 + m0, i4 + m0, i2 + m0),
																					glm::u8vec4(i5 + m0, i7 + m0, i8 + m0, i6 + m0)));
						}
						else
						{
							bodyCells.push_back(std::pair<glm::u8vec4, glm::u8vec4>(glm::u8vec4(i1, i3, i4, i2), glm::u8vec4(i5, i7, i8, i6)));
							bodyCells.push_back(std::pair<glm::u8vec4, glm::u8vec4>(glm::u8vec4(i1 + m0, i2 + m0, i4 + m0, i3 + m0),
																					glm::u8vec4(i5 + m0, i6 + m0, i8 + m0, i7 + m0)));
							
						}
					}
				}
			}
		}
	}

#ifdef _DEBUG
	CheckGeometry();
#endif
}

void Body::CheckGeometry()
{
	if (dimensions == 3)
	{
		for (auto& face : bodyFaces)
		{
			auto& v1 = bodyVertices[face[0]];
			auto& v2 = bodyVertices[face[1]];
			auto& v3 = bodyVertices[face[2]];
			auto& v4 = bodyVertices[face[3]];

			glm::vec3 trueNormal = glm::normalize(v1 + v2 + v3 + v4);
			auto impliedNormal = glm::normalize(glm::cross(glm::vec3(v2 - v1), glm::vec3(v3 - v2)));
			assert(glm::dot(impliedNormal, trueNormal) > 0.99f);
		}
	}

	if (dimensions == 4)
	{
		for (auto& cell : bodyCells)
		{
			auto& v1 = bodyVertices[cell.first[0]];
			auto& v2 = bodyVertices[cell.first[1]];
			auto& v3 = bodyVertices[cell.first[2]];
			auto& v4 = bodyVertices[cell.first[3]];
			auto& v5 = bodyVertices[cell.second[0]];
			auto& v6 = bodyVertices[cell.second[1]];
			auto& v7 = bodyVertices[cell.second[2]];
			auto& v8 = bodyVertices[cell.second[3]];

			auto trueNormal = glm::normalize(v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8);
			auto impliedNormal = glm::normalize(glm::cross(v2 - v1, v3 - v2, v3 - v7));
			assert(glm::dot(impliedNormal, trueNormal) > 0.99f);
		}
	}
}

void Body::UpdateLinearVelocity(const glm::vec4& addedVelocity)
{
	velocity += addedVelocity;
}

void Body::UpdateAngularVelocity(const glm::mat4& addedAngularVelocity)
{
	angularVelocity += addedAngularVelocity;
}

glm::vec4 Body::GetInstantaneousLinearVelocityAtPoint(const glm::vec4& point)
{
	glm::vec4 retVal = velocity + angularVelocity * point;
	return retVal;
}

void Body::IncrementContactCount(int increment)
{
	numOfContacts += increment;
}

void Body::ResetContactCount()
{
	numOfContacts = 0;
}

bool Body::IsAlmostAtRest(float vNormThreshold, float wNormThreshold, int minNumOfContactsForStability) const
{
	if (numOfContacts < minNumOfContactsForStability)
	{
		return false;
	}

	const float vNorm = glm::length(velocity);
	const float wNorm = glm::sqrt(
		angularVelocity[0][1] * angularVelocity[0][1] +
		angularVelocity[0][2] * angularVelocity[0][2] +
		angularVelocity[0][3] * angularVelocity[0][3] +
		angularVelocity[1][2] * angularVelocity[1][2] +
		angularVelocity[1][3] * angularVelocity[1][3] +
		angularVelocity[2][3] * angularVelocity[2][3]);

	return vNorm < vNormThreshold && wNorm < wNormThreshold;
}

void Body::NullifyVelocityVectors()
{
	velocity = glm::vec4(0.0f);
	angularVelocity = glm::mat4(0.0f);
}

void Body::Set(int dim, float w, float m)
{
	width = w;
	dimensions = dim;

	position = glm::vec4(0.0f);
	rotation = glm::mat4(1.0f);
	velocity = glm::vec4(0.0f);
	angularVelocity = glm::mat4(0.0f);
	force = glm::vec4(0.0f);
	torque = glm::mat4(0.0f);

	UpdateGeometry();

	mass = m;
	if (mass < FLT_MAX)
	{
		invMass = 1.0f / mass;
		I = mass * (width * width) / 6.0f;
		invI = 1.0f / I;
	}
	else
	{
		invMass = 0.0f;
		I = FLT_MAX;
		invI = 0.0f;
	}
}

void Body::AddForce(const glm::vec4& f)
{
	force += f;
}

void Body::Impress(MemoryChunk& chunk) const
{
	ISolid::Impress(chunk);

	chunk.SetMat4("rotation", rotation);
	chunk.SetVec4("velocity", velocity);
	chunk.SetMat4("angularVelocity", angularVelocity);
	chunk.SetVec4("force", force);
	chunk.SetMat4("torque", torque);
}

void Body::Comprehend(MemoryChunk& chunk) const
{
	ISolid::Comprehend(chunk);
}

void Body::Evoke(const MemoryChunk& chunk)
{
	ISolid::Evoke(chunk);

	rotation = chunk.GetMat4("rotation");
	velocity = chunk.GetVec4("velocity");
	angularVelocity = chunk.GetMat4("angularVelocity");
	force = chunk.GetVec4("force");
	torque = chunk.GetMat4("torque");
}

void Body::Commence(const MemoryChunk& chunk)
{
	ISolid::Commence(chunk);

	rotation = chunk.GetMat4("rotation");
	velocity = chunk.GetVec4("velocity");
	angularVelocity = chunk.GetMat4("angularVelocity");
	force = chunk.GetVec4("force");
	torque = chunk.GetMat4("torque");
}
