#pragma once
#include "render/window.h"

struct cameraObject
{
	GLFWwindow* window;
	shared_ptr<shaderObject> shader;
	vec3 position = vec3(1, -1, 6);
	vec3 lookatDirection = vec3(0, 0, -1);
	vec3 up = vec3(0, 1, 0);
	const float sensitivity = 0.05f;
	const float cameraSpeed = 4;
	int width = 800;
	int height = 600;

	mat4 projectionMatrix = perspective(70, (float)width / (float)height, 0.1f, 50.0f);

	// mouselook variables
	float yaw = -90;
	float pitch = 0;
	bool firstUpdate = true;
	float lastX = 800 * 0.5;
	float lastY = 600 * 0.5;
	
	// constructor
	cameraObject(GLFWwindow* window, shared_ptr<shaderObject> shader) : window(window), shader(shader) {}

	void update(float const& deltaTime)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			position += lookatDirection * cameraSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			position -= lookatDirection * cameraSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			position += normalize(cross(lookatDirection, up)) * cameraSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			position -= normalize(cross(lookatDirection, up)) * cameraSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			position += up * cameraSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			position -= up * cameraSpeed * deltaTime;

		// get cursor xy coordinates
		double xPos;
		double yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		// calculate xy offsets
		float xOffset;
		float yOffset;
		if (firstUpdate)
		{
			xOffset = 0;
			yOffset = 0;
			firstUpdate = false;
		}
		else
		{
			xOffset = (float)xPos - lastX;
			yOffset = (float)yPos - lastY;
		}

		// update last position
		lastX = (float)xPos;
		lastY = (float)yPos;

		// apply mouselook sensitivity scaling
		xOffset *= sensitivity;
		yOffset *= sensitivity;
		
		// add offsets to yaw and pitch
		yaw -= xOffset;
		pitch += yOffset;
		
		// clamp pitch to prevent flipping around y-axis
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		
		// construct new lookat direction from yaw and pitch
		lookatDirection = normalize(vec3(cosf(toRadians(yaw)) * cosf(toRadians(pitch)), sinf(toRadians(pitch)), sinf(toRadians(yaw)) * cosf(toRadians(pitch))));

		// construct view matrix
		mat4 viewMatrix = lookat(position, position + lookatDirection, up);

		// update view and projection matrix uniforms
		int viewMatrixUniformLocation = glGetUniformLocation(shader->program, "viewMatrix");
		int projectionMatrixUniformLocation = glGetUniformLocation(shader->program, "projectionMatrix");
		glUniformMatrix4fv(viewMatrixUniformLocation, 1, 0, &viewMatrix[0][0]);
		glUniformMatrix4fv(projectionMatrixUniformLocation, 1, 0, &projectionMatrix[0][0]);
	}
};