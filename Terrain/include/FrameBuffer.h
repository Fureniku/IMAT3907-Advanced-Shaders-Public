#pragma once
#include <../../Dependancies/include/glad/glad.h>
#include <../../Dependancies/include/glm/glm.hpp>
#include "Properties.h"

class FrameBuffer {
private:
	glm::vec2 m_screenSize;

	unsigned int reflectionFrameBuffer;
	unsigned int reflectionTexture;
	
	unsigned int refractionFrameBuffer;
	unsigned int refractionTexture;

	unsigned int refractionDepthBuffer;
	unsigned int refractionDepthTexture;

	unsigned int fboShadowMap;
	unsigned int shadowAttachment;
	
public:
	FrameBuffer() {
		m_screenSize = glm::vec2(Properties::SCR_WIDTH, Properties::SCR_HEIGHT);
	}
	
	void setFBOWaterReflection();
	void setFBOWaterRefraction();
	void setFBOWaterDepth();
	void setFBOShadowMap();
	
	unsigned int getFBOWaterReflection() { return reflectionFrameBuffer; }
	unsigned int getFBOWaterRefraction() { return refractionFrameBuffer; }
	unsigned int getFBORefractionDepth() { return refractionDepthBuffer; }
	unsigned int getFboShadowMap() { return fboShadowMap; }
	
	unsigned int getWaterReflectionAttachment() { return reflectionTexture; }
	unsigned int getWaterRefractionAttachment() { return refractionTexture; }
	unsigned int getWaterRefractionDepthAttachment() { return refractionDepthTexture; }
	unsigned int getShadowAttachment() { return shadowAttachment; }
	

};