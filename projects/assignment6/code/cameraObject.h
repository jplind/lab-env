#pragma once
#include "render/window.h"
#include "math/math.h"
#include "shaderObject.h"

struct cameraObject
{
	GLFWwindow* window;
	vec3 position;
	vec3 lookatDirection = vec3(0, 0, -1);
	vec3 up = vec3(0, 1, 0);
	const float sensitivity = 0.05f;
	const float cameraSpeed = 5;
	int width;
	int height;

	mat4 projectionMatrix = perspective(70, (float)width / (float)height, 0.1f, 100.0f);
	mat4 viewMatrix = lookat(position, position + lookatDirection, up);

	float yaw = -90;
	float pitch = 0;
	bool firstUpdate = true;
	float lastX = 0;
	float lastY = 0;
	
	cameraObject(GLFWwindow* window, vec3 const& position, int const& width, int const& height) 
		: window(window), position(position), width(width), height(height) {}

	void update(float const& deltaTime)
	{
		vec3 translation = vec3();
		translation += normalize(vec3(lookatDirection.x, 0, lookatDirection.z))
			* (float)((int)(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) - (int)(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS));
		translation += cross(lookatDirection, up)
			* (float)((int)(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) - (int)(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS));
		if (translation != vec3())
			position += normalize(translation) * cameraSpeed * deltaTime;

		double xPos;
		double yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

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

		lastX = (float)xPos;
		lastY = (float)yPos;

		xOffset *= sensitivity;
		yOffset *= sensitivity;

		yaw += xOffset;
		pitch -= yOffset;

		if (pitch > 50.0f)
			pitch = 50.0f;
		if (pitch < -70.0f)
			pitch = -70.0f;

		lookatDirection = normalize(vec3(cosf(toRadians(yaw)) * cosf(toRadians(pitch)), sinf(toRadians(pitch)), sinf(toRadians(yaw)) * cosf(toRadians(pitch))));
		viewMatrix = lookat(position, position + lookatDirection, up);
	}
};