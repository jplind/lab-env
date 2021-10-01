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
	shared_ptr<meshResource> mesh1(new meshResource("../../../cube.obj"));
	shared_ptr<meshResource> mesh2(new meshResource("../../../capsule.obj"));
	shared_ptr<meshResource> mesh3(new meshResource("../../../statue.obj"));
	shared_ptr<textureResource> texture1(new textureResource("../../../wall2.jpg"));
	shared_ptr<shaderObject> shader(new shaderObject("../../../blinnPhong.shader"));
	
	// setup graphicsNodes
	graphicsNode node1(vec3(3, 0, 3), mat4(), vec3(), 1, mesh1, texture1, shader);
	graphicsNode node2(vec3(3, 0, -3), mat4(), vec3(0.5f, 0.7f, 0.11f), 1, mesh1, texture1, shader);
	graphicsNode node3(vec3(-3, 0, -3), mat4(), vec3(), 1, mesh2, texture1, shader);
	graphicsNode node4(vec3(-3, 0, 3), mat4(), vec3(0.5f, 0.7f, 0.11f), 1, mesh2, texture1, shader);
	graphicsNode node5(vec3(0, -2, 0), mat4(rotationx(-PI / 2)), vec3(0, 0, 0.5f), 0.02, mesh3, texture1, shader);

	// setup camera
	cameraObject camera = cameraObject(window, shader, vec3(0, 2, 10), width, height);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// setup light
	shader->use();
	lightPoint light1 = lightPoint(vec3(0, 5, 5), vec3(1, 1, 1), 25);
	int lightPosUniformLocation = glGetUniformLocation(shader->program, "lightPos");
	glUniform3fv(lightPosUniformLocation, 1, &light1.position[0]);
	int lightColorUniformLocation = glGetUniformLocation(shader->program, "lightColor");
	glUniform3fv(lightColorUniformLocation, 1, &light1.color[0]);
	int lightIntensityUniformLocation = glGetUniformLocation(shader->program, "lightIntensity");
	glUniform1f(lightIntensityUniformLocation, light1.intensity);

	lightPoint light2 = lightPoint(vec3(0, 5, -5), vec3(0.2f, 1, 1), 25);
	int lightPos2UniformLocation = glGetUniformLocation(shader->program, "lightPos2");
	glUniform3fv(lightPos2UniformLocation, 1, &light2.position[0]);
	int lightColor2UniformLocation = glGetUniformLocation(shader->program, "lightColor2");
	glUniform3fv(lightColor2UniformLocation, 1, &light2.color[0]);
	int lightIntensity2UniformLocation = glGetUniformLocation(shader->program, "lightIntensity2");
	glUniform1f(lightIntensity2UniformLocation, light2.intensity);

	// setup delta time
	float deltaTime = 0;
	float lastFrame = 0;

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
		node3.draw();
		node4.draw();
		node5.draw();

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
