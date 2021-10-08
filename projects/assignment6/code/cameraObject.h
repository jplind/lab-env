#pragma once

struct cameraObject
{
	GLFWwindow* window;
	shared_ptr<shaderObject> shader;
	vec3 position;
	vec3 lookatDirection = vec3(0, 0, -1);
	vec3 up = vec3(0, 1, 0);
	const float sensitivity = 0.05f;
	const float cameraSpeed = 5;
	int width;
	int height;
	int viewMatrixUniformLocation;
	int projectionMatrixUniformLocation;

	mat4 projectionMatrix = perspective(70, (float)width / (float)height, 0.1f, 100.0f);

	// mouselook variables
	float yaw = -90;
	float pitch = 0;
	bool firstUpdate = true;
	float lastX = 0;
	float lastY = 0;
	
	// constructor
	cameraObject(GLFWwindow* window, shared_ptr<shaderObject> shader, vec3 const& position, int const& width, int const& height) 
		: window(window), shader(shader), position(position), width(width), height(height) 
	{
		viewMatrixUniformLocation = glGetUniformLocation(shader->program, "viewMatrix");
		projectionMatrixUniformLocation = glGetUniformLocation(shader->program, "projectionMatrix");
	}

	void update(float const& deltaTime)
	{
		// update position from wasd input
		vec3 translation = vec3();
		translation += normalize(vec3(lookatDirection.x, 0, lookatDirection.z))
			* (float)((int)(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) - (int)(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS));
		translation += cross(lookatDirection, up)
			* (float)((int)(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) - (int)(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS));
		if (translation != vec3())
			position += normalize(translation) * cameraSpeed * deltaTime;

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
		yaw += xOffset;
		pitch -= yOffset;
		
		// clamp pitch to prevent flipping y-axis and breaking neck
		if (pitch > 50.0f)
			pitch = 50.0f;
		if (pitch < -70.0f)
			pitch = -70.0f;
		
		// construct new lookat direction from yaw and pitch
		lookatDirection = normalize(vec3(cosf(toRadians(yaw)) * cosf(toRadians(pitch)), sinf(toRadians(pitch)), sinf(toRadians(yaw)) * cosf(toRadians(pitch))));

		// construct view matrix
		mat4 viewMatrix = lookat(position, position + lookatDirection, up);

		// update view and projection matrix uniforms
		glUniformMatrix4fv(viewMatrixUniformLocation, 1, 0, &viewMatrix[0][0]);
		glUniformMatrix4fv(projectionMatrixUniformLocation, 1, 0, &projectionMatrix[0][0]);

		// update camera position uniform
		int cameraPosUniformLocation = glGetUniformLocation(shader->program, "cameraPos");
		glUniform3fv(cameraPosUniformLocation, 1, &position[0]);
	}
};