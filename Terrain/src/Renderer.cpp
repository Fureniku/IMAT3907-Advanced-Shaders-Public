#include "Renderer.h"

#include "Properties.h"

void Renderer::registerLights() {
	pointLights.emplace_back(glm::vec3(0.0, 100.0, 0.0), glm::vec3(0.0, 500.0, 0.0)); //green
	pointLights.emplace_back(glm::vec3(250.0, 15.0, 250.0), glm::vec3(0.0, 0.0, 500.0)); //blue
	pointLights.emplace_back(glm::vec3(0.0, 15.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 5.0, true); //white
}

void Renderer::setLightingUniforms(Shader& shader) {
	for (int i = 0; i < pointLights.size(); i++) {
		pointLights.at(i).setUniforms(i, shader);
	}
}

void Renderer::renderDebugLights(Camera camera) {
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(Properties::SCR_WIDTH) / static_cast<float>(Properties::SCR_HEIGHT), 0.1f, 1200.0f);
	glm::mat4 view = camera.GetViewMatrix();
	
	plainShader.use();
	plainShader.setMat4("projection", projection);
	plainShader.setMat4("view", view);
	

	for (int i = 0; i < pointLights.size(); i++) {
		pointLights.at(i).draw(plainShader);
	}
}

void Renderer::renderTerrain(Shader& shader) {
	terrain.draw(shader);
}

void Renderer::updateComputeShader(Camera& camera) {
	computeShader.use();
	computeShader.setVec3("seedPos", camera.Position);
	computeShader.setFloat("screenW", Properties::SCR_WIDTH);
	computeShader.setBool("algorithm", Properties::toggleNoiseAlgorithm);
	computeShader.createTexture(texMan);
	computeShader.bindTexture(0);
	computeShader.dispatch();
	computeShader.wait();
}

void Renderer::renderWater(Camera& camera, FrameBuffer& frameBuffer, float deltaTime) {
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(Properties::SCR_WIDTH) / static_cast<float>(Properties::SCR_HEIGHT), Properties::VIEW_NEAR, Properties::VIEW_FAR);
	glm::mat4 model = glm::mat4(1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	water.incrementMoveFactor(deltaTime);
	
	waterShader.use();
	waterShader.setMVP(model, view, projection);

	water.getDuDvTexture().activateTexture(waterShader);
	water.getDuDvNormalTexture().activateTexture(waterShader);

	waterShader.setFloat("screenW", Properties::SCR_WIDTH);
	waterShader.setFloat("screenH", Properties::SCR_HEIGHT);
	waterShader.setFloat("nearPlane", Properties::VIEW_NEAR);
	waterShader.setFloat("farPLane", Properties::VIEW_FAR);
	waterShader.setFloat("moveFactor", water.getMoveFactor());
	waterShader.setFloat("refractivity", 3);
	waterShader.setVec3("viewPos", camera.Position);

	waterShader.setVec3("dLightCol", dLightCol);
	waterShader.setVec3("dLightDir", dLightDir);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameBuffer.getWaterReflectionAttachment());
	waterShader.setInt("reflection", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, frameBuffer.getWaterRefractionAttachment());
	waterShader.setInt("refraction", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, frameBuffer.getWaterRefractionDepthAttachment());
	waterShader.setInt("depthMap", 2);
	water.draw(waterShader);

	glDisable(GL_BLEND);

	camUnderWater = camera.Position.y < 1; //Set in here so it doesn't affect the reflection cam
}

void Renderer::renderSkybox(Camera& camera) {
	glDepthFunc(GL_LEQUAL);
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(Properties::SCR_WIDTH) / static_cast<float>(Properties::SCR_HEIGHT), Properties::VIEW_NEAR, Properties::VIEW_FAR);
	skyboxShader.use();
	
	skyboxShader.setInt("skybox",  15);
	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);
	skybox.draw(skyboxShader);
	glDepthFunc(GL_LESS);
}


void Renderer::render(Camera& camera, glm::vec4 clipPlane) {
	renderSkybox(camera);
	skyboxShader.use();
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(Properties::SCR_WIDTH) / static_cast<float>(Properties::SCR_HEIGHT), Properties::VIEW_NEAR, Properties::VIEW_FAR);
	glm::mat4 model = glm::mat4(1.0f);

    terrainShader.use();

	heightMap.activateTexture(terrainShader);
	textureRegistry.activateAllTextures(terrainShader);
	
	terrainShader.setInt("numPointLights", pointLights.size());
	terrainShader.setInt("numSpotLights", spotLights.size());
	
	setLightingUniforms(terrainShader);

	float scale = 100;
	if (Properties::heightGenerationMethod == 0 && !Properties::toggleNoiseAlgorithm) {
		scale = 300;
	}

	terrainShader.setMVP(model, view, projection);
	terrainShader.setVec3("viewPos", camera.Position);
	terrainShader.setInt("scale", scale);
	terrainShader.setInt("heightGenerationMethod", Properties::heightGenerationMethod);
	terrainShader.setVec3("dLightCol", dLightCol);
	terrainShader.setVec3("dLightDir", dLightDir);
	terrainShader.setFloat("shine", 128);
	terrainShader.setFloat("specStr", 0.4);
	terrainShader.setVec3("sky", glm::vec3(Properties::RED, Properties::GREEN, Properties::BLUE));
	terrainShader.setBool("useHeightTexture", Properties::useHeightTexture);
	terrainShader.setBool("flatShading", Properties::useFlatShading);
	terrainShader.setBool("fog", Properties::fogEnabled);
	terrainShader.setBool("algorithm", Properties::toggleNoiseAlgorithm);
	terrainShader.setBool("camUnderWater", camUnderWater);
	terrainShader.setVec4("plane", clipPlane);
	
	renderTerrain(terrainShader);
	renderDebugLights(camera);

	checkForTerrainUpdate(camera);
	terrainShader.use(); //re-enable in case updating terrain updated compute shaders.
	
	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, computeShader.getTexture());
	terrainShader.setInt("computeHeight", 14);
	
	modelShader.use();
	modelShader.setMat4("projection", projection);
	modelShader.setMat4("view", view);
	modelShader.setMat4("model", model);

	if (Properties::debugPlane) {
		debugPlaneShader.use();
		debugPlaneShader.setMat4("projection", projection);
		debugPlaneShader.setMat4("view", view);
		debugPlaneShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE14);
		glBindTexture(GL_TEXTURE_2D, computeShader.getTexture());
		debugPlaneShader.setInt("computeHeight", 14);
		debugPlane.draw();
	}
}

void Renderer::checkForTerrainUpdate(Camera& camera) {
	if (Properties::heightGenerationMethod != Properties::HEIGHTGEN_HEIGHTMAP) {
		glm::vec3 cam = camera.Position;
		int range = 50;
		if (cam.x < lastPos.x-range || cam.x > lastPos.x+range || cam.z < lastPos.z-range || cam.z > lastPos.z+range) {
			updateTerrain(camera);
		}
	}
}

void Renderer::updateTerrain(Camera& camera) {
	updateComputeShader(camera);
	terrain.setVAO();
	water.setVAO();
	terrain.updatePos(glm::vec2(camera.Position.x - terrain.getWorldSize()/2, camera.Position.z - terrain.getWorldSize()/2));
	water.updatePos(glm::vec2(camera.Position.x, camera.Position.z));
	lastPos = camera.Position;
	
}