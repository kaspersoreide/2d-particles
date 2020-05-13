#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <iostream>
#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include "particles.h"
#include "random.h"

using namespace std;
using namespace glm;

//resolution of window in pixels
const int RESX = 720;
const int RESY = 720;

GLFWwindow* window;
bool closed = false;
ParticleCluster* particles;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_SPACE) {
		if (action == GLFW_PRESS) particles->toggleGravity();
	}
	if (key == GLFW_KEY_ESCAPE) closed = true;
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
	float x = float(xpos);
	float y = float(ypos);

	particles->setOrigin(2.f * x / RESX - 1.f, -2.f * y / RESY + 1.f);
}

int init() {
	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(RESX, RESY, "Test", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	// Initialize GLEW
	//glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	//don't need depth test for 2d rendering
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//width of the particles
	glLineWidth(3.f);

	int error = glGetError();

	if (error != GL_NO_ERROR) {
		cout << "OpenGL error: " << error << "\n";
	}

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorCallback);

	return 0;
}

int main(void) {
	if (init() == -1) return -1;
	ParticleCluster::loadPrograms();
	//argument is number of particles
	particles = new ParticleCluster(2000);
	while (!glfwWindowShouldClose(window) && !closed) {
		glClear(GL_COLOR_BUFFER_BIT);

		particles->compute();
		particles->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}