#ifndef  VBO_H
#define VBO_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include <Info.h>

/*
	Vertex buffer class. This is where the vertex buffer is created and then bound. The constructor handles the creation/generation and then we bind it,
	or tell openGL to use it, using Bind(). To create a VBO, create a VBO object and then pass to its constructor a pointer to the location of your vertices array,
	as well as its size in bytes (usually by doing sizeof(vertices)). Then all you need to do is bind it when you need to use it, then unbind it when you're done. 
	The VAO does this every time a new VAO is created - it binds the VBO, then uses glVertexAttribPointer to know what to do with it, then unbinds it when the VAO is
	created. 

	At the end of its use, it can be deleted from memory using Delete()
*/

//basically just abstracts the information for each point that we had previously hardcoded in our VBO. 
struct Vertex
{
	
	glm::vec3 position; // position in world space
	glm::vec3 normal;   // orientation of the surface of the vertex. Defined as the vector which is perpendicular to a plane that is tangent to the surface at position.
	glm::vec3 color; 
	glm::vec2 texUV;
};

class VBO
{
public: 
	GLuint ID;		//this variable basically just holds the memory for our vbo
	VBO(std::vector<Vertex>& vertices);

	void Bind();
	void Unbind();
	void Delete();
};

#endif // ! VBO_H




