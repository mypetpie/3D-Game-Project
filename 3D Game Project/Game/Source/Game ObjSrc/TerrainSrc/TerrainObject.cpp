#include <Game Objects/Terrain/TerrainObject.h>

TerrainObject::TerrainObject(const char* mapPath, const char* texPath)
{
	//glFrontFace(GL_CW);
	//allocate space for incoming data.
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	//set texture's image path for use in draw call. 
	TerrainObject::texturePath = texPath;

	//load heightmap texture
	unsigned char* data; //we want our heightmap interpreted as an 8bit data structure because heightmaps are an 8-bit image. 
	data = stbi_load(mapPath, &width, &height, &nChannels, 0);

	//check to see image loaded properly
	if (data)
	{
		OGL3D_INFO("TerrainObject.cpp | " << mapPath << " loaded successfully 0u0");
	}
	else
	{
		OGL3D_ERROR("TerrainObject.cpp | " << mapPath << " did not load successfully 0n0")
	}

	//generate vertices
	float yScale = 64.0f / 256.0f, yShift = 16.0f; //Apply shift+ scale
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			//get texel for (i,j) tex coord on image
			unsigned char* texel = data + (j + width * i) * nChannels;
			//get raw height at coord:
			unsigned char y = texel[0];

			
			//calculate vertex position:
			//helper vertex
			Vertex vertex;
			glm::vec3 vertHelp;

			vertHelp.x = (-height / 2.0f + i);			// This is how far the map will span in the x direction (left-right)
			vertHelp.y = ((int)y * yScale - yShift);	// This gives our mesh the correct height
			vertHelp.z = -(-width / 2.0f + j / 2.0f);	// This is how far the map will span in the z direction (forward-back). Z must be inverted. 
			vertex.position = vertHelp;

			//calculate texture coordinates:
			glm::vec2 uvHelp;
			uvHelp.x = i / (float)width;
			uvHelp.y = (j+1) / (float)height; 
			vertex.texUV = uvHelp;

			//normals, color, and texUVs need to be added here. but once you do those:
			vertices.push_back(vertex);
		}
	}


	//generate indices
	for (unsigned int i = 0; i < height; i++) //for each strip
	{
		for (unsigned int j = 0; j < width; j++) //for each column
		{
			for (unsigned int k = 0; k < 2; k++)
			{
				indices.push_back(j + width * (i + k));
			}
		}
	}

	//create vbo, ibo, link attribs to vao
	vao.Bind();
	VBO terrainVBO(vertices);
	IBO terrainIBO(indices);

	vao.LinkAttrib(terrainVBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); //position. layout = 0
	vao.LinkAttrib(terrainVBO, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float))); //UV coords. For now this is layout 1 in terrain. 

	/*
		link other attribs:
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));	//normals. note layout = 1
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));	//colors. layout = 2.
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));	//texture. layout = 3
	*/


	// Free everything from memory
	stbi_image_free(data);
	vao.Unbind();
	terrainVBO.Unbind();
	terrainIBO.Unbind();
}

void TerrainObject::Draw(Camera& camera)
{
	const unsigned int NUM_STRIPS = height - 1;
	const unsigned int NUM_VERTS_PER_STRIP = width * 2;

	//load texture
	Texture mapTexture = Texture(texturePath, "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);

	// Draw mesh:
	terrainShader.activate();
	vao.Bind();

	mapTexture.texUnit(terrainShader, "diffuse", 0); //send the texture type to the shader program so they get however many of the right types of textures
	mapTexture.Bind();

	glUniformMatrix4fv(glGetUniformLocation(terrainShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelM));
	glUniform3f(glGetUniformLocation(terrainShader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z); //send camera info to shader to calculate light and shit. 
	camera.sendMatrix(terrainShader, "camMatrix");

	//FUCKLOAD of draw calls. Literally not possible for game
	for (unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
	{
		glDrawElements(GL_TRIANGLE_STRIP, NUM_VERTS_PER_STRIP, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * NUM_VERTS_PER_STRIP * strip));
	}
	mapTexture.Delete();
}

void TerrainObject::delTerSh()
{
	terrainShader.Delete();
}
