#include "config.h"
#include "render/window.h"
#include "graphicsNode.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
	shared_ptr<meshResource> mesh(new meshResource(vec3(0, 0, 0), 0.5));
	shared_ptr<textureResource> texture(new textureResource("wall.jpg"));
	shared_ptr<shaderObject> shader(new shaderObject("test.shader"));

	// setup graphicsNode
	graphicsNode node1(vec3(0, 0, 2), mesh, texture, shader);
	graphicsNode node2(vec3(0, 0, -2), mesh, texture, shader);
	graphicsNode node3(vec3(1, 1, 0), mesh, texture, shader);

	// setup projection matrix
	mat4 projectionMatrix = perspective(70, (float)width / (float)height, 0.1f, 50.0f);

	// setup view matrix
	const float cameraDistance = 6;
	const float cameraSpeed = 0.25;
	const float cameraHeight = -2;
	vec3 eye = vec3();
	const vec3 at = vec3(0, 0, 0);
	const vec3 up = vec3(0, 1, 0);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// update camera
		float timeValue = float(glfwGetTime());
		eye = vec3(cosf(timeValue * cameraSpeed) * cameraDistance, cameraHeight, sinf(timeValue * cameraSpeed) * cameraDistance);
		mat4 viewMatrix = lookat(eye, at, up);

		int viewMatrixUniformLocation = glGetUniformLocation(shader->program, "viewMatrix");
		int projectionMatrixUniformLocation = glGetUniformLocation(shader->program, "projectionMatrix");
		glUniformMatrix4fv(viewMatrixUniformLocation, 1, 0, &viewMatrix[0][0]); 
		glUniformMatrix4fv(projectionMatrixUniformLocation, 1, 0, &projectionMatrix[0][0]);

		// draw
		node1.draw();
		node2.draw();
		node3.draw();

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}