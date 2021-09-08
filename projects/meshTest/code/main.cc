#include "config.h"
#include "render/window.h"
#include "meshResource.h"

int
main(int argc, const char** argv)
{
	const GLchar* vs =
	"#version 430\n"
	"layout(location=0) in vec3 pos;\n"
	"layout(location=1) in vec4 color;\n"
	"layout(location=0) out vec4 Color;\n"
	"uniform float myTranslation;\n"
	"uniform vec2 myRotation;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(pos, 1) + vec4(myTranslation, 0, 0, 0);\n"

	"	mat4 myMatrix = mat4(vec4(1, 0, 0, 0), vec4(0, myRotation.y, -myRotation.x, 0), vec4(0, myRotation.x, myRotation.y, 0), vec4(0, 0, 0, 1));\n"
	"	gl_Position = myMatrix * gl_Position;\n"

	"	Color = color;\n"
	"}\n";

	const GLchar* ps =
	"#version 430\n"
	"layout(location=0) in vec4 color;\n"
	"uniform vec4 myColor;"
	"out vec4 Color;\n"
	"void main()\n"
	"{\n"
	"	Color = color * myColor;\n"
	"}\n";

    if (!glfwInit())
        return -1;

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
    GLFWwindow* window;

    // create window
    window = glfwCreateWindow(800, 600, "TRIANGLE", NULL, NULL);
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

	// mesh init
	vertex vertices[] = {
		vertex(vec3(-0.5f, -0.5f, 0), vec4(1, 0, 0, 1)),
		vertex(vec3(0, 0.8f, 0), vec4(0, 1, 0, 1)),
		vertex(vec3(0.5f, -0.8f, 0), vec4(0, 0, 1, 1))
	};
	meshResource mesh = meshResource(vertices, sizeof(vertices) / sizeof(vertices[0]));

    // loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // render here
        glClear(GL_COLOR_BUFFER_BIT);
        mesh.draw();
		glUseProgram(program);

		// update uniform color
		float timeValue = float(glfwGetTime());
		float redValue = sinf(timeValue / 2) / 2.0f + 0.5f;
		float greenValue = sinf(timeValue / 3) / 2.0f + 0.5f;
		float blueValue = sinf(timeValue / 4) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(program, "myColor");
		glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

		// update uniform translation
		float xTranslation = sinf(timeValue) / 2.0f;
		int translationLocation = glGetUniformLocation(program, "myTranslation");
		glUniform1f(translationLocation, xTranslation);

		// update uniform rotation
		float xRotation = timeValue;
		float sinX = sinf(xRotation);
		float cosX = cosf(xRotation);
		int rotationLocation = glGetUniformLocation(program, "myRotation");
		glUniform2f(rotationLocation, sinX, cosX);

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}