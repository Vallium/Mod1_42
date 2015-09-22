/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/22 15:56:07 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/22 15:56:10 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADER_HPP
# define SHADER_HPP

# define GLEW_STATIC
# include <GL/glew.h>

class	Shader {
private:
	GLuint		_program;

public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	GLuint		getProgram();
	void		Use();
};

#endif
