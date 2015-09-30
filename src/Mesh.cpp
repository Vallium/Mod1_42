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

	glBindVertexArray(0);

	_pos = glm::vec3(0,0,0);

	_vertexBuffer = new float[6];
	_instanceBuffer = new float[3];

	GLfloat *vertexBuffer = new float[6];
	GLfloat *instanceBuffer = new float[3];

	vertexBuffer[0] = 0.0f;
	vertexBuffer[1] = 0.0f;
	vertexBuffer[2] = 0.0f;
	vertexBuffer[3] = 0.0f;
	vertexBuffer[4] = 0.0f;
	vertexBuffer[5] = 0.0f;

	instanceBuffer[0] = 0.0f;
	instanceBuffer[1] = 0.0f;
	instanceBuffer[2] = 0.0f;

	setVertexBuffer(vertexBuffer, 6); // ??
	setInstanceBuffer(instanceBuffer, 3);
}

void	Mesh::setVertexBuffer(GLfloat *buffer, unsigned int size) {
	if (_vertexBuffer)
		delete [] _vertexBuffer;
	_vertexBuffer = buffer;
	_vertexBufferSize = size;
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertexBufferSize * sizeof(GLfloat), _vertexBuffer, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void	Mesh::setInstanceBuffer(GLfloat *buffer, unsigned int size) {
	if (_instanceBuffer)
		delete [] _instanceBuffer;
	_instanceBuffer = buffer;
	_instanceBufferSize = size;
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _instancedVBO);
	glBufferData(GL_ARRAY_BUFFER, _instanceBufferSize * sizeof(GLfloat), _instanceBuffer, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void	Mesh::setElementBuffer(GLfloat *buffer, unsigned int size) {
	_elementBuffer = buffer;
	_elementBufferSize = size;
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _elementBufferSize * sizeof(GLuint), _elementBuffer, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void	Mesh::render(Shader *shader) {
	shader->Use();

	glm::mat4 model;

	model = glm::translate(model, _pos);
	glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, _vertexBufferSize / 6);
	glBindVertexArray(0);
}

void	Mesh::render(Shader *shader, unsigned int instances) {
	shader->Use();

	glm::mat4 model;

	model = glm::translate(model, _pos);
	glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(_VAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, _vertexBufferSize / 6, instances);
	glBindVertexArray(0);
}
