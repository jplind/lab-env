#pragma once
#include "config.h"
#include "render/window.h"
#include "textureResource.h"
#include "shaderObject.h"
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

    window = glfwCreateWindow(width, height, "Software Rasterizer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glewExperimental = GL_TRUE;
    glewInit();

	float deltaTime = 0;
	float lastFrame = 0;

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

	shared_ptr<textureResource> texture1(new textureResource("../../../crate4.png"));
	shared_ptr<shaderObject> simpleShader(new shaderObject("../../../simple.shader"));
	shared_ptr<cameraObject> camera(new cameraObject(window, vec3(0, 2, 8), width, height));
	shared_ptr<lightPoint> light(new lightPoint(vec3(0, 5, 0), vec3(1, 1, 1), 10));

	simpleShader->use();

	rendererObject renderer = rendererObject(width, height, texture1, camera, light);

	int model0 = renderer.createModel(cubeVertices, cubeIndices, numCubeIndices, vec3(-3, 0, 0));
	int model1 = renderer.createModel(cubeVertices, cubeIndices, numCubeIndices, vec3(3, 0, 0));

    while (!glfwWindowShouldClose(window))
    {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		camera->update(deltaTime);

		renderer.clearBuffers();
		renderer.models[model0].rotate(deltaTime);
		renderer.renderToFramebuffer(model0);
		renderer.renderToFramebuffer(model1);

		renderer.renderFramebufferToScreen();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
