#ifndef MESH_H
#define MESH_H

#include <string>

#include <Graphics/VAO.h>
#include <Graphics/IBO.h>
#include <Controls/Camera.h>
#include <Graphics/Texture.h>

/*
	This class wraps up all of the vertices information, indices information, and texture information and operation that we hae
	thus far done mostly in int main. Basically, this class is the actual object that is going to be created when a model is loaded.
	To create a mesh, simply load a mesh object and pass to its constructor the location of your vertices array, your indices array, 
	and your texture array. Then, initialize its model matrix and give it a position in world space using glm::vec3. After that, all
	you need to do is pass that position as well as lighting info such as color and positions of any light objects to the shader of 
	your mesh object. After that, you should see a fully constructed and lit mesh in your scene. 

	Q: What is a mesh?
	A: A mesh is defined as a data set that contains all data relating to the object you are trying to create/load - i.e.
	   vertices from vbo, indices from ibo, textures, normals, etc. Rather than keeping the creation of all of these 
	   things seperate in their own classes, the mesh object will automatically create a "mesh," or the synthesis of all this data, 
	   so that we can basically save tons of time not having to write out every single vertex and manually do every step of the
	   object creation. The mesh class automates away a lot of the most low-level stuff, so that we can eventually use the same
	   steps we already did to load much more complex models instead of just cubes. It gives us a central place to store all our 
	   information surrounding an object we want to create. 

*/

class Mesh
{
public:
	//use vectors to allocate space for our incoming data - this way we dont need to know the size of the arrays coming in, because realistically we cant
	std::vector <Vertex> vertices;	//an array of vertexes - each vertex has attributes like position, color, normals, texcoords
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	//initialize VAO
	VAO VAO;

	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	void Draw(Shader& shader, Camera& camera);
};


#endif // !MESH_H



