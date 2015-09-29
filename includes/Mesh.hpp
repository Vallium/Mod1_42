/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/22 15:47:07 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/22 15:47:08 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESH_HPP
# define MESH_HPP

# define GLEW_STATIC
# include <GL/glew.h>

# include <GLFW/glfw3.h>

# define GLM_FORCE_RADIANS
# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>

# include <vector>

# include "Renderer.hpp"

class		Mesh {
private:
	GLuint		_VAO;
	GLuint		_VBO;
	GLuint		_instancedVBO;
	glm::vec3	_pos;

	std::vector<GLfloat>	_vertices;
	std::vector<GLfloat>	_instanceBuffer;

public:
	Mesh();

	// void	generate(int **map, int sizeX, int sizeY);
	void	setVertices(std::vector<GLfloat> vert);
	void	setInstances(std::vector<GLfloat> vert);
	void	render(Shader *shader);
	void	render(Shader *shader, unsigned int instances);
};

#endif
