#pragma once

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
#include "Mesh.hpp"
#include "InputManager.hpp"

namespace Context {
	extern float	deltaTime;
	extern float	lastFrame;

	extern int		size;
	extern float	**map;

	extern GLFWwindow	*window;

	extern int		windowWidth;
	extern int		windowHeight;

	extern Renderer 		*renderer;
	extern Mesh				*landMesh;
	extern Mesh				*sphereMesh;
	// extern Skybox 			*skybox;
	// extern World	 		*world;
	extern Camera 			*camera;
	extern InputManager 	*inputManager;

	extern glm::mat4		projection;

	extern bool		shouldClose();

	extern void		init(int ac, char **av);
	extern void		initMap(int ac, char **av);
	extern void		initGLFW();
	extern void		initOGL();
	extern void		initRenderer();
	extern void		initWorld();
	extern void		initProjection();
	extern void		initTextures();
	extern void		update();
	extern void		draw();
	extern void		deinit();
}
