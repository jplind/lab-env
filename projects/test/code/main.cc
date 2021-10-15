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

    glewExperimental = GL_TRUE;
    glewInit();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	shared_ptr<textureResource> texture1(new textureResource("../../../wall.jpg"));
	shared_ptr<shaderObject> simpleShader(new shaderObject("../../../simple.shader"));
	rendererObject renderer = rendererObject(width, height, texture1);

	vertex vertices[] = { 
		vertex(vec3(-1, -1, 0), vec2(0, 0), vec3(0, 0, 0)), 
		vertex(vec3(0, 1, 0), vec2(0.5f, 1), vec3(0, 0, 0)), 
		vertex(vec3(1, -1, 0), vec2(1, 0), vec3(0, 0, 0)) 
	};

	int triIndices[] = {
		0, 1, 2
	};

	int numIndices = sizeof(triIndices) / sizeof(triIndices[0]);

	int model0 = renderer.createModel(vertices, triIndices, numIndices);

	simpleShader->use();
	glBindTexture(GL_TEXTURE_2D, texture1->texture);
	glGenerateMipmap(GL_TEXTURE_2D);

    while (!glfwWindowShouldClose(window))
    {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

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
