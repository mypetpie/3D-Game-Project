/*
* 
this is the code for a generic point light which can be created and added into a scene.Eventually the engine will be able to support at least 8 of these in a scene together.
I have yet to figure out how to abstract this process in a way that doesn't involve manually setting the number of point lights you want in the world 
before you compile and run the game. 

*/
#ifndef LIGHT_H
#define LIGHT_H

#include <Graphics/Mesh.h>
#include <Graphics/Shader.h>



class pointLight
{
public:
	
	Shader lightShader = Shader("Assets/Shaders/lightVert.vert", "Assets/Shaders/lightFrag.frag");
	std::vector <Vertex> lightVerts;
	std::vector <GLuint> lightInd;
	std::vector <Texture> tex;

	glm::vec3 modelPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 modelM = glm::mat4(1.0f);
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Mesh light = Mesh(lightVerts, lightInd, tex);

	pointLight(glm::vec3 pos, glm::vec4 col)
	{
		Vertex lightVertices[]		//these vertices make backfaces front faces and front faces backfaces. lmao 
		{
			Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
			Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},

			Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
			Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},

			Vertex{glm::vec3(0.1f,  0.1f,  0.1f)},
			Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},

			Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
			Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)}
		};

		GLuint lightIndices[]
		{
			//front
			0, 1, 2,
			0, 2, 3,

			0, 4, 7,
			0, 7, 3,

			3, 7, 6,
			3, 6, 2,

			2, 6, 5,
			2, 5, 1,

			1, 5, 4,
			1, 4, 0,
			//back
			4, 5, 6,
			4, 6, 7
		};

		Texture textures[] //old texture array for floor plane
		{
			//load textures for floor plane:			  				  
			Texture("Assets/Textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
			//Texture("Assets/Textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE) //the color channel is only red bc we only have 1 channel. white = bright black = dar

		};

		std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
		std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
		std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
		Mesh light(lightVerts, lightInd, tex);

		modelPos = pos;
		color = col;
		modelM = glm::translate(modelM, modelPos);
	}

	void setPos(glm::vec3 pos)
	{
		modelPos = pos;
		modelM = glm::translate(modelM, modelPos);
	}

	void setColor(glm::vec4 c)
	{
		color = c;
	}

	void Draw(Camera& camera)
	{
		//inform shader of neccesary light obj data - world pos, color. If you want to "animate" a light, you need to throw each of these inside the while loop and also change lightPos according to sin(time) in there too. 
		lightShader.activate();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelM));
		lightShader.setVec4("lightColor", color.x, color.y, color.z, color.w);	//these go to light fragment shader

		light.Draw(lightShader, camera);
	}

	void Delete()
	{
		lightShader.Delete();
	}
private:


};

#endif
