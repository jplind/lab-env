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
	float* depthBuffer = new float[(long long)width * (long long)height];

	map<int, vector<int>> edges;
	shared_ptr<textureResource> texture;

	vec3 position = vec3(0, 2, 6);
	vec3 lookatDirection = vec3(0, 0, -1);
	vec3 up = vec3(0, 1, 0);
	mat4 viewMatrix = lookat(position, position + lookatDirection, up);
	mat4 projectionMatrix = perspective(70, (float)width / (float)height, 0.1f, 100.0f);

	GLFWwindow* window;
	
	const float sensitivity = 0.05f;
	const float cameraSpeed = 5;

	// mouselook variables
	float yaw = -90;
	float pitch = 0;
	bool firstUpdate = true;
	float lastX = 0;
	float lastY = 0;

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

	rendererObject(int const& width, int const& height, shared_ptr<textureResource> texture, GLFWwindow* window) : width(width), height(height), texture(texture), window(window)
	{
		std::fill(pixels, pixels + (long long)width * (long long)height, vec3(0, 0, 0));
		std::fill(depthBuffer, depthBuffer + (long long)width * (long long)height, 255);
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

	int createModel(vertex* const& vertices, int* const& indices, int numIndices)
	{
		int drawCount = numIndices;

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
		std::fill(depthBuffer, depthBuffer + (long long)width * (long long)height, 255);
		
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*glBindVertexArray(models[model].vertexArrayObject);
		glDrawElements(GL_TRIANGLES, models[model].drawCount, GL_UNSIGNED_INT, NULL);*/
		
		auto m = models[model];
		m.rotate();
		mat4 MVP = m.rotation * viewMatrix * projectionMatrix;

		for (int i = 0; i < m.drawCount; i += 3)
		{
			vertex a = m.vertices[m.indices[i]];
			vertex b = m.vertices[m.indices[i + 1]];
			vertex c = m.vertices[m.indices[i + 2]];

			rasterizeTriangle(m.rotation, MVP, a, b, c);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);
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

	void rasterizeTriangle(mat4 const& rotation, mat4 const& MVP, vertex& a, vertex& b, vertex& c)
	{
		vec4 rotatedNormal = rotation * vec4(a.normal.x, a.normal.y, a.normal.z, 1);
		vec3 aNormal = vec3(rotatedNormal.x, rotatedNormal.y, rotatedNormal.z);

		vec3 trianglePos = a.position;
		if (dot(trianglePos - position, aNormal) >= 0)
			return;

		rotatedNormal = rotation * vec4(b.normal.x, b.normal.y, b.normal.z, 1);
		vec3 bNormal = vec3(rotatedNormal.x, rotatedNormal.y, rotatedNormal.z);

		rotatedNormal = rotation * vec4(c.normal.x, c.normal.y, c.normal.z, 1);
		vec3 cNormal = vec3(rotatedNormal.x, rotatedNormal.y, rotatedNormal.z);

		vec4 rotatedPosition = MVP * vec4(a.position.x, a.position.y, a.position.z, 1);
		vec3 aPos = vec3(rotatedPosition.x / rotatedPosition.w, rotatedPosition.y / rotatedPosition.w, rotatedPosition.z / rotatedPosition.w);
		float aW = rotatedPosition.w;

		rotatedPosition = MVP * vec4(b.position.x, b.position.y, b.position.z, 1);
		vec3 bPos = vec3(rotatedPosition.x / rotatedPosition.w, rotatedPosition.y / rotatedPosition.w, rotatedPosition.z / rotatedPosition.w);
		float bW = rotatedPosition.w;

		rotatedPosition = MVP * vec4(c.position.x, c.position.y, c.position.z, 1);
		vec3 cPos = vec3(rotatedPosition.x / rotatedPosition.w, rotatedPosition.y / rotatedPosition.w, rotatedPosition.z / rotatedPosition.w);
		float cW = rotatedPosition.w;

		vec2 aUV = a.textureCoordinates;
		vec2 bUV = b.textureCoordinates;
		vec2 cUV = c.textureCoordinates;

		ivec2 aPosition = toPixelPosition(aPos);
		ivec2 bPosition = toPixelPosition(bPos);
		ivec2 cPosition = toPixelPosition(cPos);

		drawLine(aPosition, bPosition);
		drawLine(bPosition, cPosition);
		drawLine(cPosition, aPosition);

		scanlineFill(aPos, bPos, cPos, aUV, bUV, cUV, aNormal, bNormal, cNormal, aW, bW, cW, aPos.z, bPos.z, cPos.z);
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
		if (start == end)
		{
			edges[start.y].push_back(start.x);
			return;
		}

		int x = start.x;
		int y = start.y;
		int w = end.x - x;
		int h = end.y - y;

		if (w == 0)
		{
			drawVerticalLine(start, end);
			return;
		}
		if (h == 0)
		{
			drawHorizontalLine(start, end);
			return;
		}
		if (w == h)
		{
			drawDiagonalLine(start, end);
			return;
		}

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
			h < 0 ? dy2 = -1 : dy2 = 1;
			dx2 = 0;
		}

		int numerator = longest / 2;
		for (int i = 0; i <= longest; i++) 
		{
			edges[y].push_back(x);

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
	}

	void drawHorizontalLine(ivec2 const& a, ivec2 const& b)
	{
		int y = a.y;
		int start = min(a.x, b.x);
		int end = max(a.x, b.x);
		for (int x = start; x <= end; x++)
			edges[y].push_back(x);
	}

	void drawVerticalLine(ivec2 const& a, ivec2 const& b)
	{
		int x = a.x;
		int start = min(a.y, b.y);
		int end = max(a.y, b.y);
		for (int y = start; y <= end; y++)
			edges[y].push_back(x);
	}

	void drawDiagonalLine(ivec2 const& a, ivec2 const& b)
	{
		int x = a.x;
		int y = a.y;
		int dx = 1;
		int dy = 1;
		if (b.x - a.x < 0)
			dx = -1;
		if (b.y - a.y < 0)
			dy = -1;
		int length = abs(a.x - b.x);

		for (int i = 0; i <= length; i++)
		{
			edges[y].push_back(x);
			x += dx;
			y += dy;
		}
	}

	float triangleArea(ivec2 const& a, ivec2 const& b, ivec2 const& c)
	{
		return 0.5f * abs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
	}

	void scanlineFill(vec3 const& aPos, vec3 const& bPos, vec3 const& cPos, vec2& aUV, vec2& bUV, vec2& cUV, vec3 const& aNormal, vec3 const& bNormal, vec3 const& cNormal, float aW, float bW, float cW, float aZ, float bZ, float cZ)
	{
		for (auto const& edge : edges)
		{
			int y = edge.first;
			int start = *std::min_element(edges[y].begin(), edges[y].end());
			int end = *std::max_element(edges[y].begin(), edges[y].end());

			ivec2 a = toPixelPosition(aPos);
			ivec2 b = toPixelPosition(bPos);
			ivec2 c = toPixelPosition(cPos);
			float totalArea = triangleArea(a, b, c);

			for (int x = start; x <= end; x++)
			{
				if (x < 0 || x >= width || y < 0 || y >= height)
					continue;

				ivec2 pixelPosition = ivec2(x, y);

				float aWeight = triangleArea(b, c, pixelPosition) / totalArea;
				float bWeight = triangleArea(a, c, pixelPosition) / totalArea;
				float cWeight = 1 - aWeight - bWeight;

				float z = aZ * aWeight + bZ * bWeight + cZ * cWeight;
				if (depthBuffer[getPixelIndex(pixelPosition)] < z)
					continue;

				float U = ((aUV.x / aW) * aWeight + (bUV.x / bW) * bWeight + (cUV.x / cW) * cWeight) / ((1 / aW) * aWeight + (1 / bW) * bWeight + (1 / cW) * cWeight);
				float V = ((aUV.y / aW) * aWeight + (bUV.y / bW) * bWeight + (cUV.y / cW) * cWeight) / ((1 / aW) * aWeight + (1 / bW) * bWeight + (1 / cW) * cWeight);
				vec2 UV = vec2(U, V);

				UV.x = min(max(UV.x, 0.0f), 1.0f);
				UV.y = min(max(UV.y, 0.0f), 1.0f);

				unsigned char* data = texture->buffer;
				int UVx = int(texture->width * UV.x + 0.5f);
				int UVy = int(texture->height * UV.y + 0.5f);
				
				int index = 4 * (UVy * texture->width + UVx);
				if (!data[index])
					continue;

				float r = data[index] / 255.0f;
				float g = data[index + 1] / 255.0f;
				float b = data[index + 2] / 255.0f;

				vec3 currentPos = aPos * aWeight + bPos * bWeight + cPos * cWeight;
				vec3 light = calculateLightPoint(vec3(0, 3, 0), vec3(1, 1, 1), 15, aNormal, currentPos);

				vec3 color = vec3(light.x * r, light.y * g, light.z * b);

				setPixel(pixelPosition, color);
				depthBuffer[getPixelIndex(pixelPosition)] = z;
			}
		}
		edges.clear();
	}

	vec3 calculateLightPoint(vec3 lightPos, vec3 lightColor, float lightIntensity, vec3 Normal, vec3 currentPos)
	{
		float ambient = 0.15f;
		vec3 normal = normalize(Normal);
		vec3 lightDirection = normalize(lightPos - currentPos);
		float diffuse = max(dot(normal, lightDirection), 0.0f);

		float specularLight = 0.4f;
		vec3 viewDirection = normalize(position - currentPos);
		vec3 reflectionDirection = reflect(-lightDirection, normal);
		float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 20);
		float specular = specularAmount * specularLight;

		float distance = length(lightPos - currentPos);
		float attenuation = min(max(lightIntensity / (distance * distance), 0.0f), 1.0f);

		return  lightColor * (ambient + diffuse + specular) * attenuation;
	}

	vec3 reflect(vec3 I, vec3 N)
	{
		return I - N * 2.0 * dot(N, I);
	}

	void updateCamera(float const& deltaTime)
	{
		// update position from wasd input
		vec3 translation = vec3();
		translation += normalize(vec3(lookatDirection.x, 0, lookatDirection.z))
			* (float)((int)(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) - (int)(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS));
		translation += cross(lookatDirection, up)
			* (float)((int)(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) - (int)(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS));
		if (translation != vec3())
			position += normalize(translation) * cameraSpeed * deltaTime;

		// get cursor xy coordinates
		double xPos;
		double yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		// calculate xy offsets
		float xOffset;
		float yOffset;
		if (firstUpdate)
		{
			xOffset = 0;
			yOffset = 0;
			firstUpdate = false;
		}
		else
		{
			xOffset = (float)xPos - lastX;
			yOffset = (float)yPos - lastY;
		}

		// update last position
		lastX = (float)xPos;
		lastY = (float)yPos;

		// apply mouselook sensitivity scaling
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		// add offsets to yaw and pitch
		yaw += xOffset;
		pitch -= yOffset;

		// clamp pitch to prevent flipping y-axis and breaking neck
		if (pitch > 50.0f)
			pitch = 50.0f;
		if (pitch < -70.0f)
			pitch = -70.0f;

		// construct new lookat direction from yaw and pitch
		lookatDirection = normalize(vec3(cosf(toRadians(yaw)) * cosf(toRadians(pitch)), sinf(toRadians(pitch)), sinf(toRadians(yaw)) * cosf(toRadians(pitch))));

		// construct view matrix
		viewMatrix = lookat(position, position + lookatDirection, up);
	}
};