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
		velocity += glm::vec3(0.0f, (-9.81f * Context::size / 150.0f) * dt, 0.0f);

		//Get map X and Z
		int x = static_cast<int>(pos.x);
		int z = static_cast<int>(pos.z);
		if (x < 0) x = 0;
		else if (x >= Context::size) x = Context::size - 1;
		if (z < 0) z = 0;
		else if (z >= Context::size) z = Context::size - 1;

		//Interpolate map Y
		float y1 = static_cast<float>(Context::map[x][z]);
		float y2 = x + 2 < Context::size ? static_cast<float>(Context::map[x + 2][z]) : 0;
		float y3 = z + 2 < Context::size ? static_cast<float>(Context::map[x][z + 2]) : 0;
		float y = (y1 + y2 + y3) / 3.00f;


		//Collision
		if (pos.y + velocity.y * dt <= y) {
			velocity.y = -velocity.y * 0.5f;
			velocity.x = -velocity.x * 0.5f;
			velocity.z = -velocity.z * 0.5f;
			if (pos.y + velocity.y < y) {
				glm::vec3	u(1, y2 - y1, 0);
				glm::vec3	v(0, y3 - y1, 1);
				glm::vec3	n = glm::normalize(glm::cross(v, u));
				velocity = glm::rotate(velocity, 180.0f, n);
			}
		}

		//Apply friction to velocity
		velocity.x = velocity.x - (velocity.x * 0.1f * dt); // Friction
		velocity.y = velocity.y - (velocity.y * 0.1f * dt); // Friction
		velocity.z = velocity.z - (velocity.z * 0.1f * dt); // Friction

		//Set new pos and velocity
		(*drop)->setPos(pos + velocity * dt);
		(*drop)->setVelocity(velocity);

		// if (pos.y < y or pos.x < 0 or pos.z < 0 or pos. x >= Context::size or pos.z >= Context::size) {
		// 	glm::vec3	n;
		//
		// 	velocity.y = -velocity.y * 0.5f;
		// 	velocity.x = -velocity.x * 0.5f;
		// 	velocity.z = -velocity.z * 0.5f;
		// 	if (pos.y + velocity.y < y) {
		// 		glm::vec3	u(1, y2 - y1, 0);
		// 		glm::vec3	v(0, y3 - y1, 1);
		// 		n = glm::normalize(glm::cross(v, u));
		// 		velocity = glm::rotate(velocity, 180.0f, n);
		// 		if (pos.y + velocity.y < y) {
		// 			pos.y = y;
		// 		}
		// 	}
		//
		// 	if (pos.x >= Context::size) {
		// 		pos.x = Context::size - 1;
		// 		n = glm::vec3(-1, 0, 0);
		// 		velocity = glm::rotate(velocity, 180.0f, n);
		// 	}
		// 	else if (pos.x < 0) {
		//
		// 		pos.x = 0;
		// 		n = glm::vec3(1, 0, 0);
		// 		velocity = glm::rotate(velocity, 180.0f, n);
		// 	}
		//
		// 	if (pos.z >= Context::size) {
		// 		pos.z = Context::size - 1;
		// 		n = glm::vec3(0, 0, -1);
		// 		velocity = glm::rotate(velocity, 180.0f, n);
		// 	}
		// 	else if (pos.z < 0) {
		//
		// 		pos.z = 0;
		// 		n = glm::vec3(0, 0, 1);
		// 		velocity = glm::rotate(velocity, 180.0f, n);
		// 	}
		// 	(*drop)->setVelocity(velocity);
		// }
		// (*drop)->setPos(pos + velocity);

		// std::vector<Drop*> dropsNear;
		//
		// (*dropsOctree)->getPointsInsideBox(glm::vec3(pos.x - DROP_PHYSIC_SIZE, pos.y - DROP_PHYSIC_SIZE, pos.z - DROP_PHYSIC_SIZE), glm::vec3(pos.x + DROP_PHYSIC_SIZE, pos.y + DROP_PHYSIC_SIZE, pos.z + DROP_PHYSIC_SIZE), dropsNear);
		// for (auto drop2 = dropsNear.begin(); drop2 != dropsNear.end(); ++drop2) {
		// 	glm::vec3 pos2 = (*drop2)->getPos();
		// 	if (pos != pos2) {
		// 		glm::vec3       diff(pos2.x - pos.x, pos2.y - pos.y, pos2.z - pos.z);
		//
		// 		float	dist = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
		// 		if (dist < DROP_PHYSIC_SIZE) {
		// 			// std::cout << dist << std::endl;
		// 			glm::vec3	velocity2 = (*drop2)->getVelocity();
		// 			velocity.x = (velocity.x + velocity2.x) / 2 * DROP_COLLIDE_COEF;
		// 			velocity.y = (velocity.y + velocity2.y) / 2 * DROP_COLLIDE_COEF;
		// 			velocity.z = (velocity.z + velocity2.z) / 2 * DROP_COLLIDE_COEF;
		//
		// 			diff = glm::normalize(diff);
		//
		// 			pos.x -= diff.x * (DROP_PHYSIC_SIZE - dist);
		// 			pos.y -= diff.y * (DROP_PHYSIC_SIZE - dist);
		// 			pos.z -= diff.z * (DROP_PHYSIC_SIZE - dist);
		// 		}
		// 	}
		// }
		// (*drop)->setVelocity(velocity);
		// (*drop)->setPos(pos);
	}
	return drops;
	// Octree		*tmpPtr;
	// tmpPtr = *dropsOctree;
	// delete tmpPtr;
	//
	// Octree::count = 0;
	// *dropsOctree = new Octree(glm::vec3(Context::size / 2, Context::size / 2, Context::size / 2), glm::vec3(Context::size / 2, Context::size / 2, Context::size / 2));
	//
	// for (auto it = drops.begin(); it != drops.end(); ++it) {
	// 	std::cout << Octree::count << std::endl;
	// 	(*dropsOctree)->insert(*it);
	// }

	// *dropsOctree = tmpOctree;

}
