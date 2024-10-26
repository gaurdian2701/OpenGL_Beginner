#version 330 core

in vec3 tColor;
out vec4 FragColor;

void main()
{
	FragColor = vec4(tColor, 1.0f);
}