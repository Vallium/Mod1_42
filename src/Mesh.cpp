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

	setVertices({0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});
	setInstances({0.0f, 0.0f, 0.0f});
}

void	Mesh::setVertices(std::vector<GLfloat> vert) {
	_vertices = vert;
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), &(_vertices[0]), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void	Mesh::setInstances(std::vector<GLfloat> instancesArray) {
	_instanceBuffer = instancesArray;
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _instancedVBO);
	glBufferData(GL_ARRAY_BUFFER, _instanceBuffer.size() * sizeof(GLfloat), &(_instanceBuffer[0]), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void	Mesh::render(Shader *shader) {
	shader->Use();

	glm::mat4 model;

	model = glm::translate(model, _pos);
	glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, _vertices.size() / 6);
	glBindVertexArray(0);
}

void	Mesh::render(Shader *shader, unsigned int instances) {
	shader->Use();

	glm::mat4 model;

	model = glm::translate(model, _pos);
	glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(_VAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, _vertices.size() / 6, instances);
	glBindVertexArray(0);
}
