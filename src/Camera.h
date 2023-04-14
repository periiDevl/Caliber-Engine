#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"

class Camera
{
public:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	glm::vec3 trackballPos;
	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;
	bool run = false;

	glm::vec3 getDirection(glm::vec3 direction, bool vertical)
	{
		if (vertical)
		{
			direction = glm::vec3(direction.x, 0, direction.z);
		}
		if (glm::length(direction) > 0)
		{
			direction = glm::normalize(direction);
		}
		return direction;
	}

	// Stores the width and height of the window
	int width = 1920;
	int height = 1080;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 0.1f;
	float sensitivity = 100.0f;
	
	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position);
	// Define a scroll wheel callback function that updates the camera position
	// Updates the camera matrix to the Vertex Shader
	void updateMatrix3D(float FOVdeg, float nearPlane, float frPlane);
	void updateMatrix2D(float scale, float nearPlane, float farPlane);
	// Exports the camera matrix to a shader
	void Matrix(Shader& shader, const char* uniform);
	// Handles camera inputs
	void Trackaballmovement(GLFWwindow* window, float normalSpeed, float highSpeed);
	void Inputs(GLFWwindow* window, float ctrlSpeed, float norSpeed);
	void Mouse(GLFWwindow* window);
	void TrackBallMouse(GLFWwindow* window);
};
#endif