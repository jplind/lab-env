#pragma once
#include "vertex.h"
#include <map>
#include <algorithm>

/*
Adding a vertex buffer and index buffer.
This should return a handle that can be used with the draw function.

Setting up a framebuffer with arbitrary dimensions.

Retrieving a pointer to the framebuffer.

Retrieving the size of the framebuffer.

Setting a vertex shader by providing a lambda function as an argument.

Setting a pixel shader by providing a lambda function as an argument.

Setting a model-view-projection matrix to transform the triangles with.

Setting a texture resource to texture the mesh with.

Rasterizing a triangle.

Drawing an index buffer to the framebuffer by passing the buffer handle as an argument.
This calls the RasterizeTriangle for each triangle in the index buffer.
This should use the vertex and pixel shaders that has previously been set up.
*/

struct rendererObject
{
	int width;
	int height;
	unsigned int framebufferObject;
	unsigned int textureColorbuffer;
	unsigned int quadVertexArrayObject;
	vec3* pixels = new vec3[(long)width * (long)height];

	struct model
	{
		unsigned int vertexArrayObject = 0;
		int drawCount = 0;
		vertex* vertices = nullptr;
		int* indices = nullptr;

		model() {};
		model(unsigned int const& vertexArrayObject, int const& drawCount, vertex* const& vertices, int* const& indices)
			: vertexArrayObject(vertexArrayObject), drawCount(drawCount), vertices(vertices), indices(indices) {}
	};

	map<int, model> models;
	int modelCount = 0;

	rendererObject(int const& width, int const& height) : width(width), height(height)
	{
		std::fill(pixels, pixels + width * height, vec3(0, 0, 0));
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

	void renderToFramebuffer(int const& model)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*glBindVertexArray(models[model].vertexArrayObject);
		glDrawElements(GL_TRIANGLES, models[model].drawCount, GL_UNSIGNED_INT, NULL);*/

		vertex a = models[model].vertices[0];
		vertex b = models[model].vertices[1];
		vertex c = models[model].vertices[2];
		rasterizeTriangle(a, b, c);

		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_FLOAT, pixels);
	}

	void renderFramebufferToScreen()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(quadVertexArrayObject);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void rasterizeTriangle(vertex a, vertex b, vertex c)
	{
		ivec2 aPosition = toPixelPosition(a.position);
		ivec2 bPosition = toPixelPosition(b.position);
		ivec2 cPosition = toPixelPosition(c.position);

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

		setPixel(toPixelPosition(a.position), vec3(1, 0, 0));
		setPixel(toPixelPosition(b.position), vec3(0, 1, 0));
		setPixel(toPixelPosition(c.position), vec3(0, 0, 1));
	}

	void setPixel(ivec2& pixelPosition, vec3& color)
	{	
		pixels[pixelPosition.y * width + pixelPosition.x] = color;
	}

	ivec2 toPixelPosition(vec3 const& position)
	{
		return ivec2((position.x + 1) * 0.5f * width, (position.y + 1) * 0.5f * height);
	}

	void printPos(ivec2 pos)
	{
		cout << pos.x << ", " << pos.y << endl;
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

		if (!(longest > shortest)) {
			longest = abs(h);
			shortest = abs(w);
			if (h < 0) dy2 = -1; else if (h > 0) dy2 = 1;
			dx2 = 0;
		}

		int numerator = longest; // >> 1
		for (int i = 0; i <= longest; i++) {
			setPixel(ivec2(x, y), vec3(1, 1, 1));
			numerator += shortest;
			if (!(numerator < longest)) {
				numerator -= longest;
				x += dx1;
				y += dy1;
			}
			else {
				x += dx2;
				y += dy2;
			}
		}
		//int x = start.x;
		//int y = start.y;
		//int dx = end.x - x;
		//int dy = end.y - y;

		////if (dx < 0)
		////	x = end.x;
		////if (dy < 0)
		////	y = end.y;

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
};