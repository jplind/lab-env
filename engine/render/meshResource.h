#pragma once
#include "math/math.h"

struct meshResource
{
	GLuint vertexArrayObject;
	GLuint vertexBufferObject;
	GLuint indexBufferObject;
	uint32 drawCount;

	struct vertex
	{
		vec3 pos;
		vec2 uv;

		vertex() : pos(vec3(0, 0, 0)), uv(vec2(0, 0)) {}
		vertex(vec3 const& pos, vec2 const& uv) : pos(pos), uv(uv) {}
	};

	GLuint cubeIndices[36] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	meshResource(vertex* const vertices, uint32 const& numVertices, GLuint* const indices, uint32 const& numIndices) 
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 5, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 5, (GLvoid*)(sizeof(float32) * 3));

		// setup index buffer object
		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(indices[0]), indices, GL_STATIC_DRAW);

		// unbind vertex array object
		glBindVertexArray(0);
	}

	meshResource(float size) : meshResource(generateCubeVertices(size), 24, cubeIndices, 36) {}

	vertex* generateCubeVertices(float size)
	{
		// setup cube vertices
		vertex cubeVertices[24] = {
		vertex(vec3(-1, -1, -1) * size, vec2(0, 0)),
		vertex(vec3(-1, 1, -1) * size, vec2(0, 1)),
		vertex(vec3(1, 1, -1) * size, vec2(1, 1)),
		vertex(vec3(1, -1, -1) * size, vec2(1, 0)),

		vertex(vec3(-1, -1, 1) * size, vec2(0, 0)),
		vertex(vec3(-1, 1, 1) * size, vec2(0, 1)),
		vertex(vec3(1, 1, 1) * size, vec2(1, 1)),
		vertex(vec3(1, -1, 1) * size, vec2(1, 0)),

		vertex(vec3(-1, -1, -1) * size, vec2(0, 0)),
		vertex(vec3(-1, 1, -1) * size, vec2(0, 1)),
		vertex(vec3(-1, 1, 1) * size, vec2(1, 1)),
		vertex(vec3(-1, -1, 1) * size, vec2(1, 0)),

		vertex(vec3(-1, 1, -1) * size, vec2(0, 0)),
		vertex(vec3(1, 1, -1) * size, vec2(0, 1)),
		vertex(vec3(1, 1, 1) * size, vec2(1, 1)),
		vertex(vec3(-1, 1, 1) * size, vec2(1, 0)),

		vertex(vec3(1, 1, -1) * size, vec2(0, 0)),
		vertex(vec3(1, -1, -1) * size, vec2(0, 1)),
		vertex(vec3(1, -1, 1) * size, vec2(1, 1)),
		vertex(vec3(1, 1, 1) * size, vec2(1, 0)),

		vertex(vec3(1, -1, -1) * size, vec2(0, 0)),
		vertex(vec3(-1, -1, -1) * size, vec2(0, 1)),
		vertex(vec3(-1, -1, 1) * size, vec2(1, 1)),
		vertex(vec3(1, -1, 1) * size, vec2(1, 0))
		};

		return cubeVertices;
	}

	~meshResource()
	{
		glDeleteBuffers(1, &vertexBufferObject);
		glDeleteBuffers(1, &indexBufferObject);
		glDeleteVertexArrays(1, &vertexArrayObject);
	}

	void draw()
	{
		glBindVertexArray(vertexArrayObject);
		glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}
};