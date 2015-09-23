#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color_in;

out vec3 rgb;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	rgb = color_in;
}
