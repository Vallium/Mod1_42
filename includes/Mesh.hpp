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
	GLuint		_EBO;
	glm::vec3	_pos;

	GLfloat			*_vertexBuffer;
	unsigned int	_vertexBufferSize;

	GLfloat			*_instanceBuffer;
	unsigned int	_instanceBufferSize;

	GLuint			*_elementBuffer;
	unsigned int	_elementBufferSize;

public:
	Mesh();

	// void	generate(int **map, int sizeX, int sizeY);
	void	setVertexBuffer(GLfloat *buffer, unsigned int size);
	void	setInstanceBuffer(GLfloat *buffer, unsigned int size);
	void	setElementBuffer(GLuint *buffer, unsigned int size);

	GLfloat		*getVertexBuffer(void);
	GLfloat		*getInstanceBuffer(void);
	GLuint		*getElementBuffer(void);

	void	render(Shader *shader);
	void	render(Shader *shader, unsigned int instances);
};

#endif
