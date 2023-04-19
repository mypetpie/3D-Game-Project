#include <Graphics/VAO.h>

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

/*
									Q: What does linkAttrib do?
					A: Basically the same thing as glVertexAttribPointer but it also binds. 

	First, we bind the VBO we want to draw our attributes from. 
	Next, we describe the layout locations. In this case, position 0 = coordinates/position, and position 1 = color. 
	Next, we describe how many components per layout. 3 in both cases here, since pos is xyz and color is rgb.
	Then, the type of the components, in the case of xyzrgb it is GLfloat. 
	Next, the function takes in a variable for the stride of your vertex, i.e. the distance in bytes from beginning of vert to end of vert. sizeof type * numcomponents
	Finally, it applies an offset which tells us what layout position we are linking. 

	After that, the layout is enabled, and the VBO is unbound.

	Note: If you want just the coordinates, then I think you have to multiply by the color offset. And to get just color you have to multiply by coord offset. 
		  This doesn't matter here, since xyz and rgb positions are 3 float components, but it might matter somewhere else down the line. 
		  
		  For any offset beyond 0, it will be numComponents after 0 * sizeof type, e.g. xyzrgb color offset would be 3*sizeof(GLfloat)

	x  y  z  r  g  b		r  g  b  x  y  z
	|color |	   |		|coords|	   |
	|offset|	   |	    |offset|	   |
	|			   |		|			   |
	|			   |		|			   |
	| coords stride|		| color stride |
*/

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);		//vertex attribute is a way of communicating with the vertex shader
	glEnableVertexAttribArray(layout); //tells us which layout to use in VAO 
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}
