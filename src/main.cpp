#include <string.h>
#include <vector>
#include <list>

#include "SDL.h"
#include "SDL_ttf.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>

#include "GraphicsUtils.h"

#include "Audio.h"
#include "Camera.h"
#include "Projector4D.h"
#include "World.h"
#include "Body.h"
#include "HalfSpace.h"
#include "Mesh.h"

#include "MaterialManager.h"

#include <time.h>

#ifdef _DEBUG_DUMP_TELEMETRY
#include <fstream>
#endif

static int LoadContext(GLES2_Context* data)
{
#if SDL_VIDEO_DRIVER_UIKIT
#define __SDL_NOGETPROCADDR__
#elif SDL_VIDEO_DRIVER_ANDROID
#define __SDL_NOGETPROCADDR__
#elif SDL_VIDEO_DRIVER_PANDORA
#define __SDL_NOGETPROCADDR__
#endif

#if defined __SDL_NOGETPROCADDR__
#define SDL_PROC(ret,func,params) data->func=func;
#else
#define SDL_PROC(ret,func,params) \
    do { \
        data->func = (ret (APIENTRY *) params)SDL_GL_GetProcAddress(#func); \
        if ( ! data->func ) { \
            return SDL_SetError("Couldn't load GLES2 function %s: %s", #func, SDL_GetError()); \
        } \
    } while ( 0 );
#endif /* __SDL_NOGETPROCADDR__ */

#include "../src/render/opengles2/SDL_gles2funcs.h"
#undef SDL_PROC
	return 0;
}

#define SDL_IPHONE_MAX_GFORCE 5.0f
#define USE_FIXED_DELTA_TIME
//#define USE_IPHONEX_SETTINGS

#if defined(TARGET_OS_IPHONE)
const std::string g_resDir = "./";
#else
const std::string g_resDir = "../../";
#endif

GLES2_Context ctx;
Audio * rollSound = nullptr;

namespace
{
#if defined(TARGET_OS_IPHONE) || defined(USE_IPHONEX_SETTINGS)
	int resW = 375, resH = 812;
	float FoV = glm::radians(95.0f);
	bool useTopDownProjection = true;
#else
	int resW = 800, resH = 800;
	float FoV = glm::radians(60.0f);
	bool useTopDownProjection = false;
#endif

	Body bodies[16];
	int numBodies = 0;

	HalfSpace planes[16];
	int numPlanes = 0;
	
	// 200 times the dimensions of a Die Hard six sided die, see
	// https://www.dieharddice.com/dice-size-and-weight
	Body* die = nullptr;
	float dieWidth = 2.8f;
	float dieMass = 3.2f;
	int dimensions = 4;

	float timeStep = 1.0f / 60.0f;
	int iterations = 10;

	// Arbiter parameters
	const float contactDetectionEpsilon = 0.028f;		// 1/100 of the die dimensions
	const float velocityNormThreshold = 0.05f;			// Arbitrary...
	const float angularVelocityNormThreshold = 0.05f;	// Even more arbitrary...
	const float restitutionCoefficient = 0.3f;

	const glm::vec4 gravity(0.0f, -9.8f, 0.0f, 0.0f);
	const float shakeForceMultiplier = 500.0f;

	std::list<SDL_TouchFingerEvent> touchEventQueue;
	bool touchAndHoldEvent = false;

	int demoIndex = 1;

	Camera* pCamera = nullptr;
	float cameraPanningSensitivity = 0.5f;
	float cameraRotationSensitivity = 0.05f;
	bool cameraFollow = false;

	Projector4D* pProjector = nullptr;

	bool use4DBackfaceCulling = true;
	std::vector<glm::vec3> colors;

	World world(gravity, iterations);

	SDL_Window* window = nullptr;
	TTF_Font* fontMedium = nullptr;
	TTF_Font* fontLarge = nullptr;
	bool done = false;

	bool soundEnabled = true;
	SDL_AudioSpec targetSpec, wavSpec;
	SDL_Joystick *accelerometer;

	std::vector<glm::vec2> cellParams;

	unsigned int textureIds[8];

	MaterialManager* pMaterialManager;
}

void LoadGlobalTextures()
{
	ctx.glGenTextures(8, textureIds);

	for (unsigned int i = 0; i < 8; i++)
	{
		SDL_Surface *loadedImage = nullptr;
		
		loadedImage = SDL_LoadBMP((g_resDir + "textures/" + (char)(i + '1') + ".bmp").c_str()); // Temporary hardcode

		if (loadedImage)
		{
			ctx.glBindTexture(GL_TEXTURE_2D, textureIds[i]);
			ctx.glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, loadedImage->w, loadedImage->h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, loadedImage->pixels);

			ctx.glGenerateMipmap(GL_TEXTURE_2D);

			ctx.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			ctx.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			ctx.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			ctx.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			ctx.glFinish();

			SDL_FreeSurface(loadedImage);
		}
	}
}

void UnloadGlobalTextures()
{
	for (unsigned int i = 0; i < 8; i++)
	{
		ctx.glDeleteTextures(1, &textureIds[i]);
	}
}

void DrawString(unsigned int shader, TTF_Font* font, int x, int y, const char *string, bool centered = false)
{
	ctx.glEnable(GL_BLEND);
	ctx.glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_Color textColor = { 240, 240, 240, 255 }; // Interpret as BGRA
	SDL_Surface* text = TTF_RenderText_Blended(font, string, textColor);

	float w_2 = text->w / 2.0f;
	float h_2 = text->h / 2.0f;
	glm::vec3 offset((float)(x + (centered ? 0 : text->w / 2)), (float)y, 0.0f);
	std::vector<vertexData> vtxData(4);
	vtxData[0].position = offset + glm::vec3(w_2, h_2, 0.0f);
	vtxData[0].UV = glm::vec2(1.0f, 1.0f);
	vtxData[1].position = offset + glm::vec3(-w_2, h_2, 0.0f);
	vtxData[1].UV = glm::vec2(0.0f, 1.0f);
	vtxData[2].position = offset + glm::vec3(-w_2, -h_2, 0.0f);
	vtxData[2].UV = glm::vec2(0.0f, 0.0f);
	vtxData[3].position = offset + glm::vec3(w_2, -h_2, 0.0f);
	vtxData[3].UV = glm::vec2(1.0f, 0.0f);
	std::vector<unsigned int> idxData;
	idxData.push_back(0); idxData.push_back(1); idxData.push_back(2); idxData.push_back(3);

	unsigned int texId;
	ctx.glGenTextures(1, &texId);
	textureData texData;
	texData.id = texId;

	ctx.glBindTexture(GL_TEXTURE_2D, texId);
	ctx.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text->w, text->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, text->pixels);

	ctx.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	ctx.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	ctx.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	ctx.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	std::vector<textureData> texDataVec;
	texDataVec.push_back(texData);

	Mesh glyph(GL_TRIANGLE_FAN, &vtxData, &idxData, &texDataVec);
	glyph.draw(shader);

	SDL_FreeSurface(text);

	ctx.glDisable(GL_BLEND);
}

void DrawSphericalLabel(unsigned int shader, int x, int y, int w, int h, unsigned int i, bool centered = false)
{
	ctx.glEnable(GL_BLEND);
	ctx.glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE);

	float scale = cellParams[i].x;
	float w_2 = scale * w / 2.0f;
	float h_2 = scale * h / 2.0f;
	glm::vec3 offset((float)(x + (centered ? 0 : w_2)), (float)y, 0.0f);
	std::vector<vertexData> vtxData(4);
	vtxData[0].position = offset + glm::vec3( w_2,  h_2, 0.0f);
	vtxData[0].UV = glm::vec2(1.0f, 1.0f);
	vtxData[1].position = offset + glm::vec3(-w_2,  h_2, 0.0f);
	vtxData[1].UV = glm::vec2(0.0f, 1.0f);
	vtxData[2].position = offset + glm::vec3(-w_2, -h_2, 0.0f);
	vtxData[2].UV = glm::vec2(0.0f, 0.0f);
	vtxData[3].position = offset + glm::vec3( w_2, -h_2, 0.0f);
	vtxData[3].UV = glm::vec2(1.0f, 0.0f);
	std::vector<unsigned int> idxData;
	idxData.push_back(0); idxData.push_back(1); idxData.push_back(2); idxData.push_back(3);

	textureData texData;
	texData.id = textureIds[i];
	texData.isGlobalTexture = true;

	std::vector<textureData> texDataVec;
	texDataVec.push_back(texData);

	Mesh glyph(GL_TRIANGLE_FAN, &vtxData, &idxData, &texDataVec);
	glyph.draw(shader);

	ctx.glFinish();

	ctx.glDisable(GL_BLEND);
}

void PrintThrowResult(unsigned int shader, Body* body)
{
	if (glm::length(body->velocity) < 0.25)
	{
		if (dimensions == 3)
		{
			for (unsigned int i = 0; i < body->bodyFaces.size(); i++)
			{
				auto& face = body->bodyFaces[i];
				auto& bodyVertices = body->bodyVertices;

				auto& v1 = bodyVertices[face[0]];
				auto& v2 = bodyVertices[face[1]];
				auto& v3 = bodyVertices[face[2]];

				auto normal = glm::normalize(body->rotation * glm::vec4(glm::cross(glm::vec3(v2 - v1), glm::vec3(v3 - v2)), 0.0f));
				auto up = glm::normalize(-gravity);
				if (glm::dot(normal, up) > 0.99f)
				{
					DrawString(shader, fontLarge, resW / 2, 7 * resH / 8, std::to_string(i + 1).c_str(), true);
				}
			}
		}

		if (dimensions == 4)
		{
			for (unsigned int i = 0; i < body->bodyCells.size(); i++)
			{
				auto& cell = body->bodyCells[i];
				auto& bodyVertices = body->bodyVertices;

				auto& v1 = bodyVertices[cell.first[0]];
				auto& v2 = bodyVertices[cell.first[1]];
				auto& v3 = bodyVertices[cell.first[2]];
				auto& v7 = bodyVertices[cell.second[2]];

				auto normal = glm::normalize(body->rotation * glm::cross(v2 - v1, v3 - v2, v3 - v7));
				auto up = glm::normalize(-gravity);
				if (glm::dot(normal, up) > 0.99f)
				{
					DrawString(shader, fontLarge, resW / 2, 7 * resH / 8, std::to_string(i + 1).c_str(), true);
					DrawSphericalLabel(shader, resW / 2, (int)(1.15f * resH / 8), 128, 128, i, true);
				}
			}
		}
	}
}

void DrawBody(unsigned int shader, Body* body)
{
	ctx.glUniform4f(ctx.glGetUniformLocation(shader, "diffuseColor"), 0.8f, 0.8f, 0.9f, 0.1f);

	vertexData v1, v2, v3, v4;
	std::vector<vertexData> vtxData;
	std::vector<unsigned int> idxData;
	
	for (unsigned int i = 0; i < body->bodyEdges.size(); ++i)
	{
		auto& edge = body->bodyEdges[i];
		v1.position = pProjector->Project(body->rotation * body->bodyVertices[edge.first] + body->position);
		vtxData.push_back(v1); idxData.push_back(2*i);
		v2.position = pProjector->Project(body->rotation * body->bodyVertices[edge.second] + body->position);
		vtxData.push_back(v2); idxData.push_back(2*i+1);
	}

	Mesh outline(GL_LINES, &vtxData, &idxData);
	outline.draw(shader);

	ctx.glEnable(GL_BLEND);
	ctx.glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE);

	vtxData.clear(); idxData.clear();
	for (int i = 0, idx = 0; i < (int)body->bodyFaces.size(); ++i)
	{
		auto& bodyVertices = body->bodyVertices;
		auto face = body->bodyFaces[i];

		// Backface Culling:
		if (dimensions == 4 && use4DBackfaceCulling)
		{
			bool drawFace = true;
			auto dir = bodyVertices[face[0]] + bodyVertices[face[1]] + bodyVertices[face[2]] + bodyVertices[face[3]];
			for (int j = 0; j < 4; j++)
			{
				if (glm::abs(dir[j]) > body->width)
				{
					glm::vec4 normal(0.0f);
					normal[j] = 1.0f * glm::sign(dir[j]);
					auto viewDir = pProjector->GetViewDirection();
					if (glm::dot(body->rotation * normal, -viewDir) < -0.01f)
					{
						drawFace = false;
					}
				}
			}

			if (!drawFace)
			{
				continue;
			}
		}

		v1.position = pProjector->Project(body->rotation * bodyVertices[face[0]] + body->position);
		vtxData.push_back(v1);
		v2.position = pProjector->Project(body->rotation * bodyVertices[face[1]] + body->position);
		vtxData.push_back(v2);
		v3.position = pProjector->Project(body->rotation * bodyVertices[face[2]] + body->position);
		vtxData.push_back(v3);
		v4.position = pProjector->Project(body->rotation * bodyVertices[face[3]] + body->position);
		vtxData.push_back(v4);

		idxData.push_back(4 * idx); idxData.push_back(4 * idx + 1); idxData.push_back(4 * idx + 2);
		idxData.push_back(4 * idx); idxData.push_back(4 * idx + 2); idxData.push_back(4 * idx + 3);
		idx++;
	}

	if (!vtxData.empty())
	{
		auto& color = colors[0];
		ctx.glUniform4f(ctx.glGetUniformLocation(shader, "diffuseColor"), color.r, color.g, color.b, 0.20f);

		Mesh faces(GL_TRIANGLES, &vtxData, &idxData);
		faces.draw(shader);
	}

	ctx.glDisable(GL_BLEND);
}

void DrawBodyCells(unsigned int shader, Body* body)
{
	ctx.glUniform4f(ctx.glGetUniformLocation(shader, "diffuseColor"), 0.9f, 0.9f, 1.0f, 1.0f);

	ctx.glEnable(GL_BLEND);
	ctx.glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE);

	if (dimensions == 3 || dimensions == 4)
	{
		auto ViewMatrix = pCamera->getViewMatrix();
		float nearClipDistance = pCamera->getNearClipDistance();

		for (unsigned int i = 0; i < body->bodyCells.size(); i++)
		{
			auto& cell = body->bodyCells[i];
			auto& bodyVertices = body->bodyVertices;

			auto& v1 = bodyVertices[cell.first[0]];
			auto& v2 = bodyVertices[cell.first[1]];
			auto& v3 = bodyVertices[cell.first[2]];
			auto& v4 = bodyVertices[cell.first[3]];
			auto& v5 = bodyVertices[cell.second[0]];
			auto& v6 = bodyVertices[cell.second[1]];
			auto& v7 = bodyVertices[cell.second[2]];
			auto& v8 = bodyVertices[cell.second[3]];
			auto cellPosOffset = 0.125f * (v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8);
			auto normal = glm::normalize(glm::cross(v2 - v1, v3 - v2, v3 - v7));

			// Backface Culling:
			if (dimensions == 4 && use4DBackfaceCulling)
			{
				auto viewDir = pProjector->GetViewDirection();
				auto viewDirComponent = glm::dot(body->rotation * normal, -viewDir);
				if (viewDirComponent <= 0.01f)
				{
					continue;
				}
			}

			glm::vec4 farPoint(FLT_MAX), nearPoint(-FLT_MAX);
			for (unsigned int j = 0; j < bodyVertices.size(); j++)
			{
				auto worldSpacePosition = body->rotation * bodyVertices[j] + body->position;
				auto prjectedPosition = glm::vec3(pProjector->Project(worldSpacePosition));
				auto viewSpacePosition = *ViewMatrix * glm::vec4(prjectedPosition, 1.0f);
				farPoint = glm::min(viewSpacePosition, farPoint);
				nearPoint = glm::max(viewSpacePosition, nearPoint);
			}

			if (-farPoint.z > nearClipDistance)
			{
				auto ViewMatrixInv = glm::inverse(*ViewMatrix);
				ctx.glUniformMatrix4fv(ctx.glGetUniformLocation(shader, "View_Matrix_Inv"), 1, false, glm::value_ptr(ViewMatrixInv));

				auto ProjectorFrameInv = glm::transpose(pProjector->GetFrameMatrix());
				ctx.glUniformMatrix4fv(ctx.glGetUniformLocation(shader, "ProjectorFrame_Matrix_Inv"), 1, false, glm::value_ptr(ProjectorFrameInv));

				// Draw settings:
				float outlineHalfWidth = 1.0f * 0.5f * body->width;
				ctx.glUniform1f(ctx.glGetUniformLocation(shader, "outlineHalfWidth"), outlineHalfWidth);

				ctx.glUniform1i(ctx.glGetUniformLocation(shader, "cellIndex"), i);

				ctx.glUniform2fv(ctx.glGetUniformLocation(shader, "cellParams"), cellParams.size(), (const float*)(&cellParams[0]));

				// 4D World-space:
				glm::mat4 cellTangents(0.0f);
				auto startCorner = body->rotation * bodyVertices[cell.first[0]];
				cellTangents[0] = (startCorner - body->rotation * bodyVertices[cell.first[1]]) / body->width;
				cellTangents[1] = (startCorner - body->rotation * bodyVertices[cell.first[3]]) / body->width;
				cellTangents[2] = (startCorner - body->rotation * bodyVertices[cell.second[0]]) / body->width;
				auto CellTangentsInv = glm::transpose(cellTangents);
				ctx.glUniformMatrix4fv(ctx.glGetUniformLocation(shader, "CellTangent_Matrix_Inv"), 1, false, glm::value_ptr(CellTangentsInv));

				auto cellPos = body->rotation * cellPosOffset + body->position;
				ctx.glUniform4f(ctx.glGetUniformLocation(shader, "cellPosition"), cellPos.x, cellPos.y, cellPos.z, cellPos.w);
				auto cellNormal = body->rotation * normal;
				ctx.glUniform4f(ctx.glGetUniformLocation(shader, "cellNormal"), cellNormal.x, cellNormal.y, cellNormal.z, cellNormal.w);
				auto CellRotationInv = glm::transpose(body->rotation);
				ctx.glUniformMatrix4fv(ctx.glGetUniformLocation(shader, "CellRotation_Matrix_Inv"), 1, false, glm::value_ptr(CellRotationInv));

				// Projected into 3D View-space:
				vertexData v1, v2, v3, v4;
				std::vector<vertexData> vtxData;
				std::vector<unsigned int> idxData;

				float rayMarchingStep = -0.015625f;
				float renderProxyDepth = glm::floor(farPoint.z / rayMarchingStep + 0.5f) * rayMarchingStep;
				glm::vec2 bottomLeftPoint(FLT_MAX), topRightPoint(-FLT_MAX);
				for (unsigned int j = 0; j < bodyVertices.size(); j++)
				{
					auto worldSpacePosition = body->rotation * bodyVertices[j] + body->position;
					auto prjectedPosition = glm::vec3(pProjector->Project(worldSpacePosition));
					auto viewSpacePosition = *ViewMatrix * glm::vec4(prjectedPosition, 1.0f);
					auto viewSpaceProjectedPosition = viewSpacePosition * renderProxyDepth / viewSpacePosition.z;
					bottomLeftPoint = glm::min(glm::vec2(viewSpaceProjectedPosition), bottomLeftPoint);
					topRightPoint = glm::max(glm::vec2(viewSpaceProjectedPosition), topRightPoint);
				}

				v1.position = glm::vec3(topRightPoint.x, topRightPoint.y, renderProxyDepth);
				vtxData.push_back(v1);
				v2.position = glm::vec3(bottomLeftPoint.x, topRightPoint.y, renderProxyDepth);
				vtxData.push_back(v2);
				v3.position = glm::vec3(bottomLeftPoint.x, bottomLeftPoint.y, renderProxyDepth);
				vtxData.push_back(v3);
				v4.position = glm::vec3(topRightPoint.x, bottomLeftPoint.y, renderProxyDepth);
				vtxData.push_back(v4);

				idxData.push_back(0); idxData.push_back(1); idxData.push_back(2);
				idxData.push_back(0); idxData.push_back(2); idxData.push_back(3);

				float rayMarchingStart = -nearPoint.z > nearClipDistance ? nearPoint.z : -nearClipDistance;
				rayMarchingStart = glm::floor(rayMarchingStart / rayMarchingStep + 0.5f) * rayMarchingStep;
				float rayMarchingEnd = renderProxyDepth;

				ctx.glUniform1f(ctx.glGetUniformLocation(shader, "rayMarchingStart"), rayMarchingStart);
				ctx.glUniform1f(ctx.glGetUniformLocation(shader, "rayMarchingEnd"), rayMarchingEnd);
				ctx.glUniform1f(ctx.glGetUniformLocation(shader, "rayMarchingStep"), rayMarchingStep);

				Mesh volumeRendererProxy(GL_TRIANGLES, &vtxData, &idxData);
				volumeRendererProxy.draw(shader);
			}
		}
	}

	ctx.glDisable(GL_BLEND);
}

void DrawPlane(HalfSpace* plane)
{
	auto normal4D = pProjector->Project(plane->normal);
	if (glm::abs(normal4D.w) > 0.0f)
	{
		// Skip planes oriented away from the projected volume
		return;
	}

	auto pos4D = pProjector->Project(plane->position);
	glm::vec3 pos = pos4D;
	float h = 10.0f;

	glm::vec3 normal = normal4D;
	auto proj = glm::dot(normal, glm::vec3(0.0f, 1.0f, 0.0f));
	auto angle = glm::acos(proj);
	auto axis = glm::abs(proj) < 0.9999f ? glm::normalize(glm::cross(normal, glm::vec3(0.0f, 1.0f, 0.0f))) : normal;
	auto R = glm::rotate(glm::mat4(1.0f), angle, axis);

	glm::vec3 v1 = pos + glm::vec3(R * glm::vec4(-h, 0.0f, 0.0f, 0.0f));
	glm::vec3 v2 = pos + glm::vec3(R * glm::vec4(h, 0.0f, 0.0f, 0.0f));

	auto Rlocal = glm::rotate(glm::mat4(1.0f), glm::half_pi<float>(), normal);
	glm::vec3 v3 = pos + glm::vec3(Rlocal * glm::vec4(v1 - pos, 0.0f));
	glm::vec3 v4 = pos + glm::vec3(Rlocal * glm::vec4(v2 - pos, 0.0f));

	glm::vec3 v5 = v1 - 0.5f*(v4 - v3);
	glm::vec3 v6 = v1 + 0.5f*(v4 - v3);
	glm::vec3 v7 = v2 + 0.5f*(v4 - v3);
	glm::vec3 v8 = v2 - 0.5f*(v4 - v3);

	std::vector<vertexData> vtxData(8);
	vtxData[0].position = v1;
	vtxData[1].position = v2;
	vtxData[2].position = v3;
	vtxData[3].position = v4;
	vtxData[4].position = v5;
	vtxData[5].position = v6;
	vtxData[6].position = v7;
	vtxData[7].position = v8;
	std::vector<unsigned int> idxData;

	unsigned int programObject = pMaterialManager->GetShaderByMaterialName(std::string(""));
	ctx.glUniform4f(ctx.glGetUniformLocation(programObject, "diffuseColor"), 0.0f, 0.9f, 0.0f, 0.1f);

	ctx.glEnable(GL_BLEND);
	ctx.glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE);

	idxData.push_back(4);
	idxData.push_back(5);
	idxData.push_back(6);
	idxData.push_back(4);
	idxData.push_back(6);
	idxData.push_back(7);
	Mesh walls(GL_TRIANGLES, &vtxData, &idxData);
	walls.draw(programObject);

	ctx.glDisable(GL_BLEND);

	idxData.clear();
	idxData.push_back(4);
	idxData.push_back(5);
	idxData.push_back(5);
	idxData.push_back(6);
	idxData.push_back(6);
	idxData.push_back(7);
	idxData.push_back(7);
	idxData.push_back(4);
	Mesh edges(GL_LINES, &vtxData, &idxData);
	edges.draw(programObject);

	ctx.glUniform4f(ctx.glGetUniformLocation(programObject, "diffuseColor"), 0.3f, 0.3f, 0.3f, 1.0f);

	idxData.clear();
	idxData.push_back(0);
	idxData.push_back(1);
	idxData.push_back(2);
	idxData.push_back(3);
	Mesh centralLines(GL_LINES, &vtxData, &idxData);
	centralLines.draw(programObject);
}

void InitDemo(int index);

void ThrowDie()
{
#ifdef _DEBUG_DUMP_TELEMETRY
	std::ofstream("D:/dice_telemetry.txt", std::ios::trunc);
#endif

	InitDemo(demoIndex);

	if (!die)
	{
		die = bodies + numBodies;

		die->Set(dimensions, dieWidth, dieMass);

		// The effective coefficient of friction will be sqrt(die->friction)
		die->friction = 0.81f;

		world.Add(die);
		++numBodies;
	}

	static glm::vec4 startPos = glm::vec4(0.0f, 2.0f, 0.0f, 0.0f);
	static glm::mat4 startRotation = glm::rotate(glm::mat4(1.0f), 
										glm::acos(glm::dot(glm::normalize(glm::vec3(1.0f,1.0f,1.0f)), glm::vec3(1.0f,0.0f,0.0f))),
										glm::cross(glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)), glm::vec3(1.0f, 0.0f, 0.0f))
										);
	static glm::vec4 startVelocity = glm::vec4(5.0f, 5.0f, 0.0f, 0.0f);
	static glm::mat4 startAngularVelocityTensor = glm::mat4(0.0f, 0.0f, 0.0f, 10.0f, 0.0f, 0.0f);
	static bool dieParamsInitialized = false;
	if (World::reinitDieParams || !dieParamsInitialized)
	{
		if (dimensions == 2 || dimensions == 1)
		{
			startPos = glm::vec4(glm::linearRand(-10.0f + 2.0*dieWidth, 10.0f - 2.0*dieWidth), 15.0f, 0.0f, 0.0f);
			startRotation = glm::rotate(glm::mat4(1.0f), glm::linearRand(-1.5f, 1.5f), glm::vec3(0.0f,0.0f,1.0f));
			startVelocity = glm::vec4(glm::linearRand(-20.0f, 20.0f), glm::linearRand(-20.0f, 20.0f), 0.0f, 0.0f);
			startAngularVelocityTensor = glm::mat4(glm::linearRand(-2.0f, 2.0f), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			dieParamsInitialized = true;
		}

		if (dimensions == 3)
		{
			startPos = glm::vec4(glm::linearRand(-10.0f + 2.0*dieWidth, 10.0f - 2.0*dieWidth), 15.0f, glm::linearRand(-9.0f, 9.0f), 0.0f);
			startRotation = glm::mat4(1.0f);
			startVelocity = glm::vec4(glm::linearRand(-20.0f, 20.0f), glm::linearRand(-20.0f, 0.0f), glm::linearRand(-20.0f, 20.0f), 0.0f);
			startAngularVelocityTensor = glm::mat4(glm::linearRand(-2.0f, 2.0f), glm::linearRand(-2.0f, 2.0f), 0.0f, glm::linearRand(-2.0f, 2.0f), 0.0f, 0.0f);
			dieParamsInitialized = true;
		}

		if (dimensions == 4)
		{
			startPos = glm::vec4(glm::linearRand(-10.0f + 2.0*dieWidth, 10.0f - 2.0*dieWidth), 15.0f, glm::linearRand(-9.0f, 9.0f), glm::linearRand(-9.0f, 9.0f));
			startRotation = glm::mat4(1.0f);
			startVelocity = glm::vec4(glm::linearRand(-20.0f, 20.0f), glm::linearRand(-20.0f, 0.0f), glm::linearRand(-20.0f, 20.0f), glm::linearRand(-20.0f, 20.0f));
			startAngularVelocityTensor = glm::mat4(glm::linearRand(-2.0f, 2.0f), glm::linearRand(-2.0f, 2.0f), glm::linearRand(-2.0f, 2.0f), glm::linearRand(-2.0f, 2.0f), glm::linearRand(-2.0f, 2.0f), glm::linearRand(-2.0f, 2.0f));
			dieParamsInitialized = true;
		}
	}

	die->position = startPos;
	die->rotation = startRotation;
	die->velocity = startVelocity;
	die->angularVelocity = startAngularVelocityTensor;
}

// Box Walls
void Demo1(Body* b, HalfSpace* p)
{
	b->Set(dimensions, 20.0f, FLT_MAX);
	b->position = glm::vec4(0.0f, -0.5f * b->width, 0.0f, 0.0f);
	world.Add(b);
	++b; ++numBodies;

	b->Set(dimensions, 20.0f, FLT_MAX);
	b->position = glm::vec4(0.0f, 28.0f, 0.0f, 0.0f);
	world.Add(b);
	++b; ++numBodies;

	b->Set(dimensions, 20.0f, FLT_MAX);
	b->position = glm::vec4(-20.0f, 9.0f, 0.0f, 0.0f);
	world.Add(b);
	++b; ++numBodies;

	b->Set(dimensions, 20.0f, FLT_MAX);
	b->position= glm::vec4(20.0f, 9.0f, 0.0f, 0.0f);
	world.Add(b);
	++b; ++numBodies;

	GetMemories().Add(&world);
}

// Half-space Walls
void Demo2(Body* b, HalfSpace* p)
{
	p->Set(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	world.Add(p);
	++p; ++numPlanes;

	p->Set(glm::vec4(0.0f, 18.0f, 0.0f, 0.0f), glm::vec4(0.0f, -1.0f, 0.0f, 0.0f));
	world.Add(p);
	++p; ++numPlanes;

	p->Set(glm::vec4(-10.0f, 8.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	world.Add(p);
	++p; ++numPlanes;

	p->Set(glm::vec4(10.0f, 8.0f, 0.0f, 0.0f), glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
	world.Add(p);
	++p; ++numPlanes;

	p->Set(glm::vec4(0.0f, 8.0f, 10.0f, 0.0f), glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	world.Add(p);
	++p; ++numPlanes;

	p->Set(glm::vec4(0.0f, 8.0f, -10.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	world.Add(p);
	++p; ++numPlanes;

	p->Set(glm::vec4(0.0f, 8.0f, 0.0f, 10.0f), glm::vec4(0.0f, 0.0f, 0.0f, -1.0f));
	world.Add(p);
	++p; ++numPlanes;

	p->Set(glm::vec4(0.0f, 8.0f, 0.0f, -10.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	world.Add(p);
	++p; ++numPlanes;

	GetMemories().Add(&world);
}

void(*demos[])(Body* b, HalfSpace* p) = { Demo1, Demo2};
char* demoStrings[] = {
	"Demo 1: Box Walls",
	"Demo 2: Half-space Walls"};

void InitCamera()
{
	if (!pCamera)
	{
		pCamera = new Camera();
	}

	pCamera->resetCameraFrame();
	pCamera->setProjectionParams(FoV, resW / (float)resH);
	pCamera->addYawRotation(0.0f);
	pCamera->addPitchRotation(0.0f);
	if (useTopDownProjection)
	{
		pCamera->translateCamera(0.0f, 30.0f, 0.0f);
		pCamera->addPitchRotation(-glm::half_pi<float>());
		pCamera->setMovementUpDirection(glm::vec3(0.0f, 0.0f, -1.0f));
	}
	else
	{
		pCamera->translateCamera(0.0f, 10.0f, 30.0f);
		pCamera->setMovementUpDirection(glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (!pProjector)
	{
		pProjector = new Projector4D();
	}

	pProjector->setPrincipalViewDirection(useTopDownProjection ? Projector4D::E_PVD_NEGATIVE_Y : Projector4D::E_PVD_NEGATIVE_W);
	pProjector->resetProjectorFrame();
}

void Reshape(int width, int height)
{
	if (height == 0)
		height = 1;

	resW = width;
	resH = height;
	glViewport(0, 0, width, height);

	if (pCamera)
	{
		pCamera->setProjectionParams(FoV, resW / (float)resH);
	}
}

bool InitScene()
{
	bool successfullInit = true;

	//Load all Shaders
	pMaterialManager = MaterialManager::GetInstance();
#if defined(__APPLE__)
	successfullInit &= pMaterialManager->LoadShaders(std::string("shaders/"));
#else
    successfullInit &= pMaterialManager->LoadShaders(std::string("../../shaders/"));
#endif

	InitCamera();
	Reshape(resW, resH);

	LoadGlobalTextures();

	return successfullInit;
}

void InitDemo(int index)
{
	world.Clear();
	world.SetArbiterParameters(contactDetectionEpsilon, velocityNormThreshold, angularVelocityNormThreshold, restitutionCoefficient, dimensions <= 2 ? 2 : 3);
	numBodies = 0;
	numPlanes = 0;
	die = NULL;

	colors.clear();
	colors.push_back(glm::vec3(0.9f, 0.9f, 0.9f));

	float texScale = 0.8f;
	float voxelIntensity = 0.01f;
	cellParams.clear();
	cellParams.push_back(glm::vec2(0.33f, voxelIntensity));
	cellParams.push_back(glm::vec2(texScale, voxelIntensity));
	cellParams.push_back(glm::vec2(0.66f, voxelIntensity));
	cellParams.push_back(glm::vec2(texScale, voxelIntensity));
	cellParams.push_back(glm::vec2(texScale, voxelIntensity));
	cellParams.push_back(glm::vec2(texScale, voxelIntensity));
	cellParams.push_back(glm::vec2(texScale, voxelIntensity));
	cellParams.push_back(glm::vec2(texScale, voxelIntensity));

	demoIndex = index;
	demos[index](bodies, planes);
}

void DrawHelpMenus(unsigned int shader)
{
	DrawString(shader, fontMedium, 5, 10, "Keys: 1-2 Demos, Move: WSAD + ArrowKeys, +/- Change Dimension, Space to Launch the Die");

	char buffer[64];

	sprintf(buffer, "(H)euristic Starting %s", World::warmStarting ? "ON" : "OFF");
	DrawString(shader, fontMedium, 5, 30, buffer);

	sprintf(buffer, "(T)oggle Random Start %s", World::reinitDieParams ? "ON" : "OFF");
	DrawString(shader, fontMedium, 5, 50, buffer);

	sprintf(buffer, "(Tab) Use Top-down Projection Direction %s", useTopDownProjection ? "ON" : "OFF");
	DrawString(shader, fontMedium, 5, 70, buffer);

	sprintf(buffer, "(B)ackface Culling %s", use4DBackfaceCulling ? "ON" : "OFF");
	DrawString(shader, fontMedium, 5, 90, buffer);

	sprintf(buffer, "(C)amera Follow %s", cameraFollow ? "ON" : "OFF");
	DrawString(shader, fontMedium, 5, 110, buffer);

	sprintf(buffer, "(P)ause Audio %s", !soundEnabled ? "ON" : "OFF");
	DrawString(shader, fontMedium, 5, 130, buffer);
}

void SimulationLoop(float dt)
{
	ctx.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if defined(TARGET_OS_IPHONE)
	if (die && touchAndHoldEvent)
	{
		glm::vec4 shakeAcceleration(0.0f);
		shakeAcceleration.x = SDL_JoystickGetAxis(accelerometer, 0);
		shakeAcceleration.y = SDL_JoystickGetAxis(accelerometer, 1);
		shakeAcceleration.z = SDL_JoystickGetAxis(accelerometer, 2);
		shakeAcceleration = shakeForceMultiplier * 9.8f * shakeAcceleration * (SDL_IPHONE_MAX_GFORCE / INT16_MAX);
		//printf("Acceleration: %006.4f \t %006.4f \t %006.4f\n", shakeAcceleration.x, shakeAcceleration.y, shakeAcceleration.z);

		shakeAcceleration *= glm::step(0.1f, glm::abs(shakeAcceleration));
		float shakeImprecission = glm::min(glm::min(shakeAcceleration.x, shakeAcceleration.y), shakeAcceleration.z);
		glm::vec4 acceleration = shakeAcceleration.x * glm::vec4(pCamera->getMovementRightDirection(), 0.0f) +
								 (-shakeAcceleration.y) * glm::vec4(pCamera->getMovementUpDirection(), 0.0f) +
								 glm::vec4(0.0f, shakeImprecission, 0.0f, 0.0f) +
								 glm::vec4(0.0f, 0.0f, 0.0f, shakeAcceleration.z);
		die->force += die->mass * acceleration;
		shakeAcceleration = glm::vec4(0.0f);

		world.SetSimulationEnabled(true);
	}
#endif //TARGET_OS_IPHONE

	world.Simulate(dt);

	if (die && cameraFollow)
	{
		pCamera->setFollowTarget(glm::vec3(die->position.x, die->position.y, die->position.z));
	}
	else
	{
		pCamera->setFollowTarget(glm::vec3(0.f));
	}

	auto ProjMatrix = pCamera->getProjMatrix();
	auto ViewMatrix = pCamera->getViewMatrix();
	glm::mat4x4 ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	unsigned int programObject = pMaterialManager->GetShaderByMaterialName(std::string("Simple-material"));
	ctx.glUseProgram(programObject);

	GLint MM_Addr = ctx.glGetUniformLocation(programObject, "Model_Matrix");
	ctx.glUniformMatrix4fv(MM_Addr, 1, false, glm::value_ptr(ModelMatrix));

	GLint VM_Addr = ctx.glGetUniformLocation(programObject, "View_Matrix");
	ctx.glUniformMatrix4fv(VM_Addr, 1, false, glm::value_ptr(*ViewMatrix));

	GLint PM_Addr = ctx.glGetUniformLocation(programObject, "Proj_Matrix");
	ctx.glUniformMatrix4fv(PM_Addr, 1, false, glm::value_ptr(*ProjMatrix));

	for (int i = 0; i < numPlanes; ++i)
		DrawPlane(planes + i);

#if !defined(TARGET_OS_IPHONE) && !defined(USE_IPHONEX_SETTINGS)
	world.DebugDraw(programObject, pProjector);
#endif // TARGET_OS_IPHONE

	for (int i = 0; i < numBodies; ++i)
	{
		DrawBody(programObject, bodies + i);
	}

	programObject = pMaterialManager->GetShaderByMaterialName(std::string("Volumetric-material"));
	ctx.glUseProgram(programObject);

	PM_Addr = ctx.glGetUniformLocation(programObject, "Proj_Matrix");
	ctx.glUniformMatrix4fv(PM_Addr, 1, false, glm::value_ptr(*ProjMatrix));

	for (int i = 0; i < numBodies; ++i)
	{
		DrawBodyCells(programObject, bodies + i);
	}

	programObject = pMaterialManager->GetShaderByMaterialName(std::string("Texts-material"));
	ctx.glUseProgram(programObject);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	glm::mat4 ProjMatrix2D = glm::ortho(0.0f, (float)w, (float)h, 0.0f);

	GLint PM2D_Addr = ctx.glGetUniformLocation(programObject, "Proj_Matrix");
	ctx.glUniformMatrix4fv(PM2D_Addr, 1, false, glm::value_ptr(ProjMatrix2D));

	if (die)
	{
		PrintThrowResult(programObject, die);
	}

#if !defined(TARGET_OS_IPHONE) && !defined(USE_IPHONEX_SETTINGS)
	DrawHelpMenus(programObject);
#endif // TARGET_OS_IPHONE

	SDL_GL_SwapWindow(window);
}

void Keyboard(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym)
	{
	case 'w':
		pCamera->translateCamera(0.0f, 0.0f, -cameraPanningSensitivity);
		break;

	case 's':
		pCamera->translateCamera(0.0f, 0.0f, cameraPanningSensitivity);
		break;

	case 'a':
		pCamera->translateCamera(-cameraPanningSensitivity, 0.0f, 0.0f);
		break;

	case 'd':
		pCamera->translateCamera(cameraPanningSensitivity, 0.0f, 0.0f);
		break;

	default:
		break;
	}

	if (!key.repeat)
	{
		switch (key.keysym.sym)
		{
		case 27:
			done = true;
			break;

		case '1':
		case '2':
			InitDemo(key.keysym.sym - '1');
			break;

		case 'r':
			InitCamera();
			break;

		case 'c':
			cameraFollow = !cameraFollow;
			break;

		case 't':
			World::reinitDieParams = !World::reinitDieParams;
			break;

		case 'h':
			World::warmStarting = !World::warmStarting;
			break;

		case 127:
			World::isReplaying = true;
			GetMemories().Begining();
			break;

		case '\t':
			useTopDownProjection = !useTopDownProjection;
			InitCamera();
			break;

		case 'b':
			use4DBackfaceCulling = !use4DBackfaceCulling;
			break;

		case 'p':
			soundEnabled = !soundEnabled;
			if (soundEnabled)
			{
				unpauseAudio();
			}
			else
			{
				pauseAudio();
			}
			break;

		case ' ':
			ThrowDie();
			break;

		default:
			break;
		}
	}
}

void SpecialKeyboard(SDL_KeyboardEvent& key)
{
	switch (key.keysym.scancode)
	{
	case SDL_SCANCODE_LEFT:
		pCamera->addYawRotation(cameraRotationSensitivity);
		break;

	case SDL_SCANCODE_RIGHT:
		pCamera->addYawRotation(-cameraRotationSensitivity);
		break;

	case SDL_SCANCODE_UP:
		pCamera->addPitchRotation(cameraRotationSensitivity);
		break;

	case SDL_SCANCODE_DOWN:
		pCamera->addPitchRotation(-cameraRotationSensitivity);
		break;

	default:
		break;
	}

	if (!key.repeat)
	{
		switch (key.keysym.scancode)
		{
		case SDL_SCANCODE_KP_MINUS:
			dimensions = glm::max(1, dimensions - 1);
			InitDemo(demoIndex);
			break;

		case SDL_SCANCODE_KP_PLUS:
			dimensions = glm::min(4, dimensions + 1);
			InitDemo(demoIndex);
			break;

		case SDL_SCANCODE_END:
			if (World::isReplaying)
			{
				World::isReplaying = false;
				GetMemories().Commence();
			}
			else
			{
				World::isReplaying = true;
				GetMemories().Cease();
			}
			break;

		case SDL_SCANCODE_PAGEDOWN:
			World::isReplaying = true;
			GetMemories().End();
			break;

		case SDL_SCANCODE_INSERT:
			if (World::isReplaying)
			{
				GetMemories().Back();
			}
			break;

		case SDL_SCANCODE_PAGEUP:
			if (World::isReplaying)
			{
				GetMemories().Forward();
			}
			break;

		default:
			break;
		}
	}
}

void Touch(SDL_TouchFingerEvent& finger)
{
	switch (finger.type)
	{
	case SDL_FINGERDOWN:
	case SDL_FINGERUP:
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		touchEventQueue.push_back(finger);
		break;

	default:
		break;
	}
}

void ProcessTouchEventQueue()
{
	auto time = SDL_GetTicks();
	auto it = touchEventQueue.begin();
	while (it != touchEventQueue.end())
	{
		switch (it->type)
		{
		case SDL_FINGERDOWN:
		case SDL_MOUSEBUTTONDOWN:
			touchAndHoldEvent = true;
			break;

		case SDL_FINGERUP:
		case SDL_MOUSEBUTTONUP:
			touchAndHoldEvent = false;
			break;

		default:
			break;
		}

		if (it->timestamp < time - 500)
		{
			it = touchEventQueue.erase(it);
		}
		else
		{
			it++;
		}
	}

	// Double tap:
	if (touchEventQueue.size() > 3 && !touchAndHoldEvent)
	{
		ThrowDie();
		touchEventQueue.clear();
	}
}

void MainLoop()
{
	SDL_Event event;
	Uint32 curTime = 0, prevTime = SDL_GetTicks();
	static float dt = 0.0f;
	
	/* Check for events */
	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				Keyboard(event.key);
				SpecialKeyboard(event.key);
				break;
			case SDL_FINGERDOWN:
			case SDL_FINGERUP:
			#ifndef TARGET_OS_IPHONE
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			#endif
				Touch(event.tfinger);
				break;
			case SDL_KEYUP:
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					{
						Reshape(event.window.data1, event.window.data2);
						break;
					}
				}
				break;
			case SDL_QUIT:
				done = true;
				break;
			}
		}

		ProcessTouchEventQueue();

	#ifdef USE_FIXED_DELTA_TIME
		SimulationLoop(timeStep);
	#else	
		// Update dt:
		curTime = SDL_GetTicks();
 		dt += (curTime - prevTime) * 0.001f; // convert to seconds
		prevTime = curTime;

		int simIterations = 0;
		while (dt > timeStep && simIterations < 10) 
		{
			SimulationLoop(timeStep);
			dt -= timeStep;
			simIterations++;
		}

		dt = glm::mod(dt, timeStep);
	#endif // USE_FIXED_DELTA_TIME
	}
}

int main(int argc, char** argv)
{
#ifndef _DEBUG
	srand((unsigned int)::time(NULL));
#endif
	InitDemo(demoIndex);

	SDL_SetHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "1");
	SDL_SetHint(SDL_HINT_ANDROID_SEPARATE_MOUSE_AND_TOUCH, "1");
	SDL_SetHint("SDL_IOS_ORIENTATIONS", "Portrait");

	/* initialize SDL, specifically its video subsystem, i.e. graphics: */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) < 0) {
		printf("Could not initialize SDL. Exiting.\n");
		return 1;
	}
	else {
		SDL_Log("SDL is now initialized. Yay!\n");
	}

#if defined(TARGET_OS_IPHONE)
	/* obtain information from SDL about the current graphics display: */
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	resW = displayMode.w; resH = displayMode.h;
    
	// Enable Multisampling:
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
#endif

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);

	window = SDL_CreateWindow("Dice4D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resW, resH, SDL_WINDOW_OPENGL | 
#if defined(TARGET_OS_IPHONE)
		SDL_WINDOW_FULLSCREEN
#else
		SDL_WINDOW_RESIZABLE
#endif
	);
	if (!window) {
		SDL_Log("Could not initialize SDL Window. Exiting.\n");
		return 1;
	}

	auto context = SDL_GL_CreateContext(window);
	if (!context) {
		SDL_Log("Could not initialize OpenGL context. Exiting.\n");
		SDL_DestroyWindow(window);
		return 1;
	}

	if (LoadContext(&ctx) < 0) {
		SDL_Log("Could not load GLES2 functions\n");
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		return 0;
	}

	// Init fonts:
	TTF_Init();
	fontMedium = TTF_OpenFont((g_resDir + "fonts/Roboto-Regular.ttf").c_str(), 18);
	TTF_SetFontStyle(fontMedium, TTF_STYLE_NORMAL);
	fontLarge = TTF_OpenFont((g_resDir + "fonts/Audiowide-Regular.ttf").c_str(), 48);
	if (!fontMedium || !fontLarge)
	{
		SDL_Log("Font not found\n");
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		return 1;
	}
	else
	{
		SDL_Log("Fonts Ok\n");
	}

	// Init Audio:
	initAudio();
	if (!soundEnabled)
	{
		pauseAudio();
	}

	// Example for playing background music, if needed:
	//playMusic("../../sounds/<background_music_file>", SDL_MIX_MAXVOLUME);

	const std::string soundFilePath = g_resDir + "sounds/roll.wav";
	rollSound = createAudio(soundFilePath.c_str(), 0, SDL_MIX_MAXVOLUME);

	// Init accelerometer:
	SDL_Log("Available joysticks: %d\n", SDL_NumJoysticks());
	accelerometer = SDL_JoystickOpen(0);
	if (accelerometer == NULL) {

		SDL_Log("Could not open joystick (accelerometer)");

	}

	InitScene();

	SDL_PumpEvents();
	MainLoop();

	UnloadGlobalTextures();

	TTF_Quit();
	freeAudio(rollSound);
	SDL_JoystickClose(accelerometer);
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
