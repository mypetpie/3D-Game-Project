#version 330 core

//output color in RGBA (will be greyscale i think?)
out vec4 fragColor;

//take in current position
in vec3 curPos;
in float height;

in vec2 texCoords;

uniform sampler2D diffuse0;

vec4 direcLight()
{
	return texture(diffuse0, texCoords);
}


void main()
{
	float h = (height + 16)/32.0f;	// shift and scale the height into a grayscale value
	//vec4 terrainCol = texture(diffuse0, texCoords);
	fragColor = direcLight();
}