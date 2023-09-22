#include <Controls/Camera.h>

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Camera::position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	//initialize matrices
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	//position from which you are looking, the target you are looking at, and the up vector.
	view = glm::lookAt(position, position + orientation, up);	//lookAt is a matrix function that rotates/positions something to point toward another point in space from a point in space
	//FOV, aspect ratio, nearest visible point, furthest visible point. Sets the perspective of our matrixes. 
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);
	cameraMatrix = projection * view; //note: matrix multiplication is not communitive. 
}

void Camera::sendMatrix(Shader& shader, const char* uniform)
{
	//export the matrix to vertex shader. tells openGL of the location of camera matrix so that the uniforms in shader can use it:
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window)	
{
	//position += speed * orientation, or in the case of left and right, pos += speed * right vec, which is cross vector of orientation and up
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += speed * orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position += speed * -glm::normalize(glm::cross(orientation, up));	//normalized bc, remember, the view matrix coords are local. 
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position += speed * -orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += speed * glm::normalize(glm::cross(orientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //go up
	{
		position += speed * up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) //go down
	{
		position += speed * -up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) //lil sprint hehe
	{
		speed = 1.00f; //.004
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.25f; //.001
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) //if left mouse press, change orientation of camera according to mouse pos
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		//these variables are the rotation of the orientation vector. half the height is subtracted from them, and then they are divided by the height so that the coords are normalized and start at screen center
		//then they are transformed into degrees
		float rotx = sensitivity * (float)(mouseY - (height / 2)) / height;
		float roty = sensitivity * (float)(mouseX - (height / 2)) / height;
		//pre-emptively calculate the new position of the orientation around the x axis
		glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotx), glm::normalize(glm::cross(orientation, up)));

		//figure out whether or not the next rotation is within an acceptable boundary 
		if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			orientation = newOrientation;
		}

		//rotate left and right
		orientation = glm::rotate(orientation, glm::radians(-roty), up);

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) //if left mouse released, show mouse
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}

}
