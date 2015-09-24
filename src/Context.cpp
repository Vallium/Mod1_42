#include "Context.hpp"

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
#include <sstream>
#include <fstream>

float	Context::deltaTime = 0.0f;
float	Context::lastFrame = 0.0f;

int		Context::size = 0;
float	**Context::map;

GLFWwindow* Context::window;

int		Context::windowWidth = 640;
int		Context::windowHeight = 480;

Renderer 		*Context::renderer;
Camera 			*Context::camera;
Mesh			*Context::landMesh;
InputManager	*Context::inputManager;

glm::mat4		Context::projection;

void    Context::init(int ac, char **av) {
	initMap(ac, av);
	initGLFW();
	initOGL();
	initRenderer();
	initWorld();
	initProjection();
}

std::vector<std::string>		split(std::string str, char delimiter) {
	std::vector<std::string>		internal;
	std::stringstream		ss(str);
	std::string				tok;

	while(getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}
	return internal;
}

std::vector<int>		parse(std::string name) {
	std::ifstream			file;
	std::string				line;
	std::vector<int>		out;

	file.open(name.data());
	while (std::getline(file, line)) {
		std::vector<std::string> pts = split(line, ' ');
		for (auto pt = pts.begin(); pt != pts.end(); ++pt) {
			std::string tmp = pt->substr(1, pt->size() - 2);

			std::vector<std::string> coords = split(tmp, ',');

			out.push_back(stoi(coords[0]));
			out.push_back(stoi(coords[1]));
			out.push_back(stoi(coords[2]));
		}
	}
	file.close();
	return out;
}

static int		pgcd(int a, int b) {
	if (a == 0)
		return b;
	if (b == 0)
		return a;
	if (a % 2 == 0 and b % 2 == 0)
		return 2 * pgcd(a / 2, b / 2);
	if (a % 2 == 0)
		return pgcd(a / 2, b);
	if (b % 2 == 0)
		return pgcd(a, b / 2);
	if (a > b)
		return pgcd(a - b, b);
	return pgcd(a, b - a);
}

static int		ppcm(int a, int b) {
	return (a * b) / pgcd(a, b);
}

#define BASE_SIZE 20000
#define MIN_TAB_SIZE 500

float		**create_tab(std::vector<int> &pts, int &size) {
	float	**tab = nullptr;

	int		pd = pts[0];

	for (int i = 0; i < pts.size(); i += 3) {
		pd = pgcd(pd, pts[i]);
		pd = pgcd(pd, pts[i + 1]);
		pd = pgcd(pd, pts[i + 2]);
	}
	std::cout << "pgcd: " << pd << std::endl;
	size = ppcm(BASE_SIZE / pd, MIN_TAB_SIZE);

	std::cout << "size: " << size << std::endl;

	tab = new float*[size];
	for (int x = 0; x < size; x++) {
		tab[x] = new float[size];
		for (int y = 0; y < size; y++) {
			tab[x][y] = 0.0f;
		}
	}

	for (int i = 0; i < pts.size(); i += 3) {
		pts[i] = pts[i] * size / BASE_SIZE;
		pts[i + 1] = pts[i + 1] * size / BASE_SIZE;
		pts[i + 2] = pts[i + 2] * size / BASE_SIZE;
	}
	return tab;
}

void 	hill( float **map, int size, int x, int y, int zin ) {
	float z = zin;
	int rayon = 1;

	while (rayon < static_cast<float>(size / 4)) {
		int ix = x - rayon;
		while (ix < x + rayon) {
			int iy = y - rayon;
			while (iy < y + rayon) {
				if ( ix >= 0 and iy > 0 and ix < size and iy < size
					and (ix - x)*(ix - x) + (iy - y)*(iy - y) < (rayon*rayon)
					and map[ix][iy] < z)
					map[ix][iy] = z;
				iy++;
			}
			ix++;
		}
		rayon++;
		z -= static_cast<float>(zin) / static_cast<float>(size / 4);
	}
}


void		errect_hills(float **tab, int size, std::vector<int> pts) {
	for (int i = 0; i < pts.size(); i += 3) {
		hill(tab, size, pts[i], pts[i + 1], pts[i + 2]);
	}
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
	window = glfwCreateWindow(640, 480, "Mod1", NULL, NULL);
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
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void	Context::initRenderer() {

	renderer = new Renderer();

	renderer->loadShaders();

}

static void multipush(std::vector<GLfloat> &target, std::vector<GLfloat> src)
{
	for (std::vector<GLfloat>::iterator it = src.begin(); it < src.end(); ++it) {
		target.push_back(*it);
	}
}

static void addVertex(std::vector<GLfloat> &vertices, float x, float y, float z)
{
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
	vertices.push_back(1.0f * y / 10);
	vertices.push_back(1.0f - y / 10);
	vertices.push_back(1.0f);
}

#define RENDER_SIZE 20.0f

static std::vector<GLfloat>	generateMesh(float **map, int size) {
	std::vector<GLfloat>	vertices;
	float ratio = RENDER_SIZE / static_cast<float>(size);


	for (int y = 0; y < size-1; y++) {
		int x;
		for (x = 0; x < size-1; x++) {
			float	xf = static_cast<float>(x);
			float	yf = static_cast<float>(y);
			// addVertex(vertices, xf, static_cast<float>(map[x][y]), yf);
			vertices.push_back(xf * ratio);
			vertices.push_back(map[x][y] * ratio);
			vertices.push_back(yf * ratio);
			vertices.push_back(0.8f * map[x][y] * ratio / 5.0f);
			vertices.push_back(0.4f);
			vertices.push_back(0.0f);

			// addVertex(vertices, xf,(map[x][y + 1]), yf + 1.0f);
			vertices.push_back(xf * ratio);
			vertices.push_back(map[x][y + 1] * ratio);
			vertices.push_back((yf + 1.0f) * ratio);
			vertices.push_back(0.8f * map[x][y] * ratio / 5.0f);
			vertices.push_back(0.4f);
			vertices.push_back(0.0f);

			// addVertex(vertices, xf + 1.0f,(map[x + 1][y]), yf);
			vertices.push_back((xf + 1.0f) * ratio);
			vertices.push_back(map[x + 1][y] * ratio);
			vertices.push_back(yf * ratio);
			vertices.push_back(0.8f * map[x+1][y] * ratio / 5.0f);
			vertices.push_back(0.4f);
			vertices.push_back(0.0f);

			// addVertex(vertices, xf + 1.0f,(map[x + 1][y + 1]), yf + 1.0f);
			vertices.push_back((xf + 1.0f) * ratio);
			vertices.push_back(map[x + 1][y + 1] * ratio);
			vertices.push_back((yf + 1.0f) * ratio);
			vertices.push_back(0.8f * map[x+1][y] * ratio / 5.0f);
			vertices.push_back(0.4f);
			vertices.push_back(0.0f);


			// addVertex(vertices, xf + 1.0f,(map[x + 1][y]), yf);
			vertices.push_back((xf + 1.0f) * ratio);
			vertices.push_back(map[x + 1][y] * ratio);
			vertices.push_back(yf * ratio);
			vertices.push_back(0.8f * map[x+1][y] * ratio / 5.0f);
			vertices.push_back(0.4f);
			vertices.push_back(0.0f);

			// addVertex(vertices, xf,(map[x][y + 1]), yf + 1.0f);
			vertices.push_back(xf * ratio);
			vertices.push_back(map[x][y + 1] * ratio);
			vertices.push_back((yf + 1.0f) * ratio);
			vertices.push_back(0.8f * map[x][y] * ratio / 5.0f);
			vertices.push_back(0.4f);
			vertices.push_back(0.0f);
		}
	}
	return vertices;
}

void	Context::initWorld() {
	camera = new Camera(glm::vec3(-10.0f, 25.0f, -10.0f));
	landMesh = new Mesh();
	inputManager = new InputManager(window, camera);

	landMesh->setVertices(generateMesh(map, size));
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
