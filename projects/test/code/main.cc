#pragma once
#include "config.h"
#include "render/window.h"
#include "graphicsNode.h"
#include "cameraObject.h"
#include "lightPoint.h"
#include "rendererObject.h"
#include <iostream>

int main(int argc, const char** argv)
{
    GLFWwindow* window;
	int width = 800;
	int height = 600;

	if (!glfwInit())
		return -1;

    window = glfwCreateWindow(width, height, "Lighting and OBJs", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glewExperimental = GL_TRUE;
    glewInit();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	shared_ptr<textureResource> texture1(new textureResource("../../../crateTexture.png"));
	shared_ptr<shaderObject> simpleShader(new shaderObject("../../../simple.shader"));
	rendererObject renderer = rendererObject(width, height, texture1, window);

	vertex triangleVertices[] = { 
		vertex(vec3(-1, -1, -1), vec2(0, 0), vec3(0, 0, -1)), 
		vertex(vec3(0, 1, -1), vec2(0.5f, 1), vec3(0, 0, -1)), 
		vertex(vec3(1, -1, -1), vec2(1, 0), vec3(0, 0, -1)) 
	};

	int triangleIndices[] = {
		0, 1, 2
	};

	vertex quadVertices[] = {
		vertex(vec3(-1, -1, -1), vec2(0, 0), vec3(0, 0, -1)),
		vertex(vec3(-1, 1, -1), vec2(0, 1), vec3(0, 0, -1)),
		vertex(vec3(1, 1, -1), vec2(1, 1), vec3(0, 0, -1)),
		vertex(vec3(1, -1, -1), vec2(1, 0), vec3(0, 0, -1)),
	};

	int quadIndices[] = {
		0, 1, 2,
		0, 2, 3
	};

	vertex cubeVertices[24] = {
		vertex(vec3(-1, -1, -1), vec2(0, 0), vec3(0, 0, -1)),
		vertex(vec3(-1, 1, -1), vec2(0, 1), vec3(0, 0, -1)),
		vertex(vec3(1, 1, -1), vec2(1, 1), vec3(0, 0, -1)),
		vertex(vec3(1, -1, -1), vec2(1, 0), vec3(0, 0, -1)),

		vertex(vec3(-1, -1, 1), vec2(0, 0), vec3(0, 0, 1)),
		vertex(vec3(-1, 1, 1), vec2(0, 1), vec3(0, 0, 1)),
		vertex(vec3(1, 1, 1), vec2(1, 1), vec3(0, 0, 1)),
		vertex(vec3(1, -1, 1), vec2(1, 0), vec3(0, 0, 1)),

		vertex(vec3(-1, -1, -1), vec2(0, 0), vec3(-1, 0, 0)),
		vertex(vec3(-1, 1, -1), vec2(0, 1), vec3(-1, 0, 0)),
		vertex(vec3(-1, 1, 1), vec2(1, 1), vec3(-1, 0, 0)),
		vertex(vec3(-1, -1, 1), vec2(1, 0), vec3(-1, 0, 0)),

		vertex(vec3(-1, 1, -1), vec2(0, 0), vec3(0, 1, 0)),
		vertex(vec3(1, 1, -1), vec2(0, 1), vec3(0, 1, 0)),
		vertex(vec3(1, 1, 1), vec2(1, 1), vec3(0, 1, 0)),
		vertex(vec3(-1, 1, 1), vec2(1, 0), vec3(0, 1, 0)),

		vertex(vec3(1, 1, -1), vec2(0, 0), vec3(1, 0, 0)),
		vertex(vec3(1, -1, -1), vec2(0, 1), vec3(1, 0, 0)),
		vertex(vec3(1, -1, 1), vec2(1, 1), vec3(1, 0, 0)),
		vertex(vec3(1, 1, 1), vec2(1, 0), vec3(1, 0, 0)),

		vertex(vec3(1, -1, -1), vec2(0, 0), vec3(0, -1, 0)),
		vertex(vec3(-1, -1, -1), vec2(0, 1), vec3(0, -1, 0)),
		vertex(vec3(-1, -1, 1), vec2(1, 1), vec3(0, -1, 0)),
		vertex(vec3(1, -1, 1), vec2(1, 0), vec3(0, -1, 0))
	};

	int cubeIndices[36] = {
	0, 1, 2, 0, 2, 3,
	4, 5, 6, 4, 6, 7,
	8, 9, 10, 8, 10, 11,
	12, 13, 14, 12, 14, 15,
	16, 17, 18, 16, 18, 19,
	20, 21, 22, 20, 22, 23
	};

	int numCubeIndices = sizeof(cubeIndices) / sizeof(cubeIndices[0]);
	int numTriangleIndices = sizeof(triangleIndices) / sizeof(triangleIndices[0]);
	int numQuadIndices = sizeof(quadIndices) / sizeof(quadIndices[0]);

	//int model0 = renderer.createModel(triangleVertices, triangleIndices, numTriangleIndices);
	//int model0 = renderer.createModel(quadVertices, quadIndices, numQuadIndices);
	int model0 = renderer.createModel(cubeVertices, cubeIndices, numCubeIndices);

	simpleShader->use();
	glBindTexture(GL_TEXTURE_2D, texture1->texture);
	glGenerateMipmap(GL_TEXTURE_2D);

	float deltaTime = 0;
	float lastFrame = 0;

    while (!glfwWindowShouldClose(window))
    {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		renderer.updateCamera(deltaTime);

		/*glBindTexture(GL_TEXTURE_2D, texture1->texture);*/
		glUseProgram(0);
		renderer.renderToFramebuffer(model0);

		simpleShader->use();
		renderer.renderFramebufferToScreen();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
