#ifndef SPHERE_HPP
# define SPHERE_HPP

# include <vector>

# define GLEW_STATIC
# include <GL/glew.h>

# include <glm/glm.hpp>

std::vector<GLfloat>	generateSphere(float radius, int rings,int slices);
void		generateCubeMesh(float size, GLfloat **vertexBuffer, unsigned int &vertexBufferSize, GLuint **elementBuffer, unsigned int &elementBufferSize);

#endif
