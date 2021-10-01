#pragma once
#include "config.h"
#include "render/window.h"
#include "graphicsNode.h"
#include "cameraObject.h"
#include "lightPoint.h"
#include <iostream>

int main(int argc, const char** argv)
{
    GLFWwindow* window;
	int width = 800;
	int height = 600;

	if (!glfwInit())
		return -1;

    // create window
    window = glfwCreateWindow(width, height, "Lighting and OBJs", NULL, NULL);
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
	shared_ptr<meshResource> mesh1(new meshResource("../../../plane.obj"));
	shared_ptr<meshResource> mesh2(new meshResource("../../../sofa.obj"));
	shared_ptr<textureResource> texture1(new textureResource("../../../grunge2.jpg"));
	shared_ptr<textureResource> texture2(new textureResource("../../../wool2.jpg"));
	shared_ptr<shaderObject> shader(new shaderObject("../../../test.shader"));
	
	// setup graphicsNodes
	graphicsNode node1(vec3(0, -0.1f, 0), mesh1, texture1, shader);
	graphicsNode node2(vec3(0, 0, -1), mesh2, texture2, shader);

	// setup camera
	cameraObject camera = cameraObject(window, shader, vec3(0, 1, 3), width, height);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// setup delta time
	float deltaTime = 0;
	float lastFrame = 0;

	// setup light
	lightPoint light = lightPoint(vec3(0, 4, 0), vec3(1, 1, 1), 20);
	shader->use();
	int lightPosUniformLocation = glGetUniformLocation(shader->program, "lightPos");
	glUniform3fv(lightPosUniformLocation, 1, &light.position[0]);
	int lightColorUniformLocation = glGetUniformLocation(shader->program, "lightColor");
	glUniform3fv(lightColorUniformLocation, 1, &light.color[0]);
	int lightIntensityUniformLocation = glGetUniformLocation(shader->program, "lightIntensity");
	glUniform1f(lightIntensityUniformLocation, light.intensity);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
		// press esc to terminate
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

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
