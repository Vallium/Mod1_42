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

# define DROP_RENDER_SIZE 0.2f
# define DROP_PHYSIC_SIZE 0.1f

class	Drop {
private:
	glm::vec3	_pos;
	glm::vec3	_velocity;

public:
	Drop(glm::vec3 pos);

	glm::vec3	getPos(void) const;
};

#endif
