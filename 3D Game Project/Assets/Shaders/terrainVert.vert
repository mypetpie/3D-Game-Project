#version 330 core
/* 
When rendering, we'll pass the y coordinate of our vertex from the vertex shader to the fragment shader. 
In the fragment shader, we'll then normalize this value (using the reverse of shift & scale from terrainObject.cpp) to convert it into a grayscale value (for now).
*/


//take in layout attributes.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

//output current camera position. view is rendered based on aPos.
out vec3 curPos;
//height (for color)
out float height;

//tex coords out for actual color delete above
out vec2 texCoords;

//takes in the model matrix and cam matrix created by the camera object in main.cpp. used to out put the coordinates of each vertex in the proper persective.
uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
	height = aPos.y;
	texCoords = aTexCoord;
	//Calculate current position
	curPos = vec3(model * vec4(aPos, 1.0f));


	//Output coords of all vertices in proper perspective
	gl_Position = camMatrix * model * vec4(aPos, 1.0);
}