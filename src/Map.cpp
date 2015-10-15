#include "Map.hpp"

Map::Map() {
	_mapRenderer = new MapRenderer();
}

Map::~Map() {
	delete _mapRenderer;
}

void		Map::addPoint(glm::vec3 const &pt) {
	_points.push_back(pt);
}

float		Map::interpolate(float x, float z) {
	float	y = 0.0f;

	float		i = 0.0f;
	for (auto it = _points.begin(); it != _points.end(); ++it) {
		float	dist = sqrt((x - it->x) * (x - it->x) + (z - it->z) * (z - it->z)) / 75.0f;
		float	tmpY = it->y - dist * dist;

		if (tmpY >= 0.0f)
			i++, y += tmpY;
	}
	return y / i;
}

void		Map::render(Renderer *r) {
	_mapRenderer->render(r);
}

void		Map::generateMesh() {
	_mapRenderer->generateMesh(this);
}
