#version 330 core

layout (location = 0) in vec3 aPos;

layout (location = 1) in vec3 aNormal;

layout (location = 2) in vec3 aColor;

layout (location = 3) in vec2 aTex;

//outputs normals and curPos/camera's current pos value to fragment shader for light use
out vec3 curPos;

out vec3 Normal;

// Outputs color to fragment shader
out vec3 color;

//outputs texture coordinates to fragment shader
out vec2 texCoords; 



//takes in the model matrix and cam matrix created by the camera object in main.cpp. used to out put the coordinates of each vertex in the proper persective.
uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
	//calculate current position
	curPos = vec3(model * vec4(aPos, 1.0f));
	//normals from vertex data 
	Normal = aNormal;
	//colors from vertex data
	color = aColor;
	//uv coords from vertex data
	texCoords = aTex;
	

	// Outputs the coords of all vertices in proper perspective
	gl_Position = camMatrix * model * vec4(curPos, 1.0);
}