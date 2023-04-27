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
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);	//direction camera faces. toward negative Z
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	bool firstClick = true;

	int width;
	int height;

	float speed = 0.001f;
	float sensitivity = 100.0f;

	Camera(int width, int height, glm::vec3 position);

	void updateMatrix(float FOVdeg, float nearPlane, float farPlane); //compute camera matrix
	void sendMatrix(Shader& shader, const char* uniform); //send matrices to shader. this is in its own seperate function so tht we can calculate the perspective of multiple objects in scene 
	void Inputs(GLFWwindow* window);
};

#endif // !CAMERA_H