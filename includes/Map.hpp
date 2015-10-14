#ifndef MAP_HPP
# define MAP_HPP

# include <glm/glm.hpp>
# include <vector>

class	Map {
private:
	std::vector<glm::vec3>		_points;
	MapRenderer					*_mapRenderer;

public:
	Map();
	~Map();

	void		addPoint(glm::vec3 const &pt);
	float		interpolate(float x, float z);
	void		render(Renderer *r);
	void		generateMesh();
};

#endif
