/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/24 19:12:48 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/24 19:12:50 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>


std::vector<std::string>		split(std::string str, char delimiter) {
	std::vector<std::string>		internal;
	std::stringstream		ss(str);
	std::string				tok;

	while(getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}
	return internal;
}

std::vector<glm::vec3>		parse(std::string name) {
	std::ifstream			file;
	std::string				line;
	std::vector<glm::vec3>	out;

	file.open(name.data());
	while (std::getline(file, line)) {
		std::vector<std::string> pts = split(line, ' ');
		for (auto pt = pts.begin(); pt != pts.end(); ++pt) {
			std::string tmp = pt->substr(1, pt->size() - 2);

			std::vector<std::string> coords = split(tmp, ',');

			out.push_back(glm::vec3(stof(coords[0]), stof(coords[2]), stof(coords[1])));
		}
	}
	file.close();
	return out;
}
