#include "MaterialManager.h"

#include "GraphicsUtils.h"

#include <rapidxml/rapidxml.hpp>

#include <iostream>
#include <fstream>
#include <vector>

using namespace rapidxml;

MaterialManager* MaterialManager::m_pInstance = NULL;

MaterialManager* MaterialManager::GetInstance()
{
	if (!m_pInstance)
		m_pInstance = new MaterialManager;

	return m_pInstance;
}

MaterialManager::MaterialManager()
{
	InitShaderDefs();
};

MaterialManager::~MaterialManager()
{
	DeleteShaders();
};

void MaterialManager::InitShaderDefs()
{
	m_materialMap[std::string("")] = std::string("shader_basic.xml"); //Default Material
	m_materialMap[std::string("Simple-material")] = std::string("shader_basic.xml");
	m_materialMap[std::string("Texts-material")] = std::string("shader_texts.xml");
	m_materialMap[std::string("Volumetric-material")] = std::string("shader_volume_renderer.xml");
}

void MaterialManager::DeleteShaders()
{
	std::map<std::string, int>::iterator sit;

	for (sit = m_shaderMap.begin(); sit != m_shaderMap.end(); sit++)
	{
		ctx.glDeleteProgram(sit->second);
	}
	m_shaderMap.clear();
}

void MaterialManager::ReloadShaders()
{
	DeleteShaders();
	LoadShaders(m_shaderPath);
}

/////////////////////////////////////////////////////
// Class Interface:
/////////////////////////////////////////////////////

bool MaterialManager::LoadShaders(const std::string& path)
{
	m_shaderPath = path;

	std::map<std::string, std::string>::iterator it;
	std::map<std::string, int>::iterator sit;

	if (m_shaderMap.size() > 0)
	{
		printf("Shaders have already been loaded with this manager\n");
		return false;
	}

	for (it = m_materialMap.begin(); it != m_materialMap.end(); it++)
	{
		sit = m_shaderMap.find(it->second);
		if (sit == m_shaderMap.end())
		{
			std::string shaderPath = path + it->second;
			int programObject = CreateShader(shaderPath);
			if (programObject != -1)
			{
				m_shaderMap[it->second] = programObject;
			}
			else
			{
				DeleteShaders();
				return false;
			}
		}
	}

	return true;
}

int MaterialManager::GetShaderByName(std::string& shaderName)
{
	int shader;
	std::map<std::string, int>::iterator sit;

	sit = m_shaderMap.find(shaderName);
	if (sit != m_shaderMap.end())
	{
		shader = sit->second;
	}
	else
	{
		//printf("No compiled shader matches: %s\n Falling back to default shader\n", shaderName.c_str());
		shader = m_shaderMap[""];
	}

	return shader;
}

int MaterialManager::GetShaderByMaterialName(const std::string& materialName)
{
	int shader;
	std::map<std::string, std::string>::iterator it;

	it = m_materialMap.find(materialName);
	if (it != m_materialMap.end())
	{
		shader = GetShaderByName(it->second);
	}
	else
	{
		printf("No predefined material matches: %s\n Falling back to default material\n", materialName.c_str());
		shader = GetShaderByName(m_materialMap[std::string("")]);
	}

	return shader;
}

/////////////////////////////////////////////////////
// Helper Routines:
/////////////////////////////////////////////////////

///
// Initialize the shader and program object
//
int MaterialManager::CreateShader(std::string& shaderPath)
{
	xml_document<> shaderDoc;
	xml_node<> * root_node;
	std::ifstream theFile;

	theFile.open(shaderPath.c_str());
	bool isOpen = theFile.is_open();
	if (isOpen)
	{
		printf("Reading Shader: %s\n", shaderPath.c_str());
	}
	else
	{
		printf("Failed to Read Shader: %s\n", shaderPath.c_str());
	}

	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());	buffer.push_back('\0');
	theFile.close();


	shaderDoc.parse<0>(&buffer[0]);
	root_node = shaderDoc.first_node("shader");

	xml_node<> * vshader_node = root_node->first_node("vshader");
	const char* vShaderStr = vshader_node->first_node(NULL)->value();
	std::string vShaderSource(vShaderStr);

	xml_node<> * fshader_node = root_node->first_node("fshader");
	const char* fShaderStr = fshader_node->first_node(NULL)->value();
	std::string fShaderSource(fShaderStr);

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	// Load the vertex/fragment shaders
	vertexShader = LoadShader(GL_VERTEX_SHADER, (const char *)vShaderSource.c_str());
	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, (const char *)fShaderSource.c_str());

	// Create the program object
	programObject = ctx.glCreateProgram();

	if (programObject == 0)
		return -1;

	ctx.glAttachShader(programObject, vertexShader);
	ctx.glAttachShader(programObject, fragmentShader);

	// Link the program
	ctx.glLinkProgram(programObject);

	// Check the link status
	ctx.glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint infoLen = 0;

		ctx.glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = (char *)malloc(sizeof(char) * infoLen);

			ctx.glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
			printf("Error linking program:\n%s\n", infoLog);

			free(infoLog);
		}

		ctx.glDeleteProgram(programObject);
		return -1;
	}

	return programObject;
}

///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint MaterialManager::LoadShader(GLenum type, const char *shaderSrc)
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = ctx.glCreateShader(type);

	if (shader == 0)
		return 0;

	// Load the shader source
	ctx.glShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile the shader
	ctx.glCompileShader(shader);

	// Check the compile status
	ctx.glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint infoLen = 0;

		ctx.glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = (char *)malloc(sizeof(char) * infoLen);
			GLsizei len;

			ctx.glGetShaderInfoLog(shader, infoLen, &len, infoLog);
			printf("Error compiling shader:\n%s\n", infoLog);

			free(infoLog);
		}

		ctx.glDeleteShader(shader);
		return 0;
	}

	return shader;
}

