#include "particles.h"
#include "random.h"

GLuint ParticleCluster::renderProgram;
GLuint ParticleCluster::computeProgram;

void ParticleCluster::loadPrograms() {
	//geometry shader turns points into lines based on velocity
	renderProgram = loadGeometryShader(
		"shaders/particleVert.shader",
		"shaders/particleGeom.shader",
		"shaders/particleFrag.shader"
	);
	//compute shader makes changes in velocity and moves the particles
	computeProgram = loadComputeShader("shaders/particleCompute.shader");
}

ParticleCluster::ParticleCluster(int n) {
	origin = vec2(0.0f);
	g = .001f;
	particleCount = n;
	int vertexArrayStride = 4 * sizeof(float);
	std::vector<vec2> bufferdata;
	glCreateBuffers(1, &vertexBuffer0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer0);
	for (int i = 0; i < n; i++) {
		//initialize points randomly inside unit circle
		bufferdata.push_back(rng::randf() * rng::randvec2());
		//initialize velocities to 0
		bufferdata.push_back(vec2(.0));
	}
	glBufferData(GL_ARRAY_BUFFER, n * vertexArrayStride, &bufferdata[0], GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	for (int i = 0; i < 2; i++) {
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(
			i,								//position
			2,								//size
			GL_FLOAT,						//type
			GL_FALSE,						//normalize?
			vertexArrayStride,				//stride (in bytes)
			(void*)(2 * i * sizeof(float))	//offset
		);
	}

	glGenBuffers(1, &vertexBuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer1);
	glBufferData(GL_ARRAY_BUFFER, n * vertexArrayStride, NULL, GL_DYNAMIC_DRAW);
}

void ParticleCluster::render() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexArray);
	glBindVertexArray(vertexArray);
	glUseProgram(renderProgram);
	//points will be made into lines by geometry shader
	glDrawArrays(GL_POINTS, 0, particleCount);
}

void ParticleCluster::compute() {
	//binding 0 is source and binding 1 is target
	setUniforms();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertexBuffer0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vertexBuffer1);
	glUseProgram(computeProgram);
	glDispatchCompute(particleCount, 1, 1);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	//swapping the buffers
	GLuint temp = vertexBuffer1;
	vertexBuffer1 = vertexBuffer0;
	vertexBuffer0 = temp;
}

void ParticleCluster::printBuffer() {
	//prints entire vertex buffer for debugging
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer0);
	vector<float> arr(4 * particleCount);
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(float) * particleCount, &arr[0]);
	for (int i = 0; i < particleCount * 4; i++) {
		if (i % 4 == 0) {
			std::cout << '\n';
		}
		std::cout << arr[i] << ", ";
	}
}

void ParticleCluster::toggleGravity() {
	g *= -1;
}

void ParticleCluster::setUniforms() {
	glUseProgram(computeProgram);
	GLuint l_o = glGetUniformLocation(computeProgram, "origin");
	glUniform2fv(l_o, 1, &origin[0]);
	GLuint l_g = glGetUniformLocation(computeProgram, "G");
	glUniform1f(l_g, g);
}

ParticleCluster::~ParticleCluster() {
	glDeleteBuffers(1, &vertexBuffer0);
	glDeleteBuffers(1, &vertexBuffer1);
	glDeleteVertexArrays(1, &vertexArray);
}