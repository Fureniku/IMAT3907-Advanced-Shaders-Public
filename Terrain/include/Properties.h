#pragma once

class Properties {

public:
	static constexpr unsigned int SCR_WIDTH = 2460;
	static constexpr unsigned int SCR_HEIGHT = 1340;
	static constexpr float VIEW_NEAR = 0.1f;
	static constexpr float VIEW_FAR = 1200.f;

	static constexpr float RED = 92.0f/255.0f;
	static constexpr float GREEN = 142.0f/255.0f;
	static constexpr float BLUE = 179.0f/255.0f;

	static constexpr int HEIGHTGEN_COMPUTE = 0;
	static constexpr int HEIGHTGEN_EVALUATION = 1;
	static constexpr int HEIGHTGEN_HEIGHTMAP = 2;
	
	static bool wireframe;
	static bool useHeightTexture;
	static bool useFlatShading;
	static bool fogEnabled;
	static bool debugPlane;
	static bool toggleNoiseAlgorithm; //true for conor provided one, false for github one

	static int heightGenerationMethod; 
};
