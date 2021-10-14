#pragma once
#include "vertex.h"
#include <map>
#include <algorithm>

struct rendererObject
{
	int width;
	int height;
	unsigned int framebufferObject;
	unsigned int textureColorbuffer;
	unsigned int quadVertexArrayObject;
	vec3* pixels = new vec3[(long long)width * (long long)height];
	map<int, vector<int>> edges;
	shared_ptr<textureResource> texture;

	vec3 eye = vec3(0, 0, 2);
	vec3 lookatDirection = vec3(0, 0, -1);
	vec3 up = vec3(0, 1, 0);
	mat4 viewMatrix = lookat(eye, eye + lookatDirection, up);
	mat4 projectionMatrix = perspective(70, (float)width / (float)height, 0.1f, 100.0f);

	struct model
	{
		unsigned int vertexArrayObject = 0;
		int drawCount = 0;
		vertex* vertices = nullptr;
		int* indices = nullptr;
		mat4 rotation = mat4();

		model() {};
		model(unsigned int const& vertexArrayObject, int const& drawCount, vertex* const& vertices, int* const& indices)
			: vertexArrayObject(vertexArrayObject), drawCount(drawCount), vertices(vertices), indices(indices) {}

		void rotate()
		{
			float deltaTime = (float)glfwGetTime();
			rotation = rotationy(deltaTime * 0.13f) * rotationy(deltaTime * 0.17f) * rotationz(deltaTime * 0.23f) * rotation;
		}
	};

	map<int, model> models;
	int modelCount = 0;

	rendererObject(int const& width, int const& height, shared_ptr<textureResource> texture) : width(width), height(height), texture(texture)
	{
		std::fill(pixels, pixels + (long long)width * (long long)height, vec3(0, 0, 0));
		setupFullscreenQuad();
		setupFramebufferObject();
	}

	~rendererObject()
	{
		delete[] pixels;
	}

	void setupFullscreenQuad()
	{
		float quadVertices[] = {
			-1, -1, 0, 0, 0,
			1, -1, 0, 1, 0,
			1, 1, 0, 1, 1,
			-1, -1, 0, 0, 0,
			1, 1, 0, 1, 1,
			-1, 1, 0, 0, 1
		};

		glGenVertexArrays(1, &quadVertexArrayObject);
		glBindVertexArray(quadVertexArrayObject);

		GLuint quadVBO;
		glGenBuffers(1, &quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), &quadVertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));
	}

	void setupFramebufferObject()
	{
		// setup frame buffer object
		glGenFramebuffers(1, &framebufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);

		// generate texture
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		// attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		//// setup render buffer object
		//unsigned int rbo;
		//glGenRenderbuffers(1, &rbo);
		//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		//glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//// attach rbo to the depth and stencil attachments of the frame buffer
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		// check if frame buffer is complete
		auto frameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (frameBufferStatus != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer error: " << frameBufferStatus << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int createModel(vertex* const& vertices, int* const& indices)
	{
		int drawCount = 3;

		unsigned int vertexArrayObject;
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		unsigned int vertexBufferObject;
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, (GLvoid*)(sizeof(float32) * 5));

		unsigned int indexBufferObject;
		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, drawCount * sizeof(int), &indices[0], GL_STATIC_DRAW);

		int modelID = modelCount;
		models[modelCount++] = model(vertexArrayObject, drawCount, vertices, indices);

		return modelID;
	}

	void printMatrix(mat4 m)
	{
		cout << m[0].x << "   " << m[0].y << "   " << m[0].z << "   " << m[0].w << endl;
		cout << m[1].x << "   " << m[1].y << "   " << m[1].z << "   " << m[1].w << endl;
		cout << m[2].x << "   " << m[2].y << "   " << m[2].z << "   " << m[2].w << endl;
		cout << m[3].x << "   " << m[3].y << "   " << m[3].z << "   " << m[3].w << endl;
	}

	void renderToFramebuffer(int const& model)
	{
		std::fill(pixels, pixels + (long long)width * (long long)height, vec3(0, 0, 0));
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*glBindVertexArray(models[model].vertexArrayObject);
		glDrawElements(GL_TRIANGLES, models[model].drawCount, GL_UNSIGNED_INT, NULL);*/

		auto m = models[model];
		m.rotate();
		mat4 MVP = m.rotation * viewMatrix * projectionMatrix;

		vertex a = models[model].vertices[0];
		vertex b = models[model].vertices[1];
		vertex c = models[model].vertices[2];

		rasterizeTriangle(MVP, a, b, c);

		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_FLOAT, pixels);
	}

	void renderFramebufferToScreen()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(quadVertexArrayObject);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void rasterizeTriangle(mat4 const& MVP, vertex& a, vertex& b, vertex& c)
	{
		vec4 rotatedPosition = MVP * vec4(a.position.x, a.position.y, a.position.z, 1);
		vec3 aPos = vec3(rotatedPosition.x / rotatedPosition.w, rotatedPosition.y / rotatedPosition.w, rotatedPosition.z / rotatedPosition.w);

		rotatedPosition = MVP * vec4(b.position.x, b.position.y, b.position.z, 1);
		vec3 bPos = vec3(rotatedPosition.x / rotatedPosition.w, rotatedPosition.y / rotatedPosition.w, rotatedPosition.z / rotatedPosition.w);

		rotatedPosition = MVP * vec4(c.position.x, c.position.y, c.position.z, 1);
		vec3 cPos = vec3(rotatedPosition.x / rotatedPosition.w, rotatedPosition.y / rotatedPosition.w, rotatedPosition.z / rotatedPosition.w);

		ivec2 aPosition = toPixelPosition(aPos);
		ivec2 bPosition = toPixelPosition(bPos);
		ivec2 cPosition = toPixelPosition(cPos);

		vec2 aUV = a.textureCoordinates;
		vec2 bUV = b.textureCoordinates;
		vec2 cUV = c.textureCoordinates;

		drawLine(aPosition, bPosition);
		drawLine(bPosition, cPosition);
		drawLine(cPosition, aPosition);

		scanlineFill(aPosition, bPosition, cPosition);
		edges.clear();
	}

	void setPixel(ivec2 const& pixelPosition, vec3 const& color)
	{	
		if (pixelPosition.x > 0 && pixelPosition.x < width && pixelPosition.y > 0 && pixelPosition.y < height)
			pixels[getPixelIndex(pixelPosition)] = color;
	}

	int getPixelIndex(ivec2 pixelPosition)
	{
		return pixelPosition.y * width + pixelPosition.x;
	}

	ivec2 toPixelPosition(vec3 const& position)
	{
		float x = (position.x + 1) * 0.5f * width;
		float y = (position.y + 1) * 0.5f * height;
		return ivec2(int(x + 0.5f), int(y + 0.5f));
	}

	void testOctants()
	{
		ivec2 position0 = toPixelPosition(vec3(0, 0, 0));
		ivec2 position1 = toPixelPosition(vec3(0.2f, 0.5f, 0));
		ivec2 position2 = toPixelPosition(vec3(0.5f, 0.2f, 0));
		ivec2 position3 = toPixelPosition(vec3(0.5f, -0.2f, 0));
		ivec2 position4 = toPixelPosition(vec3(0.2f, -0.5f, 0));
		ivec2 position5 = toPixelPosition(vec3(-0.2f, -0.5f, 0));
		ivec2 position6 = toPixelPosition(vec3(-0.5f, -0.2f, 0));
		ivec2 position7 = toPixelPosition(vec3(-0.5f, 0.2f, 0));
		ivec2 position8 = toPixelPosition(vec3(-0.2f, 0.5f, 0));

		drawLine(position0, position1);
		drawLine(position0, position2);
		drawLine(position0, position3);
		drawLine(position0, position4);
		drawLine(position0, position5);
		drawLine(position0, position6);
		drawLine(position0, position7);
		drawLine(position0, position8);
	}

	void drawLine(ivec2 start, ivec2 end)
	{
		int x = start.x;
		int y = start.y;
		int w = end.x - x;
		int h = end.y - y;
		int dx1 = 0;
		int dy1 = 0;
		int dx2 = 0;
		int dy2 = 0;

		w < 0 ? dx1 = -1 : dx1 = 1;
		h < 0 ? dy1 = -1 : dy1 = 1;
		w < 0 ? dx2 = -1 : dx2 = 1;

		int longest = abs(w);
		int shortest = abs(h);

		if (longest < shortest)
		{
			longest = abs(h);
			shortest = abs(w);
			if (h < 0) dy2 = -1; else if (h > 0) dy2 = 1;
			dx2 = 0;
		}

		int numerator = longest;
		for (int i = 0; i <= longest; i++) 
		{
			edges[y].push_back(x);
			setPixel(ivec2(x, y), vec3(1, 1, 1));

			numerator += shortest;
			if (numerator > longest)
			{
				numerator -= longest;
				x += dx1;
				y += dy1;
			}
			else 
			{
				x += dx2;
				y += dy2;
			}
		}
		//int x = start.x;
		//int y = start.y;
		//int dx = end.x - x;
		//int dy = end.y - y;

		//int D = 2 * dy - dx;

		//if (abs(dx) > abs(dy))
		//{
		//	int c1 = 2 * dy;
		//	int c2 = -2 * (dx - dy);

		//	for (; x < end.x; x++)
		//	{
		//		if (D > 0)
		//		{
		//			y++;
		//			D += c2;
		//		}
		//		else
		//		{
		//			D += c1;
		//		}
		//		setPixel(ivec2(x, y), vec3(1, 1, 1));
		//	}
		//}
		//else
		//{
		//	int c1 = 2 * dx;
		//	int c2 = -2 * (dy - dx);

		//	for (y; y < end.y; y++)
		//	{
		//		if (D > 0)
		//		{
		//			x++;
		//			D += c2;
		//		}
		//		else
		//		{
		//			D += c1;
		//		}
		//		setPixel(ivec2(x, y), vec3(1, 1, 1));
		//	}
		//}
	}

	void scanlineFill(ivec2 const& a, ivec2 const& b, ivec2 const& c)
	{
		for (auto const& edge : edges)
		{
			int y = edge.first;
			int x1 = edge.second.front();
			int x2 = edge.second.back();
			int start = x1;
			int end = x2;
			if (x1 > x2)
			{
				start = x2;
				end = x1;
			}

			float totalArea = triangleArea(a, b, c);

			for (int x = start; x < end; x++)
			{
				ivec2 pixelPosition = ivec2(x, y);

				float aWeight = triangleArea(b, c, pixelPosition) / totalArea;
				float bWeight = triangleArea(a, c, pixelPosition) / totalArea;
				float cWeight = triangleArea(a, b, pixelPosition) / totalArea;

				setPixel(pixelPosition, vec3(aWeight, bWeight, cWeight));
			}
		}
	}

	float triangleArea(ivec2 const& a, ivec2 const& b, ivec2 const& c)
	{
		return 0.5f * fabsf(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
	}
};