#ifndef SPHERE_HPP
# define SPHERE_HPP

# include <vector>

# define GLEW_STATIC
# include <GL/glew.h>

# include <glm/glm.hpp>

std::vector<GLfloat>	generateSphere(float radius, int rings,int slices);

#endif
