/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/22 15:46:45 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/22 15:46:49 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mesh.hpp"

static void multipush(std::vector<GLfloat> &target, std::vector<GLfloat> src)
{
	for (std::vector<GLfloat>::iterator it = src.begin(); it < src.end(); ++it) {
		target.push_back(*it);
	}
}

Mesh::Mesh() {
	glGenVertexArrays(1, &(_VAO));
	glGenBuffers(1, &(_VBO));
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray((_VAO));

	glBindBuffer(GL_ARRAY_BUFFER, (_VBO));
	//glBufferData(GL_ARRAY_BUFFER, sizeof(CubeRenderer::_vertices), CubeRenderer::_vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	_pos = glm::vec3(0,0,0);
}

void	Mesh::generate(int **map, int sizeX, int sizeY) {
	for (float y = 0; y < sizeY; y++) {
		float x;
		for (x = 0; x < sizeX; x++) {
			multipush(_vertices, {x, y, static_cast<float>(map[(int)x][(int)y]), x, y + 1, static_cast<float>(map[(int)x][(int)y + 1])});
		}
		multipush(_vertices, {x, y, static_cast<float>(map[(int)x - 1][(int)y]), x, y + 1, static_cast<float>(map[(int)x - 1][(int)y + 1])});
	}
}

void	Mesh::render(Renderer *renderer) {
	renderer->getLandShader()->Use();

	glm::mat4 model;
	model = glm::translate(model, _pos);
	glUniformMatrix4fv(glGetUniformLocation(renderer->getLandShader()->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, _vertices.size() / 6);
	glBindVertexArray(0);
}
