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
	glm::vec3 modelPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 modelM = glm::mat4(1.0f);

	Shader shaderProgram = Shader("Assets/Shaders/vertDefault.vert", "Assets/Shaders/fragDefault.frag");

	Model(std::string path)
	{
		loadModel(path);
		modelM = glm::translate(modelM, modelPos);
	}

	/*
	set the position in world space and calculate the color of the light on the object, as well as its local position and the lights position. Then, draw it.
	done for every object in scene, and if you move something this process must be done first
	*/
	void Draw(Camera& camera, glm::vec3 lightPos, glm::vec4 lightCol);

	//set position of model in real-time
	void setPos(glm::vec3 pos);
	
	//Delete shader program (and at some point object data) to clear memory at end of use.
	void delModelSh();
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