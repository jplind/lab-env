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
	"uniform vec3 myTranslation;\n"
	"uniform mat4 myRotation;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(pos + myTranslation, 1);\n"
	"	gl_Position = myRotation * gl_Position;\n"

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

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
    GLFWwindow* window;

	if (!glfwInit())
		return -1;

    // create window
    window = glfwCreateWindow(800, 600, "MeshTest", NULL, NULL);
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
		vertex(vec3(-0.5f, 0.5f, 0), vec4(0, 1, 0, 1)),
		vertex(vec3(0.5f, 0.5f, 0), vec4(0, 0, 1, 1)),
		vertex(vec3(0.5f, -0.5f, 0), vec4(1, 1, 0, 1))
	};
	GLuint indices[] = { 0, 1, 2, 0, 2, 3 };
	meshResource mesh = meshResource(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));

    // enter render loop
    while (!glfwWindowShouldClose(window))
    {
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);

        // draw mesh
        mesh.draw();

		// get time value for shader calculations
		float timeValue = float(glfwGetTime());

		// update uniform color
		float redValue = sinf(timeValue) * 0.25f + 0.5f;
		float greenValue = sinf(timeValue * 0.5f) * 0.25f + 0.5f;
		float blueValue = sinf(timeValue * 0.1f) * 0.25f + 0.5f;
		int myColorLocation = glGetUniformLocation(program, "myColor");
		glUniform4f(myColorLocation, redValue, greenValue, blueValue, 1.0f);

		// update uniform translation
		vec3 translationVector = vec3(sinf(timeValue) / 2.0f, 0, 0);
		int myTranslationLocation = glGetUniformLocation(program, "myTranslation");
		glUniform3fv(myTranslationLocation, 1, &translationVector[0]);

		// update uniform rotation
		mat4 rotationMatrix = rotationx(timeValue) * rotationy(timeValue * 1.2f) * rotationz(timeValue * 0.7f);
		int myRotationLocation = glGetUniformLocation(program, "myRotation");
		glUniformMatrix4fv(myRotationLocation, 1, 0, &rotationMatrix[0][0]);

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}