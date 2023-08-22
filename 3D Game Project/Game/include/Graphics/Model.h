#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Graphics/Mesh.h>
#include <Graphics/Shader.h>

#include <string>
#include <fstream>
#include <iostream>
#include <map>

class Model
{
private:
	std::vector<Texture> textures_loaded;
public:
	Model(std::string path) 
	{
		loadModel(path);
	}

	void Draw(Shader& shader, Camera& camera);

private:
	//model data
	std::vector<Mesh> meshes;
	std::string directory;

	//use assimp to load the model into a data structure from assimp called a scene object. 
	void loadModel(std::string path);
	//tail recursive function which retrieves the mesh indicese, then retrieves each mesh, and then does that again for all of the node's child nodes. We learned abt this in school!!
	void processNode(aiNode* node, const aiScene* scene);
	//3-step process: retrieve all vertex data, retrieve indices, and then retrieve relevant material data. 
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	//checks all material textures of a certain type and then loads the textures if they are not loaded. 
	std::vector<Texture> loadMaterialTex(aiMaterial* mat, aiTextureType type, std::string typeName);
	//helper for above function
	unsigned int TextureFromFile(const char* path, const std::string& directory);

};

#endif // !MODEL_H