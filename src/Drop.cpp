/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Drop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/25 17:31:20 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/25 17:31:23 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Drop.hpp"
#include "Context.hpp"
#include "map.hpp"
#include <iostream>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/rotate_vector.hpp>

Drop::Drop(glm::vec3 pos) : _pos(pos) {
	_velocity = glm::vec3(0, 0, 0);
}

Drop::Drop(Drop const &cpy) : _pos(cpy.getPos()), _velocity(cpy.getVelocity()) {

}

glm::vec3	Drop::getPos(void) const {
	return (_pos);
}

glm::vec3	Drop::getVelocity(void) const{
	return (_velocity);
}

void		Drop::setPos(glm::vec3 pos) {
	_pos = pos;
}

void		Drop::setVelocity(glm::vec3 v) {
	_velocity = v;
}

std::vector<Drop*>		Drop::update(Octree **dropsOctree, float dt) {
	std::vector<Drop*> drops;

	(*dropsOctree)->getPointsInsideBox(glm::vec3(0, 0, 0), glm::vec3(Context::size, Context::size, Context::size), drops);

	for (auto drop = drops.begin(); drop != drops.end(); ++drop) {
		glm::vec3 pos = (*drop)->getPos();
		glm::vec3 velocity = (*drop)->getVelocity();

		// Apply gravity
		velocity += glm::vec3(0.0f, (-9.81f * 100.0f) * dt, 0.0f);

		//Interpolate map Y
		float y1 = Context::map->interpolate(pos.x, pos.z);
		float y2 = Context::map->interpolate(pos.x + 1.0f, pos.z);
		float y3 = Context::map->interpolate(pos.x, pos.z + 1.0f);
		float y = (y1 + y2 + y3) / 3.00f;


		//Collision
		if (pos.y + velocity.y * dt <= y) {
			velocity.y = -velocity.y * 0.5f;
			velocity.x = -velocity.x * 0.5f;
			velocity.z = -velocity.z * 0.5f;

			glm::vec3	u(1, y2 - y1, 0);
			glm::vec3	v(0, y3 - y1, 1);
			glm::vec3	n = glm::normalize(glm::cross(v, u));
			velocity = glm::rotate(velocity, 180.0f, n);
		}

		if (pos.x + velocity.x * dt >= Context::size) {
			velocity.y = -velocity.y * 0.5f;
			velocity.x = -velocity.x * 0.5f;
			velocity.z = -velocity.z * 0.5f;

			glm::vec3	n = glm::vec3(-1, 0, 0);
			velocity = glm::rotate(velocity, 180.0f, n);
		}

		else if (pos.x + velocity.x * dt < 0) {
			velocity.y = -velocity.y * 0.5f;
			velocity.x = -velocity.x * 0.5f;
			velocity.z = -velocity.z * 0.5f;

			glm::vec3	n = glm::vec3(1, 0, 0);
			velocity = glm::rotate(velocity, 180.0f, n);
		}

		if (pos.z + velocity.z * dt >= Context::size) {
			velocity.y = -velocity.y * 0.5f;
			velocity.x = -velocity.x * 0.5f;
			velocity.z = -velocity.z * 0.5f;

			glm::vec3	n = glm::vec3(0, 0, -1);
			velocity = glm::rotate(velocity, 180.0f, n);
		}
		else if (pos.z + velocity.z * dt < 0) {
			velocity.y = -velocity.y * 0.5f;
			velocity.x = -velocity.x * 0.5f;
			velocity.z = -velocity.z * 0.5f;

			glm::vec3	n = glm::vec3(0, 0, 1);
			velocity = glm::rotate(velocity, 180.0f, n);
		}

		// Particles collide
		std::vector<Drop*> dropsNear;
		(*dropsOctree)->getPointsInsideBox(glm::vec3(pos.x - DROP_PHYSIC_SIZE, pos.y - DROP_PHYSIC_SIZE, pos.z - DROP_PHYSIC_SIZE), glm::vec3(pos.x + DROP_PHYSIC_SIZE, pos.y + DROP_PHYSIC_SIZE, pos.z + DROP_PHYSIC_SIZE), dropsNear);

		for (auto drop2 = dropsNear.begin(); drop2 != dropsNear.end(); ++drop2) {
			glm::vec3 pos2 = (*drop2)->getPos();
			if (*drop != *drop2) {
				glm::vec3       diffBefore(pos2.x - pos.x, pos2.y - pos.y, pos2.z - pos.z);
				glm::vec3       diffAfter(pos2.x - pos.x + velocity.x * dt, pos2.y - pos.y + velocity.y * dt, pos2.z - pos.z + velocity.z * dt);

				float	dist = sqrt(diffAfter.x * diffAfter.x + diffAfter.y * diffAfter.y + diffAfter.z * diffAfter.z);

				if (dist < DROP_PHYSIC_SIZE) {
					velocity.y = -velocity.y;
					velocity.x = -velocity.x;
					velocity.z = -velocity.z;

					velocity = glm::rotate(velocity, 180.0f, diffBefore);
				}
			}
		}

		//Apply friction to velocity
		velocity.x = velocity.x - (velocity.x * 0.1f * dt); // Friction
		velocity.y = velocity.y - (velocity.y * 0.1f * dt); // Friction
		velocity.z = velocity.z - (velocity.z * 0.1f * dt); // Friction

		//Set new pos and velocity
		(*drop)->setPos(pos + velocity * dt);
		(*drop)->setVelocity(velocity);


	}
	return drops;
}
