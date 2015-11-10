#ifndef WATERMAPRENDERER_HPP
# define WATERMAPRENDERER_HPP

# include <glm/glm.hpp>
# include <vector>

# include "Mesh.hpp"
# include "Map.hpp"
# include "WaterMap.hpp"
# include "Renderer.hpp"

class WaterMap;

class		WaterMapRenderer {
private:
	Mesh	*_mesh;
	bool	_generated;

public:
	WaterMapRenderer();
	~WaterMapRenderer();

	void	generateMesh(WaterMap *map);
	void	render(Renderer *r);
};

#endif
