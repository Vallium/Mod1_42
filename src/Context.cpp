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
#include "Octree.hpp"

#include <iostream>
#include <string>

float	Context::deltaTime = 0.0f;
float	Context::lastFrame = 0.0f;
short	Context::fps = 0;

int		Context::size = 0;
float	**Context::map;

GLFWwindow* Context::window;

// iMac 42
int		Context::windowWidth = 1920;
int		Context::windowHeight = 1080;

// Retina
// int		Context::windowWidth = 1440;
// int		Context::windowHeight = 900;

Renderer 		*Context::renderer;
Camera 			*Context::camera;
Mesh			*Context::landMesh;
Mesh			*Context::particleMesh;
InputManager	*Context::inputManager;
Octree			*Context::drops;

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

	// get the actual size of the window in pixels on displays that use window
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

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
	drops = new Octree(glm::vec3(size / 2, size / 2, size / 2), glm::vec3(size / 2, size / 2, size / 2));

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

	for (float x = DROP_PHYSIC_SIZE; x < size; x += 10) {
		for (float y = DROP_PHYSIC_SIZE; y < size; y += 10) {
			drops->insert(new Drop(glm::vec3(x, size, y)));
		}
	}
}

void	Context::initProjection() {
	projection = glm::perspective(45.0f, (GLfloat)windowWidth/(GLfloat)windowHeight, 0.01f, 10000.0f);
}


bool    Context::shouldClose() {
	return glfwWindowShouldClose(window);
}

static short		median(std::vector<short> scores)
{
	short		median;
	size_t	size = scores.size();

	sort(scores.begin(), scores.end());

	if (size % 2 == 0) {
		median = (scores[size / 2 - 1] + scores[size / 2]) / 2;
	}
	else {
		median = scores[size / 2];
	}
	return median;
}

void	Context::update() {
	static std::vector<short>		fpss;

	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	fpss.push_back(static_cast<short>(1.0f / deltaTime));
	if (fpss.size() > 10)
		fpss.erase(++fpss.begin());
	fps = median(fpss);

	// deltaTime *= 0.5f;
	glfwPollEvents();
	inputManager->update(deltaTime);
	// world->update(deltaTime);

	Drop::update(drops, deltaTime);

	std::vector<Drop*> dropsTmp;

	drops->getPointsInsideBox(glm::vec3(0, 0, 0), glm::vec3(Context::size, Context::size, Context::size), dropsTmp);

	unsigned int tmpSize = dropsTmp.size() * 3;
	GLfloat		*tmp = new float[tmpSize];
	unsigned int i = 0;
	for (auto it = dropsTmp.begin(); it != dropsTmp.end(); ++it) {
		tmp[i++] = (*it)->getPos().x / static_cast<float>(size) * RENDER_SIZE;
		tmp[i++] = (*it)->getPos().y / static_cast<float>(size) * RENDER_SIZE;
		tmp[i++] = (*it)->getPos().z / static_cast<float>(size) * RENDER_SIZE;
	}

	GLfloat		*tmpPtr = particleMesh->getInstanceBuffer();

	if (tmpPtr != nullptr)
		delete [] tmpPtr;

	particleMesh->setInstanceBuffer(tmp, tmpSize);
}

void	Context::draw() {
	glfwSetWindowTitle(window, std::to_string(fps).c_str());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = camera->GetViewMatrix();

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 MVP = projection * view * model;

	renderer->getLandShader()->Use();
	glUniformMatrix4fv(glGetUniformLocation(renderer->getLandShader()->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	renderer->getSphereShader()->Use();
	glUniformMatrix4fv(glGetUniformLocation(renderer->getSphereShader()->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	landMesh->render(renderer->getLandShader());
	particleMesh->render(renderer->getSphereShader(), drops->count);

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
