#include <Graphics/Mesh.h>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	//create vao and bind it. DO THIS FIRST
	
	VAO.Bind();
	//create VBO, IBO for VAO1
	VBO VBO(vertices);
	IBO IBO(indices);
	//add attributes to our VAO to determine what we want our shaders to do with the info in vertices array. 
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);	//position  
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));	//normals. note layout = 1  
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));	//colors. layout = 2.  
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));	//texture. layout = 3  
	//always unbind all - prevent accidental modification 
	VAO.Unbind();
	VBO.Unbind();
	IBO.Unbind(); 
}

void Mesh::Draw(Shader& shader, Camera& camera)
{
	shader.activate();
	VAO.Bind();
	
	//textures:
	//find how many of each texture there are and then create a texture with the proper naming conventions and bind it to the object
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		//type + num stored as a c_str means our uniforms look like: diffuse0, diffuse1, etc. and specular0 specular1 etc. before they are passed to shader program
		textures[i].texUnit(shader, (type + num).c_str(), i); //send the texture type to the shader program so they get however many of the right types of textures
		textures[i].Bind();
	}
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.sendMatrix(shader, "camMatrix");

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
