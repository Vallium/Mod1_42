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
#include <iostream>

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

void	Mesh::setVertices(std::vector<GLfloat> vert) {
	_vertices = vert;
	glBindVertexArray((_VAO));
	glBindBuffer(GL_ARRAY_BUFFER, (_VBO));
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), &(_vertices[0]), GL_STATIC_DRAW);
	glBindVertexArray(0);
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
