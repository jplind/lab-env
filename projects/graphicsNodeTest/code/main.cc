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

	// setup graphicsNodes
	graphicsNode node1(vec3(-0.1f, 0, -0.1f), mesh, texture, shader);
	graphicsNode node2(vec3(1, 0, -0.1f), mesh, texture, shader);
	graphicsNode node3(vec3(2.2f, 0, 0), mesh, texture, shader);
	graphicsNode node4(vec3(-0.3f, 1, 0.2f), mesh, texture, shader);
	graphicsNode node5(vec3(1.3f, 1, 0), mesh, texture, shader);
	graphicsNode node6(vec3(-0.3f, 0, 1), mesh, texture, shader);

	// setup camera
	cameraObject camera = cameraObject(window, shader);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// setup delta time
	float deltaTime = 0;
	float lastFrame = 0;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// update delta time
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// update camera
		camera.update(deltaTime);

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
