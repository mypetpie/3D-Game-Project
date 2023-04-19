#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include <Graphics/VBO.h>
#include <Info.h>

/*
	This class handles the creation of Vertex Array Objects, as well as the linking of attributes to it. Basically, you start by
	creating a VAO object, then you bind it, create your vbo and your ibo, then link the vbo's layouts as attributes in the VAO
	using linkAttrib() and passing it the chosen layout number, the number of components in the layout, what type of data is in 
	the layout, the stride(or size, in bytes) of the layout (usually numComponents * sizeof(type)), and the offset, or how far away it is from
	the 0th layout. 
	
	The offset is a voidptr the byte size of a float times the number of components that come before the start of the first component in your 
	layout. So if you have an array of vertex attributes that have three positions each, and you want the 1st layout (they start at 0),
	offset = (void*) 3* sizeof(float).
*/

class VAO
{
public:

	GLuint ID;
	//generates a vertex array object
	VAO();
	//link an attribute such as color, texture coordinates, and normals to our VAO so that we can do things to them in our shader. 
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	//set VAO as the currently active VAO that we should be operating on
	void Bind();
	//remove VAO as the currently active VAO
	void Unbind();
	//delete at end of use/program 
	void Delete();
};

#endif