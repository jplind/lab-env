#pragma once
#include "render/window.h"
#include "math/math.h"
#include "vertex.h"
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

	// construction from OBJ file
	meshResource(string const& filePath)
	{
		// vertex data
		vector<vec3> positions;
		vector<vec2> textureCoordinates;
		vector<vec3> normals;

		// map for indexing each unique vertex
		map<ivec3, int> vertexIndices;

		// final vertices and indices, used later to setup buffers
		vector<vertex> vertices;
		vector<GLint> indices;

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
		int indexCount = 0;

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
				// split line into elements
				vector<string> lineElements;
				string str;
				while (ss >> str)
					lineElements.push_back(str);

				// setup for quad triangulation
				int firstElementIndex;
				int thirdElementIndex;

				for (int i = 0; i < lineElements.size(); i++)
				{
					// create vertex index vector from line element
					ivec3 iv = ivec3();
					ss.clear();
					ss.str(lineElements[i]);
					ss >> iv.x;
					ss.ignore(1, '/');
					ss >> iv.y;
					ss.ignore(1, '/');
					ss >> iv.z;

					// triangulate if fourth element
					if (i == 3)
					{
						indices.push_back(firstElementIndex);
						indices.push_back(thirdElementIndex);
					}

					// check dictionary, create new vertex if needed, store first and third for triangulation
					auto item = vertexIndices.find(iv);
					if (item == vertexIndices.end())
					{
						if (i == 0)
							firstElementIndex = indexCount;
						else if (i == 2)
							thirdElementIndex = indexCount;

						indices.push_back(indexCount);
						vertexIndices[iv] = indexCount++;
						vertices.push_back(vertex(positions[(long long)iv.x - 1], textureCoordinates[(long long)iv.y - 1], normals[(long long)iv.z - 1]));
					}
					else
					{
						if (i == 0)
							firstElementIndex = item->second;
						else if (i == 2)
							thirdElementIndex = item->second;

						indices.push_back(item->second);
					}
				}
			}
		}

		drawCount = (int)indices.size();

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
		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		// unbind vertex array object
		glBindVertexArray(0);
	}

	~meshResource()
	{
		glDeleteBuffers(1, &vertexBufferObject);
		glDeleteBuffers(1, &indexBufferObject);
		glDeleteVertexArrays(1, &vertexArrayObject);
	}
};