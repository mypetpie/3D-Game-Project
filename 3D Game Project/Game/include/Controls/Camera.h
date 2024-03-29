#ifndef CAMERA_H
#define CAMERA_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <Graphics/Shader.h>

//Note: When you move the "view" matrix, you aren't really changing the location of the camera, but the whole world around the camera...

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);	//direction camera faces. toward negative Z, which is backwards and not forwards lol.
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	bool firstClick = true;

	int width;
	int height;

	float speed = 0.25f; //.001
	float sensitivity = 10.0f; //on the off chance there are two mouses plugged in, use primary mouse. shit gets weird if you dont. .	

	Camera(int width, int height, glm::vec3 position);

	//compute camera matrix. nearPlane: how close before things start culling, farPlane: how far away before things start culling.
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane); 
	void sendMatrix(Shader& shader, const char* uniform); //send matrices to shader. this is in its own seperate function so tht we can calculate the perspective of multiple objects in scene 
	void Inputs(GLFWwindow* window);
};

#endif // !CAMERA_H