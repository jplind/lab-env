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

	// setup frame buffer
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// generate texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	// setup render buffer object
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach rbo to the depth and stencil attachments of the frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// check if frame buffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// setup screen shader
	shaderObject screenShader = shaderObject("../../../simple.shader");

	// setup quad vao
	GLuint quadVAO;
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	// setup quad
	/*float quadVertices[] = {
		-1, -1, 0, 0,
		1, -1, 1, 0,
		1, 1, 1, 1,
		-1, -1, 0, 0,
		1, 1, 1, 1,
		-1, 1, 0, 1
	};*/
	float quadVertices[] = {
		-0.3f, 0.5f, 0, 0,
		0.3f, 0.5f, 1, 0,
		0.3f, 1, 1, 1,
		-0.3f, 0.5f, 0, 0,
		0.3f, 1, 1, 1,
		-0.3f, 1, 0, 1
	};

	// setup vertex buffer object
	GLuint quadVBO;
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), &quadVertices[0], GL_STATIC_DRAW);

	// setup vertex attribute pointers
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 4, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 4, (GLvoid*)(sizeof(float32) * 2));

	// unbind vertex array object
	glBindVertexArray(0);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
		// press esc to terminate
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		// update delta time
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// update camera
		shader->use();
		camera.update(deltaTime);

		//// first pass
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
		glEnable(GL_DEPTH_TEST);

		// draw
		node1.draw();
		node2.draw();
		node3.draw();
		node4.draw();
		node5.draw();

		// second pass
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		screenShader.use();
		glBindVertexArray(quadVAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		shader->use();
		glEnable(GL_DEPTH_TEST);
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
