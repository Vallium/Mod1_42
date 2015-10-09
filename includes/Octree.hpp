#ifndef Octree_H
#define Octree_H

# define GLEW_STATIC
# include <GL/glew.h>

# include <GLFW/glfw3.h>

# define GLM_FORCE_RADIANS
# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>

#include <cstddef>
#include <vector>

#include "Drop.hpp"

class	Drop;

class Octree {
	// Physical position/size. This implicitly defines the bounding
	// box of this node
	glm::vec3 origin;         //! The physical center of this node
	glm::vec3 halfDimension;  //! Half the width/height/depth of this node

	// The tree has up to eight children and can additionally store
	// a point, though in many applications only, the leaves will store data.
	Octree *children[8]; //! Pointers to child octants
	Drop *data;   //! Data point to be stored at a node

	/*
			Children follow a predictable pattern to make accesses simple.
			Here, - means less than 'origin' in that dimension, + means greater than.
			child:	0 1 2 3 4 5 6 7
			x:      - - - - + + + +
			y:      - - + + - - + +
			z:      - + - + - + - +
	 */

	public:
	static unsigned int		count;

	Octree(const glm::vec3& origin, const glm::vec3& halfDimension);

	Octree(const Octree& copy);

	~Octree();

	// Determine which octant of the tree would contain 'point'
	int getOctantContainingPoint(const glm::vec3& point) const;

	bool isLeafNode() const;

	void insert(Drop* point);

	// This is a really simple routine for querying the tree for points
	// within a bounding box defined by min/max points (bmin, bmax)
	// All results are pushed into 'results'
	void getPointsInsideBox(const glm::vec3& bmin, const glm::vec3& bmax, std::vector<Drop*>& results);

};

#endif
