#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

std::vector<std::string>		split(std::string str, char delimiter) {
	std::vector<std::string>		internal;
	std::stringstream		ss(str);
	std::string				tok;

	while(getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}
	return internal;
}

void		extract(std::string name) {
	std::ifstream			file;
	std::string				line;

	file.open(name.data());
	while (std::getline(file, line)) {
		std::vector<std::string> pts = split(line, ' ');
		for (auto pt = pts.begin(); pt != pts.end(); ++pt) {
			std::string tmp = pt->substr(1, pt->size() - 2);
			std::vector<std::string> coords = split(tmp, ',');
			std::cout << coords[0] << " " << coords[1] << " " << coords[2] << std::endl;
		}
	}
	file.close();
}

int			main(int ac, char *av[]) {
	GLFWwindow* window;

	/* Initialize the library */
	// if (!glfwInit())
	// 	return -1;

	if (ac != 2) {
		std::cout << "Put a file motherfucker!!" <<std::endl;
		return -1;
	}
	else {
		extract(std::string(av[1]));
	}
	// /* Create a windowed mode window and its OpenGL context */
	// window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	// if (!window)
	// {
	// 	glfwTerminate();
	// 	return -1;
	// }
	//
	// /* Make the window's context current */
	// glfwMakeContextCurrent(window);
	//
	// /* Loop until the user closes the window */
	// while (!glfwWindowShouldClose(window))
	// {
	// 	/* Render here */
	//
	// 	/* Swap front and back buffers */
	// 	glfwSwapBuffers(window);
	//
	// 	/* Poll for and process events */
	// 	glfwPollEvents();
	// }
	//
	// glfwTerminate();
	return 0;
}
