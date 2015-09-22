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

			// std::cout << coords[0] << " " << coords[1] << " " << coords[2] << std::endl;
			out.push_back(stoi(coords[0]));
			out.push_back(stoi(coords[1]));
			out.push_back(stoi(coords[2]));
		}
	}
	file.close();
	return out;
}

int			**create_tab(std::vector<int> pts) {
	int		**tab = nullptr;
	int xmin, xmax, ymin, ymax = -1;

	for (int i = 0; i < pts.size(); i += 3) {
		if (xmin == -1 or pts[i] < xmin)
			xmin = pts[i];
		if (xmax == -1 or pts[i] > xmax)
			xmax = pts[i];
		if (ymin == -1 or pts[i + 1] < ymin)
			ymin = pts[i + 1];
		if (ymax == -1 or pts[i + 1] > ymax)
			ymax = pts[i + 1];
	}

	// int		tab[xmax - xmin + 1][ymax - ymin + 1];
	*tab = new int[xmax - xmin + 1];
	for (size_t x = 0; x < xmax - xmin; x++) {
		tab[x] = new int[ymax - ymin + 1];
		for (size_t y = 0; y < ymax - ymin; y++) {
			tab[x][y] = -1;
		}
	}

	for (int i = 0; i < pts.size(); i += 3) {
		tab[pts[i]][pts[i + 1]] = pts[i + 2];
	}
	// std::cout << xmin << " " << xmax << " " << ymin << " " << ymax << std::endl;
	return tab;
}

int			main(int ac, char *av[]) {
	GLFWwindow	*window;

	/* Initialize the library */
	// if (!glfwInit())
	// 	return -1;

	if (ac != 2) {
		std::cout << "Put a file motherfucker!!" <<std::endl;
		return -1;
	}
	else {
		std::vector<int> pts = parse(std::string(av[1]));
		int		**tab = create_tab(pts);

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
