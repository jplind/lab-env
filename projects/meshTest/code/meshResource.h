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

	void draw()
	{
		glBindVertexArray(vertexArrayObject);
		glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}
};