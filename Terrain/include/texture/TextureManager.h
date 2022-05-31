#pragma once
#include "stb_image.h"

#include <../../Dependancies/include/glad/glad.h>
#include <../../Dependancies/include/glm/glm.hpp>
#include <../../Dependancies/include/glm/gtc/matrix_transform.hpp>>

 class TextureManager {
 private:
	 unsigned int VAO, VBO;
 public:
	 TextureManager() = default;
	 unsigned int createTexture(int width, int height);
	 void drawTexture(unsigned int output_img);
	 unsigned int loadTexture(char const* path);
 };
