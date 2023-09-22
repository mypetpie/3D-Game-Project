/*

	This is the beginning implementation of what will eventually be a terrain mesh object that will be capable of reading and rendering 8-bit heightmaps. 
	Basically, the idea is to generate a big array of vertexes and then generate triangles between them, using an index buffer to avoid objects. Eventually, each 
	vertex will have all of the normal info a vertex has, and the big array will essentially be a mesh or a vao that reads the attribute data provided by the user (texture info and coords
	from texture map assigned to heightmap) and heightmap, which alters the height of each vertex and gives it the appearance of some kind of terrain. For more info, see the docs. 

	For now, its just an implementation of this stack overflow answer: https://stackoverflow.com/questions/5915753/generate-a-plane-with-triangle-strips
	Which is, for the sake of a fair warning, wrong for odd heights in the getIndices function 

*/

#ifndef TERRAINOBJECT_H
#define TERRAINOBJECT_H

#include <Graphics/Mesh.h>
#include <Graphics/Shader.h>

class TerrainObject
{
public:

	Shader terrainShader = Shader("Assets/Shaders/terrainVert.vert", "Assets/Shaders/terrainFrag.frag");
	int width, height, nChannels; //heightmap info

	glm::mat4 modelM = glm::mat4(1.0f);

	//initialize VAO
	VAO vao;

	const char* texturePath;

	//Take in map path, build mesh from constructed data. 
	TerrainObject(const char* mapPath, const char* texPath);
	
	//render terrain mesh
	void Draw(Camera& camera);

	//free shader program from memory at end of use
	void delTerSh();
};

#endif