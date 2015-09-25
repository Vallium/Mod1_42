#include "Context.hpp"
#include "map.hpp"

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

int		Context::windowWidth = 800;
int		Context::windowHeight = 600;

Renderer 		*Context::renderer;
Camera 			*Context::camera;
Mesh			*Context::landMesh;
Mesh			*Context::sphereMesh;
InputManager	*Context::inputManager;

glm::mat4		Context::projection;

std::vector<GLfloat>	generateSphere(float radius, int rings,int slices){
	int faces = slices * rings;
	int nbV = slices * rings;

	std::vector<GLfloat>	vertices;

	//Allocation de mémoire
	float * v = new float[3 * nbV];
	float * n = new float[3 * nbV];
	// float * tex = new float[2 * nbV];
	unsigned int * id = new unsigned int[6 * faces];

	//GENERATION DE LA SPHERE
	float x,y,z,r,s;
	int idx = 0, tidx = 0;

	float const R = 1.f/(float)(rings-1);
	float const S = 1.f/(float)(slices-1);

	for(r = 0; r < rings; ++r) {
		for(s = 0; s < slices; ++s) {
			x = cos( 2 * M_PI * s * S) * sin( M_PI * r * R );
			z = sin( 2 * M_PI * s * S) * sin( M_PI * r * R );
			y = sin(-M_PI_2 + (M_PI * r * R));

			// vertices.push_back(x * radius);
			// vertices.push_back(y * radius);
			// vertices.push_back(z * radius);
			// vertices.push_back(1.0f);
			// vertices.push_back(1.0f);
			// vertices.push_back(1.0f);
			v[idx] = x * radius;
			v[idx + 1] = y * radius;
			v[idx + 2] = z * radius;

			n[idx] = x;
			n[idx + 1] = y;
			n[idx + 2] = z;

			idx += 3;

			// tex[tidx] = s*S;
			// tex[tidx+1] = r*R;
			tidx += 2;
		}
	}

	idx = 0;
	int ringStart, nextRingStart,nextslice;
	for( r = 0; r < rings; r++ ) {
		ringStart = r * slices;
		nextRingStart = (r + 1) * slices;
		for( s = 0; s < slices; s++ ) {
			nextslice = s+1;
			// The quad
			id[idx] = ringStart + s;
			vertices.push_back(v[(int)(ringStart + s) * 3]);
			vertices.push_back(v[(int)(ringStart + s) * 3 + 1]);
			vertices.push_back(v[(int)(ringStart + s) * 3 + 2]);
			vertices.push_back(0.2f);
			vertices.push_back(0.2f);
			vertices.push_back(1.0f);

			id[idx+1] = nextRingStart + s;
			vertices.push_back(v[(int)(nextRingStart + s) * 3]);
			vertices.push_back(v[(int)(nextRingStart + s) * 3 + 1]);
			vertices.push_back(v[(int)(nextRingStart + s) * 3 + 2]);
			vertices.push_back(0.2f);
			vertices.push_back(0.2f);
			vertices.push_back(1.0f);

			id[idx+2] = nextRingStart + nextslice;
			vertices.push_back(v[(int)(nextRingStart + nextslice) * 3]);
			vertices.push_back(v[(int)(nextRingStart + nextslice) * 3 + 1]);
			vertices.push_back(v[(int)(nextRingStart + nextslice) * 3 + 2]);
			vertices.push_back(0.2f);
			vertices.push_back(0.2f);
			vertices.push_back(1.0f);

			id[idx+3] = ringStart + s;
			vertices.push_back(v[(int)(ringStart + s) * 3]);
			vertices.push_back(v[(int)(ringStart + s) * 3 + 1]);
			vertices.push_back(v[(int)(ringStart + s) * 3 + 2]);
			vertices.push_back(0.2f);
			vertices.push_back(0.2f);
			vertices.push_back(1.0f);

			id[idx+4] = nextRingStart + nextslice;
			vertices.push_back(v[(int)(nextRingStart + nextslice) * 3]);
			vertices.push_back(v[(int)(nextRingStart + nextslice) * 3 + 1]);
			vertices.push_back(v[(int)(nextRingStart + nextslice) * 3 + 2]);
			vertices.push_back(0.2f);
			vertices.push_back(0.2f);
			vertices.push_back(1.0f);

			id[idx+5] = ringStart + nextslice;
			vertices.push_back(v[(int)(ringStart + nextslice) * 3]);
			vertices.push_back(v[(int)(ringStart + nextslice) * 3 + 1]);
			vertices.push_back(v[(int)(ringStart + nextslice) * 3 + 2]);
			vertices.push_back(0.2f);
			vertices.push_back(0.2f);
			vertices.push_back(1.0f);

			idx += 6;
		}
	}
	return vertices;
}


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
	camera = new Camera(glm::vec3(-10.0f, 25.0f, -10.0f));
	landMesh = new Mesh();
	sphereMesh = new Mesh();
	inputManager = new InputManager(window, camera);

	landMesh->setVertices(generateMesh(map, size));
	sphereMesh->setVertices(generateSphere(1.0f, 20, 20));
}

void	Context::initProjection() {
	projection = glm::perspective(45.0f, (GLfloat)windowWidth/(GLfloat)windowHeight, 0.01f, 10000.0f);
	renderer->getLandShader()->Use();
	glUniformMatrix4fv(glGetUniformLocation(renderer->getLandShader()->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
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
}

void	Context::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = camera->GetViewMatrix();

	renderer->getLandShader()->Use();
	glUniformMatrix4fv(glGetUniformLocation(renderer->getLandShader()->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	landMesh->render(renderer->getLandShader());
	sphereMesh->render(renderer->getLandShader());

	// Swap the buffers
	glfwSwapBuffers(window);
}

void	Context::deinit() {
	delete renderer;
	delete inputManager;
	delete landMesh;

	//TODO: Properly de-allocate all resources once they've outlived their purpose
	glfwTerminate();
}
