
#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include "SDL_opengles2.h"

#include <vector>
#include <string>
#include <map>

class MaterialManager
{
public:
	static MaterialManager* GetInstance();

	bool LoadShaders(const std::string& path);

	int GetShaderByName(std::string& shaderName);
	int GetShaderByMaterialName(const std::string& materialName);

private:
	MaterialManager();
	~MaterialManager();
	MaterialManager(MaterialManager const&) {};
	MaterialManager& operator=(MaterialManager const&) {};

	void InitShaderDefs();
	void DeleteShaders();
	void ReloadShaders();
	int CreateShader(std::string& shaderPath);
	GLuint LoadShader(GLenum type, const char *shaderSrc);

	std::string m_shaderPath;

	std::map<std::string, std::string> m_materialMap;
	std::map<std::string, int> m_shaderMap;

	static MaterialManager*	m_pInstance;
};

#endif