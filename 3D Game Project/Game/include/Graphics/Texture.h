#ifndef TEXTURE_H
#define TEXTURE_H

/*
	This class handles all functions relating to the initializing, loading, and binding of a texture.
	The way that it does this is almost exactly the same, process-wise, as the other data objects we have created.
	First, you load the image using stbi_load(), then you generate a texture object, then you bind it and fill it with img data. 
	By default, all of our textures load in with "GL_NEAREST" filtering on, meaning there isn't any linear filtering of
	textures. They also repeat by default - meaning when it exceeds its width and height, the image will repeat. 

	To add a texture to your VBO, firstly add another layout of UV coords to your VBO. Then, simply create a texture object and pass 
	to it the filepath of your image, what type of texture (GL_TEXTURE_2D, 1D, 3D), what slot (GL_TEXTURE0-16), the color 
	format of your image (RGBA), and the type of data your pixels will be stored as (it seems that this is often GL_UNSIGNED_BYTE).
	Then call texUnit() on your texture object, and pass to it the name of your shader program, which uniform you wish to modify
	(usually the name of the sampler2D uniform in your fragment shader), and which texture unit, or slot, your texture is in. The
	function will then activate the shader program and load the coordinate information given in your texCoords layout, in 
	basically the exact same process you use to inform openGL of the connection between your indices buffer and vbo. 

	Finally, once you've added a layout, created the texture object, and told openGL where to put your texture coordinate info 
	for your texture, all you need to do is call the bind() function when it is time to be drawn. 

	note: textures MUST be in 8bpc (bits per color) RGBA format. Otherwise, they will crash the program.  
		  this is because the linkattrib function relies on a set size of bytes given to each layout, so 
		  when it goes to read the color information and its too big/not the defined size or type, it will throw an exception.
*/

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <Graphics/Shader.h>

class Texture
{
public: 
	GLuint ID;
	std::string type;	//used to be const char* 
	GLuint unit; 
	std::string path; 

	Texture();
	Texture(const char* imagePath, const char* texType, GLuint slot, GLenum format, GLenum pixelType);
	//Link texture coords to shader program's uniform. unit = which slot your texture is in.
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();		
	void Unbind();
	void Delete();
};

#endif