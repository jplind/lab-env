#pragma once
#include "config.h"
#include "render/window.h"
#include "graphicsNode.h"
#include "cameraObject.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#pragma warning(disable : 26451 6262)
#include "render/stb_image.h"
#pragma warning(default : 26451 6262)

float yaw = -90;
float pitch = 0;
bool firstMouse = true;
float lastX = 800 * 0.5;
float lastY = 600 * 0.5;
vec3 cameraForward = vec3(0, 0, -1);

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = (float)xPos;
		lastY = (float)yPos;
		firstMouse = false;
	}

	float xOffset = (float)xPos - lastX;
	float yOffset = (float)yPos - lastY;
	lastX = (float)xPos;
	lastY = (float)yPos;

	float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw -= xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	vec3 forward = vec3();
	forward.x = cosf(toRadians(yaw)) * cosf(toRadians(pitch));
	forward.y = sinf(toRadians(pitch));
	forward.z = sinf(toRadians(yaw)) * cosf(toRadians(pitch));
	cameraForward = normalize(forward);
}

int main(int argc, const char** argv)
{
    GLFWwindow* window;
	int width = 800;
	int height = 600;

	if (!glfwInit())
		return -1;

    // create window
    window = glfwCreateWindow(width, height, "GraphicsNode", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // make the window's context current
    glfwMakeContextCurrent(window);

	// glew init
    glewExperimental = GL_TRUE;
    glewInit();

    // set clear color to gray
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// setup z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// setup shared resources
	shared_ptr<meshResource> mesh(new meshResource(0.5));
	shared_ptr<textureResource> texture(new textureResource("crateTexture.png"));
	shared_ptr<shaderObject> shader(new shaderObject("test.shader"));

	// setup graphicsNode
	graphicsNode node1(vec3(-0.1f, 0, -0.1f), mesh, texture, shader);
	graphicsNode node2(vec3(1, 0, -0.1f), mesh, texture, shader);
	graphicsNode node3(vec3(2.2f, 0, 0), mesh, texture, shader);
	graphicsNode node4(vec3(-0.3f, 1, 0.2f), mesh, texture, shader);
	graphicsNode node5(vec3(1.3f, 1, 0), mesh, texture, shader);
	graphicsNode node6(vec3(-0.3f, 0, 1), mesh, texture, shader);

	// setup camera
	cameraObject camera = cameraObject(shader);

	// setup projection matrix
	mat4 projectionMatrix = perspective(70, (float)width / (float)height, 0.1f, 50.0f);

	// setup view matrix
	const float cameraSpeed = 4;
	vec3 eye = vec3();
	vec3 up = vec3(0, 1, 0);

	// camera
	float deltaTime = 0;
	float lastFrame = 0;
	vec3 cameraPos = vec3(1, -1, 6);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraForward * cameraSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraForward * cameraSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos += normalize(cross(cameraForward, up)) * cameraSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos -= normalize(cross(cameraForward, up)) * cameraSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			cameraPos += up * cameraSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			cameraPos -= up * cameraSpeed * deltaTime;

		// update camera
		mat4 viewMatrix = lookat(cameraPos, cameraPos + cameraForward, up);

		int viewMatrixUniformLocation = glGetUniformLocation(shader->program, "viewMatrix");
		int projectionMatrixUniformLocation = glGetUniformLocation(shader->program, "projectionMatrix");
		glUniformMatrix4fv(viewMatrixUniformLocation, 1, 0, &viewMatrix[0][0]); 
		glUniformMatrix4fv(projectionMatrixUniformLocation, 1, 0, &projectionMatrix[0][0]);

		// draw
		node1.draw();
		node2.draw();
		node3.draw();
		node4.draw();
		node5.draw();
		node6.draw();

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
