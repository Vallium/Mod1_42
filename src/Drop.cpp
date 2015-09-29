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
