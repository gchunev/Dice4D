#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "SDL_opengles2.h"

struct vertexData {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 UV;
};

struct textureData {
	unsigned int id;
	bool isGlobalTexture = false;
};

class Mesh
{
public:
	Mesh(GLenum primitiveType, std::vector<vertexData>* vd, std::vector<unsigned int>* id, std::vector<textureData>* td = NULL);
	~Mesh();

	void draw(unsigned int shader);

private:
	std::vector<vertexData> m_data;
	std::vector<textureData> m_textures;
	std::vector<unsigned int> m_indices;

	GLenum m_primitiveType;

	unsigned int m_VBO;
	unsigned int m_IND;
};
#endif
