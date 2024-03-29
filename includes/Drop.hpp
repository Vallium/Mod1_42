/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Drop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/25 17:31:27 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/25 17:31:30 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DROP_HPP
# define DROP_HPP

# include <glm/glm.hpp>

# define DROP_RENDER_SIZE 150.0f
# define DROP_RENDER_DEFINITION 4
# define DROP_PHYSIC_SIZE 150.0f
# define DROP_COLLIDE_COEF 1.0f

# include <vector>

# include "Octree.hpp"

class	Octree;

class	Drop {
private:
	glm::vec3	_pos;
	glm::vec3	_velocity;

public:
	Drop(glm::vec3 pos);
	Drop(Drop const &cpy);

	glm::vec3	getPos(void) const;
	glm::vec3	getVelocity(void) const;

	void		setPos(glm::vec3 pos);
	void		setVelocity(glm::vec3 v);

	static std::vector<Drop*>		update(Octree **dropsOctree, float dt);
};

#endif
