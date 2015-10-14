#ifndef MAPRENDERER_HPP
# define MAPRENDERER_HPP

# include <glm/glm.hpp>
# include <vector>

# include "Mesh.hpp"
# include "Map.hpp"
# include "Renderer.hpp"

# define MAP_RENDERER_LOD 100

class Map;

class		MapRenderer {
private:
	Mesh	*_mesh;
	bool	_generated;

public:
	MapRenderer();
	~MapRenderer();

	void	generateMesh(Map *map);
	void	render(Renderer *r);
};

#endif
