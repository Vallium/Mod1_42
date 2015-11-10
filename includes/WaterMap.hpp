#ifndef WATERMAP_HPP
# define WATERMAP_HPP

# define WATERMAP_RESOLUTION 100
# define WATERMAP_VERTICAL_RESOLUTION 1000.0f

# include "WaterMapRenderer.hpp"
# include "Map.hpp"
# include "Renderer.hpp"

class	WaterMapRenderer;

class	WaterMap {
private:
	float				**_waterMap;
	float				**_landMap;
	WaterMapRenderer	*_wmRenderer;

public:
	WaterMap(Map *map);
	~WaterMap();

	float	getWaterHeight(float x, float y);

	void	addWater(float x, float y, float amount);
	void	update(float dt);
	void	render(Renderer *r);
	void	generateMesh();
};

#endif
