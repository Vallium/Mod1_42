#include "MapRenderer.hpp"

MapRenderer::MapRenderer() : _generated(false) {
	_mesh = new Mesh();
}

MapRenderer::~MapRenderer() {
	delete _mesh;
}

void	MapRenderer::generateMesh(Map *map) {
	_generated = true;

	int		bufferSize = MAP_RENDERER_LOD * MAP_RENDERER_LOD * 6 * 6;
	float	*buffer = new float[bufferSize];

	float step = 20000.0f / MAP_RENDERER_LOD;

	unsigned int i = 0;
	for (float x = 0.0f; x < 20000.0f; x += step) {
		for (float z = 0.0f; z < 20000.0f; z += step) {
			buffer[i++] = x;
			buffer[i++] = map->interpolate(x, z);
			buffer[i++] = z;
			buffer[i++] = 0.8f * map->interpolate(x, z) / 20000.0f;
			buffer[i++] = 0.4f;
			buffer[i++] = 0.0f;

			buffer[i++] = x;
			buffer[i++] = map->interpolate(x, z + step);
			buffer[i++] = z + step;
			buffer[i++] = 0.8f * map->interpolate(x, z + step) / 20000.0f;
			buffer[i++] = 0.4f;
			buffer[i++] = 0.0f;

			buffer[i++] = x + step;
			buffer[i++] = map->interpolate(x + step, z);
			buffer[i++] = z;
			buffer[i++] = 0.8f * map->interpolate(x + step, z) / 20000.0f;
			buffer[i++] = 0.4f;
			buffer[i++] = 0.0f;

			buffer[i++] = x + step;
			buffer[i++] = map->interpolate(x + step, z + step);
			buffer[i++] = z + step;
			buffer[i++] = 0.8f * map->interpolate(x + step, z + step) / 20000.0f;
			buffer[i++] = 0.4f;
			buffer[i++] = 0.0f;

			buffer[i++] = x;
			buffer[i++] = map->interpolate(x, z + step);
			buffer[i++] = z + step;
			buffer[i++] = 0.8f * map->interpolate(x, z + step) / 20000.0f;
			buffer[i++] = 0.4f;
			buffer[i++] = 0.0f;

			buffer[i++] = x + step;
			buffer[i++] = map->interpolate(x + step, z);
			buffer[i++] = z;
			buffer[i++] = 0.8f * map->interpolate(x + step, z) / 20000.0f;
			buffer[i++] = 0.4f;
			buffer[i++] = 0.0f;
		}
	}

	_mesh->setVertexBuffer(buffer, bufferSize);
}

void	MapRenderer::render(Renderer *r) {
	if (_generated)
		_mesh->render(r->getLandShader());
}
