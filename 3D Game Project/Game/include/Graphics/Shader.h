#ifndef SHADER_H
#define SHADER_H

/*
	This class handles the compilation and linking of our shader files to a shader program object that is created in the constructor.
	To create a new shader program, simply create a Shader object and pass to its constructor  one file path for the vertex, and one 
	file path for the fragment. Then, when you want openGL to use it, call the activate function. 
	The shader object is then deleted at the end of its use in the ENTIRE PROGRAM by calling the delete function.

	delete really only should be called once you know FOR sure that the shader program is not going to be used anywhere else. 
	Otherwise, this can lead to issues if you try to modify/use a shader program that has already been deleted. 
*/

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <vector>
#include <Info.h>

std::string get_file_contents(const char* filepath);		//returns file contene

class Shader
{
public:
	GLuint ID;
	//create shader program using our vertex and fragment shaders
	Shader(const char* vertexFilePath, const char* fragmentFilePath);
	//activate, or "use" the shader program object. should be called before basically all actions that send information to the shader. 
	void activate();
	//destroy program at end of use
	void Delete();
	//catches compiler errors within the shader and outputs them to the console. Goes 0u0 if successful
	void getShaderCompileErr(unsigned int shader, const char* filepath);

	//handy functions for setting the values of certain types of uniforms. Greatly reduces how much noise we have to sift thru in main. Also makes it easier to understand. 
	void setFloat(const char* uniform, GLfloat somefloat);
	void setVec3(const char* uniformVec, GLfloat x, GLfloat y, GLfloat z);
	void setVec4(const char* uniformVec, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
};

#endif // !SHADER_H
