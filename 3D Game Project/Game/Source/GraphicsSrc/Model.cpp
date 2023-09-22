#include <Graphics/Model.h>


void Model::Draw(Camera& camera, glm::vec3 lightPos, glm::vec4 lightCol)
{
	

	shaderProgram.activate();
	//inform shader of model matrix values
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelM));
	shaderProgram.setVec4("lightColor", lightCol.x, lightCol.y, lightCol.z, lightCol.w);
	shaderProgram.setVec3("pointLights[0].position", lightPos.x, lightPos.y, lightPos.z); //this must be done for eachc point light in scene. i.e. pointlights[1] pointlights[2] etc.

	//draw object
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shaderProgram, camera);


}

void Model::setPos(glm::vec3 pos)
{
	modelPos = pos;
	modelM = glm::translate(modelM, modelPos);
}

void Model::delModelSh()
{
	shaderProgram.Delete();
}

void Model::loadModel(std::string path)
{
	//declare importer object
	Assimp::Importer import;
	//read the file, and if the model does not consist entirely of triangles make it so, and then flip the UVs where necessary to match openGL's winding order 
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); //note that it took the engine 48 seconds to do this when we used the backpack...

	//if the scene doesn't exist or there is something wrong with it like the scene is incomplete or missing a root node, throw an error.
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::wclog << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	//process all the node's meshes (if there are any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	//do the same for its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	//process vertex positions, normals, and tex coords
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		//declare helper vector since assimp uses its own vector class that won't implicitly convert. must transfer the positions
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		//normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}
		//tex coordinates. similar to above processes.
		if (mesh->mTextureCoords[0]) //does mesh have tex coords?
		{
			glm::vec2 vec;
			//a vertex can contain up to 8 diff tex coords. Make the assumption that we won't use models where
			//a vertex can have multiple texture coords, so always take first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texUV = vec;
			/*tangent:						this is for subsurface lighting shit like bumpiness and all that.our mesh vertex struct doesn't have these, but we can add them with relative ease i believe .
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = vector;
			//bitangent:
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.bitangent = vector;			*/
		}
		else
			vertex.texUV = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	//go through each of the mesh's faces (triangles) and retrieve corresponding vertex indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		//retrieve all indices of face and push back to indices vector. thor bless push_back and the c++ standard library 
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	//Materials. Retrieve the material object from scene's materials array, then load the mesh's diffuse/specular textures. 
	if (mesh->mMaterialIndex > 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTex(material, aiTextureType_DIFFUSE, "diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		//std::vector<Texture> specularMaps = loadMaterialTex(material, aiTextureType_SPECULAR, "specular");
		//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTex(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false; 
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.ID = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); //add to loaded textures
		}
	}
	return textures;
}

unsigned int Model::TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, numComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &numComponents, 0);
	if (data)
	{
		GLenum format = 3;
		if (numComponents == 1)
			format = GL_RED;
		else if (numComponents == 3)
			format = GL_RGB;
		else if (numComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		OGL3D_INFO("Model.cpp | texture loaded at path: " << path << " 0u0" << std::endl);
		stbi_image_free(data);
	}
	else
	{
		OGL3D_ERROR("0n0 Model.cpp: texture failed to load at path: " << path << std::endl);
		stbi_image_free(data);
	}

	return textureID;
}
