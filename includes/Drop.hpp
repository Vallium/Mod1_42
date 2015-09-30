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

# define DROP_RENDER_SIZE 0.1f
# define DROP_RENDER_DEFINITION 4
# define DROP_PHYSIC_SIZE 0.05f

# include <vector>

class	Drop {
private:
	glm::vec3	_pos;
	glm::vec3	_velocity;

public:
	Drop(glm::vec3 pos);

	glm::vec3	getPos(void) const;
	glm::vec3	getVelocity(void) const;

	void		setPos(glm::vec3 pos);
	void		setVelocity(glm::vec3 v);

	static void		update(std::vector<Drop> *drops, float dt);
};

#endif
