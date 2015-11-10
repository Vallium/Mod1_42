#include "WaterMap.hpp"

WaterMap::WaterMap(Map *map) {
	_wmRenderer = new WaterMapRenderer();

	_waterMap = new float*[WATERMAP_RESOLUTION]();

	_landMap = new float*[WATERMAP_RESOLUTION]();

	for (int x = 0; x < WATERMAP_RESOLUTION; x++) {
		_waterMap[x] = new float[WATERMAP_RESOLUTION]();
		_landMap[x] = new float[WATERMAP_RESOLUTION]();

		for (int y = 0; y < WATERMAP_RESOLUTION; y++) {
			_waterMap[x][y] = 1000.0f;

			_landMap[x][y] = map->interpolate(static_cast<float>(x) * 20000.0f / static_cast<float>(WATERMAP_RESOLUTION), static_cast<float>(y) * 20000.0f / static_cast<float>(WATERMAP_RESOLUTION));
		}
	}
}

WaterMap::~WaterMap() {
	delete _wmRenderer;

	for (int i = 0; i < WATERMAP_RESOLUTION; i++)
		delete [] _waterMap[i];
	delete [] *_waterMap;
}

#include <iostream>

float	WaterMap::getWaterHeight(float x, float y) {
	int		x1 = static_cast<int>(x) * WATERMAP_RESOLUTION / 20000;
	int		y1 = static_cast<int>(y) * WATERMAP_RESOLUTION / 20000;

	if (x1 < 0) {
		std::cout << x1 << std::endl;
		x1 = 0;
	}
	else if (x1 >= WATERMAP_RESOLUTION) {
		std::cout << x1 << std::endl;
		x1 = WATERMAP_RESOLUTION - 1;
	}

	if (y1 < 0) {
		std::cout << y1 << std::endl;
		y1 = 0;
	}
	else if (y1 >= WATERMAP_RESOLUTION) {
		std::cout << y1 << std::endl;
		y1 = WATERMAP_RESOLUTION - 1;
	}

	return _waterMap[x1][y1] + _landMap[x1][y1];
}

void	WaterMap::update(float dt) {
	// for (int x = 0; x < WATERMAP_RESOLUTION; x++) {
	// 	for (int y = 0; y < WATERMAP_RESOLUTION; y++)
	// 		if (_waterMap[x][y] > _waterMap[x + 1][y])
	//
	// }
}

void	WaterMap::addWater(float x, float y, float amount) {

}

void		WaterMap::render(Renderer *r) {
	_wmRenderer->render(r);
}

void		WaterMap::generateMesh() {
	_wmRenderer->generateMesh(this);
}
