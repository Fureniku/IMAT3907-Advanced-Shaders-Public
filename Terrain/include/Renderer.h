#pragma once
#include "lights/PointLight.h"
#include "lights/SpotLight.h"
#include "Camera.h"
#include "shapes/Terrain.h"
#include "glm/glm.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ComputeShader.h"
#include "FrameBuffer.h"
#include "Model.h"
#include "shapes/Water.h"
#include "shapes/DebugPlane.h"
#include "shapes/Skybox.h"
#include "texture/TextureManager.h"
#include "texture/TextureRegistry.h"

class Renderer {
private:
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
	Terrain terrain;

	Shader plainShader;
	Shader terrainShader;
	Shader waterShader;
	Shader modelShader;
	Shader debugPlaneShader;
	Shader skyboxShader;

	ComputeShader computeShader;
	Water water;
	DebugPlane debugPlane;

	bool camUnderWater = false;

	TextureManager* texMan = new TextureManager();
	TextureRegistry textureRegistry;

	Texture heightMap;

	Skybox skybox;

	glm::vec3 lastPos;

	glm::vec3 dLightCol = glm::vec3(1.0, 1.0, 1.0);
	glm::vec3 dLightDir = glm::vec3(0.1, -1.0, 0.0);
	
public:
	Renderer() {
		plainShader = Shader("plain", None);
		terrainShader = Shader("terrain", Geometry | Control | Evaluation);
		waterShader = Shader("water", None);
		modelShader = Shader("model", None);
		debugPlaneShader = Shader("debugPlane", None);
		skyboxShader = Shader("skybox", None);
		
		terrain = Terrain(200, 10, glm::vec2(0,0));
		computeShader = ComputeShader("computeImage.comp", glm::uvec2(64, 64));
		water = Water(0, glm::vec3(0, -1, 0));
		debugPlane = DebugPlane(120);

		heightMap = Texture("heightMap", "..\\Resources\\heightMap.jpg", 13);
	}
	
	void registerLights();
	void renderTerrain(Shader& shader);
	void setLightingUniforms(Shader& shader);
	void renderDebugLights(Camera camera);
	void updateComputeShader(Camera& camera);
	void renderWater(Camera& camera, FrameBuffer& frameBuffer, float deltaTime);
	void render(Camera& camera, glm::vec4 clipPlane);
	void renderSkybox(Camera& camera);
	void checkForTerrainUpdate(Camera& camera);
	void updateTerrain(Camera& camera);

	Water getWater() { return water; }
};
