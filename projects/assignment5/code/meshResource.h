#pragma once
#include "render/window.h"
#include "math/math.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

using namespace std;

struct meshResource
{
	GLuint vertexArrayObject;
	GLuint vertexBufferObject;
	GLuint indexBufferObject;
	uint32 drawCount;

	struct vertex
	{
		vec3 position;
		vec2 textureCoordinates;
		vec3 normal;

		vertex() : position(vec3()), textureCoordinates(vec2()), normal(vec3()) {}
		vertex(vec3 const& position, vec2 const& textureCoordinates, vec3 const& normal) : position(position), textureCoordinates(textureCoordinates), normal(normal) {}
	};

	meshResource(string const& filePath)
	{
		vector<vec3> positions;
		vector<vec2> textureCoordinates;
		vector<vec3> normals;

		vector<int> positionIndices;
		vector<int> textureCoordinateIndices;
		vector<int> normalIndices;

		vector<vertex> vertices;

		ifstream stream = ifstream(filePath);
		if (!stream)
		{
			cout << "Failed to open " << filePath << endl;
			exit(-1);
		}

		stringstream ss;
		string line;
		string prefix;
		vec3 tempVec3 = vec3();
		vec2 tempVec2 = vec2();
		GLint tempGLint = 0;

		while (getline(stream, line))
		{
			ss.clear();
			ss.str(line);
			ss >> prefix;

			if (prefix == "v")
			{
				ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
				positions.push_back(tempVec3);
			}
			else if (prefix == "vt")
			{
				ss >> tempVec2.x >> tempVec2.y;
				textureCoordinates.push_back(tempVec2);
			}
			else if (prefix == "vn")
			{
				ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
				normals.push_back(tempVec3);
			}
			else if (prefix == "f")
			{
				int counter = 0;
				while (ss >> tempGLint)
				{
					if (counter == 0)
						positionIndices.push_back(tempGLint);
					else if (counter == 1)
						textureCoordinateIndices.push_back(tempGLint);
					else if (counter == 2)
						normalIndices.push_back(tempGLint);

					if (ss.peek() == '/')
					{
						counter++;
						ss.ignore(1, '/');
					}
					else if (ss.peek() == ' ')
					{
						counter++;
						ss.ignore(1, ' ');
					}

					if (counter > 2)
						counter = 0;
				}
			}
		}

		// create index map
		map<vec3, int> vertexIndices;


		// create final vertex array
		for (size_t i = 0; i < positionIndices.size(); i++)
			vertices.push_back(vertex(positions[positionIndices[i] - (size_t)1], textureCoordinates[textureCoordinateIndices[i] - (size_t)1], normals[normalIndices[i] - (size_t)1]));

		drawCount = vertices.size();

		// setup vertex array object
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		// setup vertex buffer object
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

		// setup vertex attribute pointers
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, (GLvoid*)(sizeof(float32) * 3));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, (GLvoid*)(sizeof(float32) * 5));

		// setup index buffer object

		// unbind vertex array object
		glBindVertexArray(0);
	}

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

	~meshResource()
	{
		glDeleteBuffers(1, &vertexBufferObject);
		glDeleteBuffers(1, &indexBufferObject);
		glDeleteVertexArrays(1, &vertexArrayObject);
	}

	void draw()
	{
		glBindVertexArray(vertexArrayObject);
		//glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, NULL);
		glDrawArrays(GL_TRIANGLES, 0, drawCount);
		glBindVertexArray(0);
	}
};