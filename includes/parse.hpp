/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/24 19:13:39 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/24 19:13:40 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP

# define BASE_SIZE 20000

# include <vector>
# include <string>
# include <algorithm>

# define GLEW_STATIC
# include <GL/glew.h>

# include <glm/glm.hpp>

std::vector<std::string>	split(std::string str, char delimiter);
std::vector<glm::vec3>		parse(std::string name);

#endif
