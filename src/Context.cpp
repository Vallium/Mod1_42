#include "Context.hpp"
#include "map.hpp"
#include "sphere.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
#define GLM_FORCE_RADIANS

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "Camera.hpp"
#include "Renderer.hpp"
#include "InputManager.hpp"

#include <iostream>

float	Context::deltaTime = 0.0f;
float	Context::lastFrame = 0.0f;

int		Context::size = 0;
float	**Context::map;

GLFWwindow* Context::window;

int		Context::windowWidth = 1920;
int		Context::windowHeight = 1080;

Renderer 		*Context::renderer;
Camera 			*Context::camera;
Mesh			*Context::landMesh;
Mesh			*Context::particleMesh;
InputManager	*Context::inputManager;
std::vector<Drop>	*Context::drops;

glm::mat4		Context::projection;

void    Context::init(int ac, char **av) {
	initMap(ac, av);
	initGLFW();
	initOGL();
	initRenderer();
	initWorld();
	initProjection();
}

void	Context::initMap(int ac, char **av) {
	if (ac != 2) {
		std::cout << "Put a file motherfucker!!" <<std::endl;
		exit(-1);
	}
	else {
		std::vector<int> pts = parse(std::string(av[1]));
		map = create_tab(pts, size);

		errect_hills(map, size, pts);
	}
}

void	Context::initGLFW() {
	if (!glfwInit())
		std::cout << "ERROR\n";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(windowWidth, windowHeight, "Mod1", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cout << "ERROR2\n";
	}

	glfwMakeContextCurrent(window);

	srand(1234);

	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, windowWidth, windowHeight);

}

void	Context::initOGL() {

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	// glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void	Context::initRenderer() {

	renderer = new Renderer();

	renderer->loadShaders();

}

void	Context::initWorld() {
	camera = new Camera(glm::vec3(-RENDER_SIZE / 2.0f, RENDER_SIZE, -RENDER_SIZE / 2.0f));
	landMesh = new Mesh();
	particleMesh = new Mesh();
	inputManager = new InputManager(window, camera);
	drops = new std::vector<Drop>();

	unsigned int landVertexBufferSize;
	GLfloat		*landVertexBuffer = generateLandMesh(map, size, landVertexBufferSize);
	landMesh->setVertexBuffer(landVertexBuffer, landVertexBufferSize);

	unsigned int particleVertexBufferSize;
	unsigned int particleElementBufferSize;
	GLfloat		*particleVertexBuffer = nullptr;
	GLuint		*particleElementBuffer = nullptr;
	generateCubeMesh(DROP_RENDER_SIZE, &particleVertexBuffer, particleVertexBufferSize, &particleElementBuffer, particleElementBufferSize);
	particleMesh->setVertexBuffer(particleVertexBuffer, particleVertexBufferSize);
	particleMesh->setElementBuffer(particleElementBuffer, particleElementBufferSize);

	for (float x = 0.0f; x < size; x += 10) {
		for (float y = 0.0f; y < size; y += 10) {
			drops->push_back(Drop(glm::vec3(x, size, y)));
		}
	}
}

void	Context::initProjection() {
	projection = glm::perspective(45.0f, (GLfloat)windowWidth/(GLfloat)windowHeight, 0.01f, 10000.0f);
	renderer->getLandShader()->Use();
	glUniformMatrix4fv(glGetUniformLocation(renderer->getLandShader()->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	renderer->getSphereShader()->Use();
	glUniformMatrix4fv(glGetUniformLocation(renderer->getSphereShader()->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}


bool    Context::shouldClose() {
	return glfwWindowShouldClose(window);
}

void	Context::update() {
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glfwPollEvents();
	inputManager->update(deltaTime);
	// world->update(deltaTime);

	Drop::update(drops, deltaTime);

	unsigned int tmpSize = drops->size() * 3;
	GLfloat		*tmp = new float[tmpSize];
	unsigned int i = 0;
	for (auto it = drops->begin(); it != drops->end(); ++it) {
		tmp[i++] = it->getPos().x / static_cast<float>(size) * RENDER_SIZE;
		tmp[i++] = it->getPos().y / static_cast<float>(size) * RENDER_SIZE;
		tmp[i++] = it->getPos().z / static_cast<float>(size) * RENDER_SIZE;
	}

	GLfloat		*tmpPtr = particleMesh->getInstanceBuffer();

	if (tmpPtr != nullptr)
		delete [] tmpPtr;

	particleMesh->setInstanceBuffer(tmp, tmpSize);
}

void	Context::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = camera->GetViewMatrix();

	renderer->getLandShader()->Use();
	glUniformMatrix4fv(glGetUniformLocation(renderer->getLandShader()->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	renderer->getSphereShader()->Use();
	glUniformMatrix4fv(glGetUniformLocation(renderer->getSphereShader()->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	landMesh->render(renderer->getLandShader());
	particleMesh->render(renderer->getSphereShader(), drops->size());

	// Swap the buffers
	glfwSwapBuffers(window);
}

void	Context::deinit() {
	delete renderer;
	delete inputManager;
	delete landMesh;
	delete particleMesh;

	//TODO: Properly de-allocate all resources once they've outlived their purpose
	glfwTerminate();
}
