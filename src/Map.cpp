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

	for (auto it = _points.begin(); it != _points.end(); ++it) {
		float	dist = sqrt((x - it->x) * (x - it->x) + (z - it->z) * (z - it->z)) / 7500.0f;
		float	coef = 1.0f - dist * dist;
		coef = coef < 0 ? 0 : coef;
		float	tmpY = it->y * coef;

		if (tmpY * coef > y)
			y = tmpY * coef;
	}
	return y;
}

void		Map::render(Renderer *r) {
	_mapRenderer->render(r);
}

void		Map::generateMesh() {
	_mapRenderer->generateMesh(this);
}
