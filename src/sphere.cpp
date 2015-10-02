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

std::vector<GLfloat>	generateSphere(float radius, int rings,int slices){
	int faces = slices * rings;
	int nbV = slices * rings;

	std::vector<GLfloat>	vertices;

	//Allocation de mémoire
	float * v = new float[3 * nbV];
	float * n = new float[3 * nbV];
	// float * tex = new float[2 * nbV];
	unsigned int * id = new unsigned int[6 * faces];

	//GENERATION DE LA SPHERE
	float x,y,z,r,s;
	int idx = 0, tidx = 0;

	float const R = 1.f/(float)(rings-1);
	float const S = 1.f/(float)(slices-1);

	for(r = 0; r < rings; ++r) {
		for(s = 0; s < slices; ++s) {
			x = cos( 2 * M_PI * s * S) * sin( M_PI * r * R );
			z = sin( 2 * M_PI * s * S) * sin( M_PI * r * R );
			y = sin(-M_PI_2 + (M_PI * r * R));

			// buffer[i++] = x * radius;
			// vertices.push_back(y * radius);
			// vertices.push_back(z * radius);
			// vertices.push_back(1.0f);
			// vertices.push_back(1.0f);
			// vertices.push_back(1.0f);
			v[idx] = x * radius;
			v[idx + 1] = y * radius;
			v[idx + 2] = z * radius;

			n[idx] = x;
			n[idx + 1] = y;
			n[idx + 2] = z;

			idx += 3;

			// tex[tidx] = s*S;
			// tex[tidx+1] = r*R;
			tidx += 2;
		}
	}

	idx = 0;
	int ringStart, nextRingStart,nextslice;
	for( r = 0; r < rings; r++ ) {
		ringStart = r * slices;
		nextRingStart = (r + 1) * slices;
		for( s = 0; s < slices; s++ ) {
			nextslice = s+1;
			// The quad
			id[idx] = ringStart + s;
			vertices.push_back(v[(int)(ringStart + s) * 3]);
			vertices.push_back(v[(int)(ringStart + s) * 3 + 1]);
			vertices.push_back(v[(int)(ringStart + s) * 3 + 2]);
			vertices.push_back(0.38f);
			vertices.push_back(0.51f);
			vertices.push_back(0.71f);

			id[idx+1] = nextRingStart + s;
			vertices.push_back(v[(int)(nextRingStart + s) * 3]);
			vertices.push_back(v[(int)(nextRingStart + s) * 3 + 1]);
			vertices.push_back(v[(int)(nextRingStart + s) * 3 + 2]);
			vertices.push_back(0.38f);
			vertices.push_back(0.51f);
			vertices.push_back(0.71f);

			id[idx+2] = nextRingStart + nextslice;
			vertices.push_back(v[(int)(nextRingStart + nextslice) * 3]);
			vertices.push_back(v[(int)(nextRingStart + nextslice) * 3 + 1]);
			vertices.push_back(v[(int)(nextRingStart + nextslice) * 3 + 2]);
			vertices.push_back(0.38f);
			vertices.push_back(0.51f);
			vertices.push_back(0.71f);

			id[idx+3] = ringStart + s;
			vertices.push_back(v[(int)(ringStart + s) * 3]);
			vertices.push_back(v[(int)(ringStart + s) * 3 + 1]);
			vertices.push_back(v[(int)(ringStart + s) * 3 + 2]);
			vertices.push_back(0.38f);
			vertices.push_back(0.51f);
			vertices.push_back(0.71f);

			id[idx+4] = nextRingStart + nextslice;
			vertices.push_back(v[(int)(nextRingStart + nextslice) * 3]);
			vertices.push_back(v[(int)(nextRingStart + nextslice) * 3 + 1]);
			vertices.push_back(v[(int)(nextRingStart + nextslice) * 3 + 2]);
			vertices.push_back(0.38f);
			vertices.push_back(0.51f);
			vertices.push_back(0.71f);

			id[idx+5] = ringStart + nextslice;
			vertices.push_back(v[(int)(ringStart + nextslice) * 3]);
			vertices.push_back(v[(int)(ringStart + nextslice) * 3 + 1]);
			vertices.push_back(v[(int)(ringStart + nextslice) * 3 + 2]);
			vertices.push_back(0.38f);
			vertices.push_back(0.51f);
			vertices.push_back(0.71f);

			idx += 6;
		}
	}
	delete [] v;
	delete [] n;
	delete [] id;

	return vertices;
}
