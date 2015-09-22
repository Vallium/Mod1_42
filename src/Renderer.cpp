/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/22 15:49:27 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/22 15:49:29 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Renderer.hpp"

Shader		*Renderer::getLandShader(void) {
	return _landShader;
}

void		Renderer::loadShaders(void) {
	_landShader = new Shader("shaders/land.vert", "shaders/land.frag");
}
