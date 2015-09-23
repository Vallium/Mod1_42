/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/22 16:03:20 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/22 16:03:22 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

# define GLM_FORCE_RADIANS
# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <string>
# include <vector>
// # include <SOIL/SOIL.h>

# define GLEW_STATIC
# include <GL/glew.h>

# define CAMERA_SPEED 20

enum Direction {UP, DOWN, FORWARD, BACKWARD, LEFT, RIGHT, BOOST_PLUS, BOOST_MOINS};

class Camera {
public:
	glm::vec3	_pos;
	glm::vec3	_front;
	glm::vec3	_up;

	GLfloat		_yaw;
	GLfloat		_pitch;
	GLfloat		_zoom;

	GLfloat		_speed;

	Camera(glm::vec3 p);

	void		ProcessKeyboard(enum Direction dir, GLfloat dt);
	void		ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset);
	glm::mat4	GetViewMatrix();
};

#endif
