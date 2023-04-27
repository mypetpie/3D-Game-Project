#include <Graphics/Shader.h>


std::string get_file_contents(const char* filepath)
{
	std::string contents;
	std::ifstream shaderStream(filepath);	//open file
	if (shaderStream.is_open())
	{
		//create a stream of strings
		std::stringstream sstr;
		//read contents of shaderStream into string stream.
		sstr << shaderStream.rdbuf();
		//convert to string
		contents = sstr.str();
		//close file
		shaderStream.close();
		return(contents);
	}
	else //maybe this should throw an exception? Im not entirely sure I want the program to try and continue if there's no shader loaded. 
	{
		OGL3D_WARNING("Shader.cpp | " << filepath << " not found OnO");
	}



}


Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath)
{
	std::string vertexCode = get_file_contents(vertexFilePath);
	std::string fragmentCode = get_file_contents(fragmentFilePath);

	//convert our string of code into character arrays of code. vertexSource = the actual array of code being put in the "glshadersource" function 
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	//create vertex shader, then inform compiler of the shader source's location, then compile the shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	getShaderCompileErr(vertexShader, vertexFilePath);

	//same:
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	getShaderCompileErr(fragmentShader, fragmentFilePath);

	//create program that holds our shaders nd attach shaders to it
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);		//tell openGL to use created shader program

	//delete since they're already in program:
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::getShaderCompileErr(unsigned int shader, const char* filepath)
{
	GLint logLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		std::vector<char> errorMessage(logLength); //get the info log to retrieve the log message that contains the error
		glGetShaderInfoLog(shader, logLength, NULL, &errorMessage[0]);
		OGL3D_WARNING("Shader.cpp | " << filepath << " compiled with errors : " << std::endl << &errorMessage[0]);
		return;
	}
	else
	{
		OGL3D_INFO("Shader.cpp | " << filepath << " compiled successfully 0u0")
	}
}

void Shader::setFloat(const char* uniform, GLfloat somefloat)
{
}

void Shader::setVec3(const char* uniformVec, GLfloat x, GLfloat y, GLfloat z)
{
	glUniform3f(glGetUniformLocation(ID, uniformVec), x, y, z);

}

void Shader::setVec4(const char* uniformVec, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	glUniform4f(glGetUniformLocation(ID, uniformVec), x, y, z, w);
}
