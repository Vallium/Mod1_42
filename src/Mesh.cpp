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
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_instancedVBO);
	glGenBuffers(1, &_EBO);

	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// TexCoord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, _instancedVBO);

	// Instanced Position attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(3);

	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	_pos = glm::vec3(0,0,0);

	_vertexBuffer = nullptr;
	_instanceBuffer = nullptr;
	_elementBuffer = nullptr;

	GLfloat		*tmp = new GLfloat[3];

	for (size_t i = 0; i < 3; i++)
		tmp[i] = 0;
	setInstanceBuffer(tmp, 3);
}

void	Mesh::setVertexBuffer(GLfloat *buffer, unsigned int size) {
	_vertexBuffer = buffer;
	_vertexBufferSize = size;
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertexBufferSize * sizeof(GLfloat), _vertexBuffer, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void	Mesh::setInstanceBuffer(GLfloat *buffer, unsigned int size) {
	_instanceBuffer = buffer;
	_instanceBufferSize = size;
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _instancedVBO);
	glBufferData(GL_ARRAY_BUFFER, _instanceBufferSize * sizeof(GLfloat), _instanceBuffer, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void	Mesh::setElementBuffer(GLuint *buffer, unsigned int size) {
	_elementBuffer = buffer;
	_elementBufferSize = size;
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _elementBufferSize * sizeof(GLuint), _elementBuffer, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

GLfloat		*Mesh::getVertexBuffer(void) {
	return _vertexBuffer;
}

GLfloat		*Mesh::getInstanceBuffer(void) {
	return _instanceBuffer;
}

GLuint		*Mesh::getElementBuffer(void) {
	return _elementBuffer;
}


void	Mesh::render(Shader *shader) {
	render(shader, 1);
}

void	Mesh::render(Shader *shader, unsigned int instances) {
	if (_vertexBuffer == nullptr or _instanceBuffer == nullptr)
		return;

	glBindVertexArray(_VAO);
	if (_elementBuffer == nullptr)
		glDrawArraysInstanced(GL_TRIANGLES, 0, _vertexBufferSize / 6, instances);
	else
		glDrawElementsInstanced(GL_TRIANGLES, _elementBufferSize, GL_UNSIGNED_INT, (void*)0, instances);
	glBindVertexArray(0);
}
