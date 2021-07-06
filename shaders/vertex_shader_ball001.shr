#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 out_color;

uniform mat4 movement;

void main(void)
{
	gl_Position = movement*vec4(position, 1.0f);
	out_color = color;
}