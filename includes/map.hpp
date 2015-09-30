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

#ifndef MAP_HPP
# define MAP_HPP

# define BASE_SIZE 20000
# define MIN_TAB_SIZE 500
# define RENDER_SIZE 100.0f

# include <vector>

# define GLEW_STATIC
# include <GL/glew.h>

# include <glm/glm.hpp>

std::vector<GLfloat>		generateMesh(float **map, int size);
std::vector<std::string>	split(std::string str, char delimiter);
std::vector<int>			parse(std::string name);
int							pgcd(int a, int b);
int							ppcm(int a, int b);
float						**create_tab(std::vector<int> &pts, int &size);
void 						hill(float **map, int size, int x, int y, int zin);
void						errect_hills(float **tab, int size, std::vector<int> pts);

#endif
