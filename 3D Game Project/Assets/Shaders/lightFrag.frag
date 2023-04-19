#version 330 core

// Output colors in RGBA
out vec4 FragColor;

uniform vec4 lightColor;

void main()
{
	FragColor = lightColor;
}