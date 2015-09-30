/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/24 19:12:48 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/24 19:12:50 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

GLfloat		*generateLandMesh(float **map, int size, unsigned int &bufferSize) {
	float ratio = RENDER_SIZE / static_cast<float>(size);

	bufferSize = size * size * 6 * 6;

	GLfloat *buffer = new float[bufferSize];

	unsigned int i = 0;
	for (int y = 0; y < size-1; y++) {
		int x;
		for (x = 0; x < size-1; x++) {
			float	xf = static_cast<float>(x);
			float	yf = static_cast<float>(y);

			buffer[i++] = xf * ratio;
			buffer[i++] = map[x][y] * ratio;
			buffer[i++] = yf * ratio;
			buffer[i++] = 0.8f * map[x][y] * ratio / (RENDER_SIZE / 4.0f);
			buffer[i++] = 0.4f;
			buffer[i++] = 0.0f;

			buffer[i++] = xf * ratio;
			buffer[i++] = map[x][y + 1] * ratio;
			buffer[i++] = (yf + 1.0f) * ratio;
			buffer[i++] = 0.8f * map[x][y] * ratio / (RENDER_SIZE / 4.0f);
			buffer[i++] = 0.4f;
			buffer[i++] = 0.0f;

			buffer[i++] = (xf + 1.0f) * ratio;
			buffer[i++] = map[x + 1][y] * ratio;
			buffer[i++] = yf * ratio;
			buffer[i++] = 0.8f * map[x+1][y] * ratio / (RENDER_SIZE / 4.0f);
			buffer[i++] = 0.4f;
			buffer[i++] = 0.0f;

			buffer[i++] = (xf + 1.0f) * ratio;
			buffer[i++] = map[x + 1][y + 1] * ratio;
			buffer[i++] = (yf + 1.0f) * ratio;
			buffer[i++] = 0.8f * map[x+1][y] * ratio / (RENDER_SIZE / 4.0f);
			buffer[i++] = 0.4f;
			buffer[i++] = 0.0f;


			buffer[i++] = (xf + 1.0f) * ratio;
			buffer[i++] = map[x + 1][y] * ratio;
			buffer[i++] = yf * ratio;
			buffer[i++] = 0.8f * map[x+1][y] * ratio / (RENDER_SIZE / 4.0f);
			buffer[i++] = 0.4f;
			buffer[i++] = 0.0f;

			buffer[i++] = xf * ratio;
			buffer[i++] = map[x][y + 1] * ratio;
			buffer[i++] = (yf + 1.0f) * ratio;
			buffer[i++] = 0.8f * map[x][y] * ratio / (RENDER_SIZE / 4.0f);
			buffer[i++] = 0.4f;
			buffer[i++] = 0.0f;
		}
	}
	return buffer;
}

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

			out.push_back(stoi(coords[0]));
			out.push_back(stoi(coords[1]));
			out.push_back(stoi(coords[2]));
		}
	}
	file.close();
	return out;
}

int		pgcd(int a, int b) {
	if (a == 0)
		return b;
	if (b == 0)
		return a;
	if (a % 2 == 0 and b % 2 == 0)
		return 2 * pgcd(a / 2, b / 2);
	if (a % 2 == 0)
		return pgcd(a / 2, b);
	if (b % 2 == 0)
		return pgcd(a, b / 2);
	if (a > b)
		return pgcd(a - b, b);
	return pgcd(a, b - a);
}

int		ppcm(int a, int b) {
	return (a * b) / pgcd(a, b);
}

float		**create_tab(std::vector<int> &pts, int &size) {
	float	**tab = nullptr;

	int		pd = pts[0];

	for (int i = 0; i < pts.size(); i += 3) {
		pd = pgcd(pd, pts[i]);
		pd = pgcd(pd, pts[i + 1]);
		pd = pgcd(pd, pts[i + 2]);
	}
	std::cout << "pgcd: " << pd << std::endl;
	size = ppcm(BASE_SIZE / pd, MIN_TAB_SIZE);

	std::cout << "size: " << size << std::endl;

	tab = new float*[size];
	for (int x = 0; x < size; x++) {
		tab[x] = new float[size];
		for (int y = 0; y < size; y++) {
			tab[x][y] = 0.0f;
		}
	}

	for (int i = 0; i < pts.size(); i += 3) {
		pts[i] = pts[i] * size / BASE_SIZE;
		pts[i + 1] = pts[i + 1] * size / BASE_SIZE;
		pts[i + 2] = pts[i + 2] * size / BASE_SIZE;
	}
	return tab;
}

void 	hill( float **map, int size, int x, int y, int zin ) {
	float z = zin;
	int rayon = 1;

	while (rayon < static_cast<float>(size / 4)) {
		int ix = x - rayon;
		while (ix < x + rayon) {
			int iy = y - rayon;
			while (iy < y + rayon) {
				if ( ix >= 0 and iy > 0 and ix < size and iy < size
					and (ix - x)*(ix - x) + (iy - y)*(iy - y) < (rayon*rayon)
					and map[ix][iy] < z)
					map[ix][iy] = z;
				iy++;
			}
			ix++;
		}
		rayon++;
		z -= static_cast<float>(zin) / static_cast<float>(size / 4);
	}
}


void		errect_hills(float **tab, int size, std::vector<int> pts) {
	for (int i = 0; i < pts.size(); i += 3) {
		hill(tab, size, pts[i], pts[i + 1], pts[i + 2]);
	}
}
