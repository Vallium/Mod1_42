/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/25 17:41:10 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/25 17:41:11 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sphere.hpp"

void		generateCubeMesh(float size, GLfloat **vertexBuffer, unsigned int &vertexBufferSize, GLuint **elementBuffer, unsigned int &elementBufferSize) {
	vertexBufferSize = 8 * 6;
	elementBufferSize = 6 * 6;

	*vertexBuffer = new GLfloat[vertexBufferSize];
	*elementBuffer = new GLuint[elementBufferSize];

	float	r = size / 2.0f;

	GLfloat		vertexBufferTmp[] = {
		-r, r, -r, 0.38f, 0.51f, 0.71f,
		r, r, -r, 0.38f, 0.51f, 0.71f,
		-r, r, r, 0.38f, 0.51f, 0.71f,
		r, r, r, 0.38f, 0.51f, 0.71f,
		-r, -r, -r, 0.38f, 0.51f, 0.71f,
		r, -r, -r, 0.38f, 0.51f, 0.71f,
		-r, -r, r, 0.38f, 0.51f, 0.71f,
		r, -r, r, 0.38f, 0.51f, 0.71f
	};

	for (size_t i = 0; i < vertexBufferSize; i++)
		(*vertexBuffer)[i] = vertexBufferTmp[i];

	GLfloat		elementBufferTmp[] = {
		0, 1, 2,
		1, 2, 3,
		4, 5, 6,
		5, 6, 7,
		0, 1, 5,
		0, 4, 5,
		1, 5, 7,
		1, 3, 7,
		2, 6, 7,
		2, 3, 7,
		0, 2, 6,
		6, 4, 0
	};

	for (size_t i = 0; i < elementBufferSize; i++)
		(*elementBuffer)[i] = elementBufferTmp[i];
}
