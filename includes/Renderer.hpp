/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Renderer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/22 15:49:46 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/22 15:49:47 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_HPP
# define RENDER_HPP

# include "Shader.hpp"

class	Renderer {
private:
	Shader		*_landShader;

public:
	Renderer();
	~Renderer();

	Shader		*getLandShader(void);
	void		loadShaders(void);
};

#endif
