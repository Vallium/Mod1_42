/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/22 16:05:39 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/22 16:05:41 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

Camera::Camera(glm::vec3 p)
{
	_pos = p;
	_front = glm::normalize(glm::vec3(10.0f, 5.0f, 10.0f) - _pos);
	_up = glm::vec3(0.0f, 1.0f,  0.0f);

	_yaw = 0.0f;
	_pitch = 0.0f;
	_zoom = 45.0f;

	_speed = 20.0f;
}

void	Camera::ProcessKeyboard(enum Direction dir, GLfloat dt)
{
	switch (dir) {
		case UP:
			_pos += dt * _speed * _up;
			break;
		case DOWN:
			_pos -= dt * _speed * _up;
			break;
		case FORWARD:
			_pos += dt * _speed * _front;
			break;
		case BACKWARD:
			_pos -= dt * _speed * _front;
			break;
		case LEFT:
			_pos -= glm::normalize(glm::cross(_front, _up)) * dt * _speed;
			break;
		case RIGHT:
			_pos += glm::normalize(glm::cross(_front, _up)) * dt * _speed;
			break;
		case BOOST_PLUS:
			_speed++;
			break;
		case BOOST_MOINS:
			_speed--;
			break;
	}
	_front = glm::normalize(glm::vec3(10.0f, 5.0f, 10.0f) - _pos);
}

void	Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset)
{
	GLfloat sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	_yaw   += xoffset;
	_pitch += yoffset;

	if(_pitch > 89.0f)
		_pitch = 89.0f;
	if(_pitch < -89.0f)
		_pitch = -89.0f;

	glm::vec3 tmp;
	tmp.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	tmp.y = sin(glm::radians(_pitch));
	tmp.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(tmp);
}

glm::mat4	Camera::GetViewMatrix()
{
	glm::mat4 view = glm::lookAt(_pos, _pos + _front, _up);
	return view;
}
