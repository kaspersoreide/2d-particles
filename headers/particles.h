#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cstdlib>
#include <vector>
#include "loadshaders.h"

using namespace glm;
using namespace std;

class ParticleCluster {
public:
	ParticleCluster(int n);
	~ParticleCluster();
	static void loadPrograms();
	void setUniforms();
	void setOrigin(float x, float y) { origin = vec2(x, y); }
	void printBuffer();
	void toggleGravity();
	void render();
	void compute();
protected:
	GLuint vertexBuffer0, vertexBuffer1, vertexArray;
	static GLuint renderProgram, computeProgram;
	int particleCount;
	vec2 origin;
	float g;
};