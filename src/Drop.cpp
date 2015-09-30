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

Drop::Drop(glm::vec3 pos) : _pos(pos) {
	_velocity = glm::vec3(0, 0, 0);
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

void		Drop::update(std::vector<Drop> *drops, float dt) {
	for (auto drop = drops->begin(); drop != drops->end(); ++drop) {
		glm::vec3 velocity = drop->getVelocity();
		velocity += glm::vec3(0.0f, -0.0981f * dt, 0.0f);
		velocity.x = velocity.x - (velocity.x * 0.2f * dt);
		velocity.y = velocity.y - (velocity.y * 0.2f * dt);
		velocity.z = velocity.z - (velocity.z * 0.2f * dt);
		drop->setVelocity(velocity);
	}

	for (auto drop = drops->begin(); drop != drops->end(); ++drop) {
		glm::vec3 pos = drop->getPos();
		pos = (pos + drop->getVelocity());
		int x = static_cast<int>(pos.x * Context::size / RENDER_SIZE);
		int z = static_cast<int>(pos.z * Context::size / RENDER_SIZE);

		float y = static_cast<float>(Context::map[x][z]) / static_cast<float>(Context::size) * RENDER_SIZE;

		if (pos.y < y)
			pos.y = y;
		drop->setPos(pos);
	}
}
