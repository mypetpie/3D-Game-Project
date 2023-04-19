#ifndef IBO_H
#define IBO_H
#include <glad/glad.h>
#include <vector>
#include <Info.h>

/*
	Index buffer class. Basically you just create an array of indices and then pass it to the constructor of an IBO object, which currently
	binds that array automatically in it. I'm not sure if this is supposed to be how it works or not - it might be wiser to bind and unbind it in
	main.cpp using the bind functions we already pasted in anyway. Who knows? I do not yet. 

*/

class IBO
{
public:
	GLuint ID;
	IBO(std::vector<GLuint>& indices);

	void Bind();
	void Unbind();
	void Delete();
};

#endif