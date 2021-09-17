#include "config.h"
#include "render/window.h"
#include "render/meshResource.h"
#include "render/textureResource.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int
main(int argc, const char** argv)
{
	const GLchar* vs =
	"#version 430\n"
	"layout(location=0) in vec3 vertexPos;\n"
	"layout(location=2) in vec2 vertexUV;\n"
	"out vec2 UV;\n"
	"uniform mat4 MVP;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(vertexPos, 1);\n"
	"	gl_Position = MVP * gl_Position;\n"
	"	UV = vertexUV;\n"
	"}\n";

	const GLchar* ps =
	"#version 430\n"
	"in vec2 UV;\n"
	"out vec4 Color;\n"
	"uniform sampler2D myTextureSampler;\n"
	"void main()\n"
	"{\n"
	"	Color = texture(myTextureSampler, UV).rgba;\n"
	"}\n";

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
    GLFWwindow* window;
	int width = 800;
	int height = 600;

	if (!glfwInit())
		return -1;

    // create window
    window = glfwCreateWindow(width, height, "Textures and camera", NULL, NULL);
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

	// setup vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLint length = static_cast<GLint>(std::strlen(vs));
	glShaderSource(vertexShader, 1, &vs, &length);
	glCompileShader(vertexShader);

	// get error log
	GLint shaderLogSize;
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetShaderInfoLog(vertexShader, shaderLogSize, NULL, buf);
		printf("[SHADER COMPILE ERROR]: %s", buf);
		delete[] buf;
	}

	// setup pixel shader
	pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	length = static_cast<GLint>(std::strlen(ps));
	glShaderSource(pixelShader, 1, &ps, &length);
	glCompileShader(pixelShader);

	// get error log
	shaderLogSize;
	glGetShaderiv(pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetShaderInfoLog(pixelShader, shaderLogSize, NULL, buf);
		printf("[SHADER COMPILE ERROR]: %s", buf);
		delete[] buf;
	}

	// create program object
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, pixelShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetProgramInfoLog(program, shaderLogSize, NULL, buf);
		printf("[PROGRAM LINK ERROR]: %s", buf);
		delete[] buf;
	}

	// setup z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	// create cube meshes
	meshResource cubeMesh1 = meshResource(vec3(0.15f, -1.4f, 0.0f), 0.5f);
	meshResource cubeMesh2 = meshResource(vec3(-4.0f, 0.9f, 0.2f), 0.5f);
	meshResource cubeMesh3 = meshResource(vec3(-0.2f, 1.0f, 2.2f), 0.5f);
	meshResource cubeMesh4 = meshResource(vec3(-2.0f, 0.25f, 0.85f), 0.5f);

	// setup texture
	textureResource wallTexture = textureResource("../../../wall.jpg");
	wallTexture.bind(0);
	glUniform1f(glGetUniformLocation(program, "myTextureSampler"), 0);
	glGenerateMipmap(GL_TEXTURE_2D);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		cubeMesh1.draw();
		cubeMesh2.draw();
		cubeMesh3.draw();
		cubeMesh4.draw();

		// update mvp uniform
		float timeValue = float(glfwGetTime());
		const float cameraDistance = 6;
		const float cameraSpeed = 0.25;
		const vec3 at = vec3(-1, 0, 1);
		const vec3 eye = vec3(cosf(timeValue * cameraSpeed) * cameraDistance, -1, sinf(timeValue * cameraSpeed) * cameraDistance);
		const vec3 up = vec3(0, 1, 0);

		mat4 modelTransformMatrix = mat4();
		mat4 viewMatrix = perspective(70, (float)width / (float)height, 0.1f, 50.0f);
		mat4 projectionMatrix = lookat(eye, at, up);

		mat4 MVP = projectionMatrix * viewMatrix * modelTransformMatrix;

		int MVPUniformLocation = glGetUniformLocation(program, "MVP");
		glUniformMatrix4fv(MVPUniformLocation, 1, 0, &MVP[0][0]);

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}