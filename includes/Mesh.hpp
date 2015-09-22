/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/22 15:47:07 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/22 15:47:08 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESH_HPP
# define MESH_HPP

# define GLEW_STATIC
# include <GL/glew.h>

# include <GLFW/glfw3.h>

# define GLM_FORCE_RADIANS
# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>

# include <vector>

# include "Renderer.hpp"

class		Mesh {
private:
	GLuint	_VAO;
	GLuint	_VBO;

	std::vector<GLfloat>	_vertices;

public:
	Mesh();

	void	generate(int **map);
	void	render(Renderer *renderer);
};

#endif
