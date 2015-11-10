#include "WaterMapRenderer.hpp"

WaterMapRenderer::WaterMapRenderer() : _generated(false) {
	_mesh = new Mesh();
}

WaterMapRenderer::~WaterMapRenderer() {
	delete _mesh;
}

void	WaterMapRenderer::generateMesh(WaterMap *map) {
	_generated = true;

	int		bufferSize = WATERMAP_RESOLUTION * WATERMAP_RESOLUTION * 6 * 6;
	float	*buffer = new float[bufferSize];

	float step = 20000.0f / WATERMAP_RESOLUTION;

	unsigned int i = 0;
	for (float x = 0.0f; x < 20000.0f; x += step) {
		for (float z = 0.0f; z < 20000.0f; z += step) {
			buffer[i++] = x / ZOOM;
			buffer[i++] = map->getWaterHeight(x, z) / ZOOM;
			buffer[i++] = z / ZOOM;
			buffer[i++] = 0.37f;
			buffer[i++] = 0.5f;
			buffer[i++] = 0.6f;

			buffer[i++] = x / ZOOM;
			buffer[i++] = map->getWaterHeight(x, z + step) / ZOOM;
			buffer[i++] = (z + step) / ZOOM;
			buffer[i++] = 0.37f;
			buffer[i++] = 0.5f;
			buffer[i++] = 0.6f;

			buffer[i++] = (x + step) / ZOOM;
			buffer[i++] = map->getWaterHeight(x + step, z) / ZOOM;
			buffer[i++] = z / ZOOM;
			buffer[i++] = 0.37f;
			buffer[i++] = 0.5f;
			buffer[i++] = 0.6f;

			buffer[i++] = (x + step) / ZOOM;
			buffer[i++] = map->getWaterHeight(x + step, z + step) / ZOOM;
			buffer[i++] = (z + step) / ZOOM;
			buffer[i++] = 0.37f;
			buffer[i++] = 0.5f;
			buffer[i++] = 0.6f;

			buffer[i++] = x / ZOOM;
			buffer[i++] = map->getWaterHeight(x, z + step) / ZOOM;
			buffer[i++] = (z + step) / ZOOM;
			buffer[i++] = 0.37f;
			buffer[i++] = 0.5f;
			buffer[i++] = 0.6f;

			buffer[i++] = (x + step) / ZOOM;
			buffer[i++] = map->getWaterHeight(x + step, z) / ZOOM;
			buffer[i++] = z / 100;
			buffer[i++] = 0.37f;
			buffer[i++] = 0.5f;
			buffer[i++] = 0.6f;
		}
	}
	_mesh->setVertexBuffer(buffer, bufferSize);
}

void	WaterMapRenderer::render(Renderer *r) {
	if (_generated)
		_mesh->render(r->getLandShader());
}
