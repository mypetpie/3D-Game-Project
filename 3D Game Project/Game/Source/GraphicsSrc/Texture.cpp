#include <Graphics/Texture.h>

Texture::Texture()
{
}

Texture::Texture(const char* imagePath, const char* texType, GLuint slot, GLenum format, GLenum pixelType)
{
	//Basically the process here, like most of openGL, is to create a texture object and then fill it with your texture data and texture params.

	type = texType;
	int widthImg, heightImg, numColCh;
	//store image as an array of char. we use stbi_set_flip_on_load(true) because openGL reads images starting from bottom left, but stb starts at the top left. 
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(imagePath, &widthImg, &heightImg, &numColCh, 0);

	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + slot); //tell openGL which texture slot to put the image in. 0-16.
	unit = slot; //make the slot variable equal to the unit variable so that when the texture is binded we know which slot we should be operating on automatically.
	glBindTexture(GL_TEXTURE_2D, ID); //type, ref value
	//tell openGL what type of filtering to use on the texture:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_nearest = no filtering. GL linear = pixels are filled in based on whats near them. nearest is good for pixel art n stuff
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//tell openGL how we want the texture to repeat/load. sti = xyz axes. GL_REPEAT = repeat the image across the texture if it does not fit (i believe)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//generate texture from texture bytes: type of tex, 0, type of color channels for tex, width, height, 0, color channels in image, type of img data, data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	//generate mipmaps, or different sizes of the texture based on distance. this helps performance greatly. 
	glGenerateMipmap(GL_TEXTURE_2D);

	//delete and unbind everything once loaded
	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{

	GLuint tex0Uni = glGetUniformLocation(shader.ID, uniform);
	shader.activate();
	glUniform1i(tex0Uni, unit); //glUniform1i is what you use to load samplers. unit = which slot your texture is in
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID); //type, ref value
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0); //type, ref value
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}
