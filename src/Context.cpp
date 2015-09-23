#include "Context.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
#define GLM_FORCE_RADIANS

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL/SOIL.h>

#include <vector>
#include "Camera.hpp"
#include "Renderer.hpp"

float	Context::deltaTime = 0.0f;
float	Context::lastFrame = 0.0f;

GLFWwindow* Context::window;

int		Context::windowWidth;
int		Context::windowHeight;

Renderer 		*Context::renderer;

glm::mat4		Context::projection;

void    Context::init() {
	initGLFW();
	initOGL();
	initRenderer();
	initWorld();
	initProjection();
	// initTextures();
}

void	Context::initGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	window = glfwCreateWindow(mode->width, mode->height, "Mod1", monitor, nullptr);
	glfwMakeContextCurrent(window);

	GLuint screenWidth = mode->width, screenHeight = mode->height;

	windowWidth = mode->width;
	windowHeight = mode->height;

	srand(1234);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, screenWidth, screenHeight);

}

void	Context::initOGL() {

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void	Context::initRenderer() {

	renderer = new Renderer();

	renderer->setLandShader(new Shader("shaders/land.vert", "shaders/land.frag"));
}

void	Context::initWorld() {

}

void	Context::initProjection() {
	projection = glm::perspective(45.0f, (GLfloat)windowWidth/(GLfloat)windowHeight, 0.01f, 10000.0f);
	renderer->getLandShader()->Use();
	glUniformMatrix4fv(glGetUniformLocation(renderer->getLandShader()->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

// void	Context::initTextures() {
// 	renderer->getBlockShader()->Use();
// 	glActiveTexture(GL_TEXTURE0);
// 	glBindTexture(GL_TEXTURE_2D_ARRAY, renderer->getTextureManager()->getTileset());
// 	glUniform1i(glGetUniformLocation(renderer->getBlockShader()->getProgram(), "ourTexture"), 0);
//
// }

bool    Context::shouldClose() {
	return glfwWindowShouldClose(window);
}

void	Context::update() {
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glfwPollEvents();
	// inputManager->update(deltaTime);
	// world->update(deltaTime);
}

void	Context::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = glm::lookAt(camera._pos, camera._pos + camera._front, camera._up);

	// skybox->render(player->_camera, projection);

	renderer->getLandShader()->Use();
	glUniformMatrix4fv(glGetUniformLocation(renderer->getLandShader()->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	// world->render(player->_pos, renderer);

	// Swap the buffers
	glfwSwapBuffers(window);
}

void	Context::deinit() {
	delete renderer;
	// delete skybox;
	// delete world;
	// delete player;
	// delete inputManager;

	//TODO: Properly de-allocate all resources once they've outlived their purpose
	glfwTerminate();
}
