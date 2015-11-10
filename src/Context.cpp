#include "Context.hpp"
#include "Map.hpp"
#include "sphere.hpp"

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "Camera.hpp"
#include "Renderer.hpp"
#include "InputManager.hpp"
#include "Octree.hpp"

#include "parse.hpp"

#include <iostream>
#include <string>
#include <algorithm>

float	Context::deltaTime = 0.0f;
float	Context::lastFrame = 0.0f;
short	Context::fps = 0;

GLFWwindow* Context::window;

int 	Context::size = 20000;

// iMac 42
int		Context::windowWidth = 1920;
int		Context::windowHeight = 1080;

// Retina
// int		Context::windowWidth = 1440;
// int		Context::windowHeight = 900;

Map				*Context::map;
Renderer 		*Context::renderer;
Camera 			*Context::camera;
// Mesh			*Context::particleMesh;
WaterMap		*Context::waterMap;
InputManager	*Context::inputManager;
// Octree			*Context::drops;

glm::mat4		Context::projection;

void    Context::init(int ac, char **av) {
	initGLFW();
	initOGL();
	initRenderer();
	initMap(ac, av);
	initWorld();
	initProjection();
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

	std::srand(std::time(0));

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

void	Context::initMap(int ac, char **av) {
	if (ac != 2) {
		std::cout << "Put a file motherfucker!!" <<std::endl;
		exit(-1);
	}
	else {
		std::vector<glm::vec3> pts = parse(std::string(av[1]));
		map = new Map();

		for (auto it = pts.begin(); it != pts.end(); ++it) {
			map->addPoint(*it);
			std::cout << it->x << " " << it->y << " " << it->z << std::endl;
		}

	}
}
#define NB_DROPS 20.0f

void	Context::initWorld() {
	camera = new Camera(glm::vec3(30000 / ZOOM, 30000 / ZOOM, 30000 / ZOOM));
	// particleMesh = new Mesh();
	inputManager = new InputManager(window, camera);
	// drops = new Octree(glm::vec3(10000, 10000, 10000), glm::vec3(10000, 10000, 10000));

	waterMap = new WaterMap(map);

	map->generateMesh();

	waterMap->generateMesh();

	// unsigned int particleVertexBufferSize;
	// unsigned int particleElementBufferSize;
	// GLfloat		*particleVertexBuffer = nullptr;
	// GLuint		*particleElementBuffer = nullptr;
	// generateCubeMesh(DROP_RENDER_SIZE / ZOOM, &particleVertexBuffer, particleVertexBufferSize, &particleElementBuffer, particleElementBufferSize);
	// particleMesh->setVertexBuffer(particleVertexBuffer, particleVertexBufferSize);
	// particleMesh->setElementBuffer(particleElementBuffer, particleElementBufferSize);

	// for (float x = 1; x < NB_DROPS; x++) {
	// 	for (float y = 1; y < NB_DROPS; y++) {
	// 		for (float z = 1; z < NB_DROPS; z++) {
	// 			drops->insert(new Drop(glm::vec3(x * size / NB_DROPS, 10000 + y * 10000 / NB_DROPS, z * size / NB_DROPS)));
	// 		}
	// 	}
	// }

	// drops->insert(new Drop(glm::vec3(250, 15000, 200)));
	// drops->insert(new Drop(glm::vec3(1, 250, 1)));
}

void	Context::initProjection() {
	projection = glm::perspective(45.0f, (GLfloat)windowWidth/(GLfloat)windowHeight, 0.01f, 1000000.0f);
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

	// Drop::update(&drops, deltaTime);

	// std::vector<Drop*> dropsTmp = Drop::update(&drops, deltaTime);

	// drops->getPointsInsideBox(glm::vec3(0, 0, 0), glm::vec3(Context::size, Context::size, Context::size), dropsTmp);

	// unsigned int tmpSize = dropsTmp.size() * 3;
	// GLfloat		*tmp = new float[tmpSize];
	// unsigned int i = 0;
	// for (auto it = dropsTmp.begin(); it != dropsTmp.end(); ++it) {
	// 	tmp[i++] = (*it)->getPos().x / ZOOM;
	// 	tmp[i++] = (*it)->getPos().y / ZOOM;
	// 	tmp[i++] = (*it)->getPos().z / ZOOM;
	// }
	// std::cout << drops->count << std::endl;

	// GLfloat		*tmpPtr = particleMesh->getInstanceBuffer();
	//
	// if (tmpPtr != nullptr)
	// 	delete [] tmpPtr;
	//
	// particleMesh->setInstanceBuffer(tmp, i);
	//
	// Octree	*tmpPtr2;
	//
	// Octree::count = 0;
	// Octree	*tmpPtr3 = new Octree(glm::vec3(10000, 10000, 10000), glm::vec3(10000, 10000, 10000));
	//
	// tmpPtr2 = drops;
	// drops = tmpPtr3;
	// delete tmpPtr2;
	//
	// for (auto it = dropsTmp.begin(); it != dropsTmp.end(); ++it)
	// 	drops->insert(*it);
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
	map->render(renderer);
	waterMap->render(renderer);
	// renderer->getSphereShader()->Use();
	// glUniformMatrix4fv(glGetUniformLocation(renderer->getSphereShader()->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	// particleMesh->render(renderer->getSphereShader(), drops->count);

	// Swap the buffers
	glfwSwapBuffers(window);
}

void	Context::deinit() {
	delete renderer;
	delete inputManager;
	// delete particleMesh;

	//TODO: Properly de-allocate all resources once they've outlived their purpose
	glfwTerminate();
}
