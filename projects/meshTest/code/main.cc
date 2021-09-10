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
	"uniform mat4 myTransform;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(pos, 1);\n"
	"	gl_Position = myTransform * gl_Position;\n"

	"	Color = color;\n"
	"}\n";

	const GLchar* ps =
	"#version 430\n"
	"layout(location=0) in vec4 color;\n"
	"out vec4 Color;\n"
	"void main()\n"
	"{\n"
	"	Color = color;\n"
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

	// setup z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// color setup
	vec4 red = vec4(0.6f, 0.2f, 0.2f, 1.0f);
	vec4 green = vec4(0.2f, 0.6f, 0.2f, 1.0f);
	vec4 blue = vec4(0.2f, 0.2f, 0.6f, 1.0f);
	vec4 yellow = vec4(0.6f, 0.6f, 0.2f, 1.0f);
	vec4 purple = vec4(0.6f, 0.2f, 0.6f, 1.0f);
	vec4 teal = vec4(0.2f, 0.6f, 0.6f, 1.0f);

	// mesh init
	vertex vertices[] = {
		vertex(vec3(-0.5f, -0.5f, -0.5f), red),
		vertex(vec3(-0.5f, 0.5f, -0.5f), red),
		vertex(vec3(0.5f, 0.5f, -0.5f), red),
		vertex(vec3(0.5f, -0.5f, -0.5f), red),

		vertex(vec3(-0.5f, -0.5f, 0.5f), green),
		vertex(vec3(-0.5f, 0.5f, 0.5f), green),
		vertex(vec3(0.5f, 0.5f, 0.5f), green),
		vertex(vec3(0.5f, -0.5f, 0.5f), green),

		vertex(vec3(-0.5f, -0.5f, -0.5f), blue),
		vertex(vec3(-0.5f, 0.5f, -0.5f), blue),
		vertex(vec3(-0.5f, 0.5f, 0.5f), blue),
		vertex(vec3(-0.5f, -0.5f, 0.5f), blue),

		vertex(vec3(-0.5f, 0.5f, -0.5f), yellow),
		vertex(vec3(0.5f, 0.5f, -0.5f), yellow),
		vertex(vec3(0.5f, 0.5f, 0.5f), yellow),
		vertex(vec3(-0.5f, 0.5f, 0.5f), yellow),

		vertex(vec3(0.5f, 0.5f, -0.5f), purple),
		vertex(vec3(0.5f, -0.5f, -0.5f), purple),
		vertex(vec3(0.5f, -0.5f, 0.5f), purple),
		vertex(vec3(0.5f, 0.5f, 0.5f), purple),

		vertex(vec3(0.5f, -0.5f, -0.5f), teal),
		vertex(vec3(-0.5f, -0.5f, -0.5f), teal),
		vertex(vec3(-0.5f, -0.5f, 0.5f), teal),
		vertex(vec3(0.5f, -0.5f, 0.5f), teal)
	};
	GLuint indices[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};
	meshResource mesh = meshResource(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));

    // render loop
    while (!glfwWindowShouldClose(window))
    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		mesh.draw();

		// update uniform transform
		float timeValue = float(glfwGetTime());
		mat4 translation = mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0), vec4(sinf(timeValue) * 0.25f, 0, 0, 1));
		mat4 rotation = rotationx(timeValue * 0.1) * rotationy(timeValue * 0.12f) * rotationz(timeValue * 0.07f);
		mat4 transform = rotation * translation;
		int myTransformLocation = glGetUniformLocation(program, "myTransform");
		glUniformMatrix4fv(myTransformLocation, 1, 0, &transform[0][0]);

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}