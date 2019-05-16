#include "GraphicsUtils.h"

#include "Mesh.h"

Mesh::Mesh(GLenum primitiveType, std::vector<vertexData>* vd, std::vector<unsigned int>* id, std::vector<textureData>* td)
{
	m_data=*vd;
	m_indices=*id;
	if (td)
	{
		m_textures = *td;
	}

	m_primitiveType = primitiveType;
	
	ctx.glGenBuffers(1,&m_VBO);
	ctx.glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	ctx.glBufferData(GL_ARRAY_BUFFER, m_data.size()*sizeof(vertexData),
	&m_data[0],GL_STATIC_DRAW);
	
	
	ctx.glGenBuffers(1,&m_IND);
	ctx.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IND);
	ctx.glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(unsigned int),&m_indices[0],GL_STATIC_DRAW);
	
	ctx.glBindBuffer(GL_ARRAY_BUFFER,0);
	ctx.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

Mesh::~Mesh()
{
	ctx.glDeleteBuffers(1,&m_VBO);
	ctx.glDeleteBuffers(1,&m_IND);

	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		if (!m_textures[i].isGlobalTexture)
		{
			ctx.glDeleteTextures(1, &m_textures[i].id);
		}
	}
	m_textures.clear();
}

void Mesh::draw(unsigned int shader)
{
	//attribute vec3 vertex
	int vertex = ctx.glGetAttribLocation(shader,"vertex"); //0
	int color = ctx.glGetAttribLocation(shader,"color"); //1
	int UV = ctx.glGetAttribLocation(shader, "UV"); //2

	//texture0
	//texture1...
	std::string str = "texture";
	for (unsigned int i = 0; i<m_textures.size(); i++)
	{
		ctx.glActiveTexture(GL_TEXTURE0 + i);
		ctx.glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
		auto texAddr = ctx.glGetUniformLocation(shader, (str + (char)(i + '0')).c_str());
		ctx.glUniform1i(texAddr, i);
	}
	
	ctx.glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	ctx.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IND);
	
	if (vertex != -1)
	{
		ctx.glEnableVertexAttribArray(vertex);
		ctx.glVertexAttribPointer(vertex,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),0);
	}
	
	if (color != -1)
	{
		ctx.glEnableVertexAttribArray(color);
		ctx.glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(3 * sizeof(float)));
	}

	if (UV != -1)
	{
		ctx.glEnableVertexAttribArray(UV);
		ctx.glVertexAttribPointer(UV, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), (void*)(6 * sizeof(float)));
	}
	
	ctx.glDrawElements(m_primitiveType, m_indices.size(),GL_UNSIGNED_INT,0);
	
	if (vertex!=-1) ctx.glDisableVertexAttribArray(vertex);
	if (color!=-1) ctx.glDisableVertexAttribArray(color);
	if (UV != -1) ctx.glDisableVertexAttribArray(UV);
	ctx.glBindBuffer(GL_ARRAY_BUFFER,0);
	ctx.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
