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
	/*GLuint m_vertexArrayObject;*/
	GLuint m_vertexArrayBuffers[1];
	uint32 m_drawCount;

	meshResource(vertex* vertices, uint32 numVertices) {
		m_drawCount = numVertices;

		glGenBuffers(1, m_vertexArrayBuffers);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[0]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/*glGenVertexArrays(1, &m_vertexArrayObject);
		glBindVertexArray(m_vertexArrayObject);
		glBindVertexArray(0);*/
	}

	void draw()
	{
		/*glBindVertexArray(m_vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
		glBindVertexArray(0);*/

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[0]);
		glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};