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

Renderer::Renderer() {
}

Renderer::~Renderer() {
}


Shader		*Renderer::getLandShader(void) {
	return _landShader;
}

Shader		*Renderer::getSphereShader(void) {
	return _sphereShader;
}

void		Renderer::loadShaders(void) {
	_landShader = new Shader("shaders/land.vert", "shaders/land.frag");
	_sphereShader = new Shader("shaders/sphere.vert", "shaders/sphere.frag");
}
