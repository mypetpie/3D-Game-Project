#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
	// Outputs the coords of all vertices according to camera and model matrix
	gl_Position = camMatrix * model * vec4(aPos, 1.0);
}