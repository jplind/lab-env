#pragma once
#include "math/math.h"

struct vertex
{
	vec3 pos;
	vec4 color;

	vertex() : pos(vec3(0, 0, 0)), color(vec4(1, 1, 1, 1)) {}
	vertex(vec3 const& pos, vec4 const& color) : pos(pos), color(color) {}
};

struct meshResource
{
	GLuint vertexArrayObject;
	GLuint vertexBufferObject;
	GLuint indexBufferObject;
	uint32 drawCount;

	GLuint cubeIndices[36] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	meshResource(vertex* vertices, uint32 numVertices, GLuint* indices, uint32 numIndices) 
	{
		drawCount = numIndices;

		// setup vertex array object
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		// setup vertex buffer object
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

		// setup vertex attribute pointers
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));

		// setup index buffer object
		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(indices[0]), indices, GL_STATIC_DRAW);

		// unbind vertex array object
		glBindVertexArray(0);
	}

	meshResource(vec3 pos, float size) : meshResource(generateCubeVertices(pos, size), 24, cubeIndices, 36) {}

	vertex* generateCubeVertices(vec3 pos, float size)
	{
		// setup colors
		vec4 red = vec4(0.6f, 0.2f, 0.2f, 1.0f);
		vec4 green = vec4(0.2f, 0.6f, 0.2f, 1.0f);
		vec4 blue = vec4(0.2f, 0.2f, 0.6f, 1.0f);
		vec4 yellow = vec4(0.6f, 0.6f, 0.2f, 1.0f);
		vec4 purple = vec4(0.6f, 0.2f, 0.6f, 1.0f);
		vec4 teal = vec4(0.2f, 0.6f, 0.6f, 1.0f);

		// setup scaled cube vertices
		vertex cubeVertices[24] = {
		vertex(vec3(-1, -1, -1) * size + pos, red),
		vertex(vec3(-1, 1, -1) * size + pos, red),
		vertex(vec3(1, 1, -1) * size + pos, red),
		vertex(vec3(1, -1, -1) * size + pos, red),

		vertex(vec3(-1, -1, 1) * size + pos, green),
		vertex(vec3(-1, 1, 1) * size + pos, green),
		vertex(vec3(1, 1, 1) * size + pos, green),
		vertex(vec3(1, -1, 1) * size + pos, green),

		vertex(vec3(-1, -1, -1) * size + pos, blue),
		vertex(vec3(-1, 1, -1) * size + pos, blue),
		vertex(vec3(-1, 1, 1) * size + pos, blue),
		vertex(vec3(-1, -1, 1) * size + pos, blue),

		vertex(vec3(-1, 1, -1) * size + pos, yellow),
		vertex(vec3(1, 1, -1) * size + pos, yellow),
		vertex(vec3(1, 1, 1) * size + pos, yellow),
		vertex(vec3(-1, 1, 1) * size + pos, yellow),

		vertex(vec3(1, 1, -1) * size + pos, purple),
		vertex(vec3(1, -1, -1) * size + pos, purple),
		vertex(vec3(1, -1, 1) * size + pos, purple),
		vertex(vec3(1, 1, 1) * size + pos, purple),

		vertex(vec3(1, -1, -1) * size + pos, teal),
		vertex(vec3(-1, -1, -1) * size + pos, teal),
		vertex(vec3(-1, -1, 1) * size + pos, teal),
		vertex(vec3(1, -1, 1) * size + pos, teal)
		};

		return cubeVertices;
	}

	void draw()
	{
		glBindVertexArray(vertexArrayObject);
		glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}
};