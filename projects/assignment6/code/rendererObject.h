#pragma once
#include "vertex.h"
#include <map>
#include <algorithm>
#include <vector>

struct rendererObject
{
	int width;
	int height;
	int bufferSize;
	unsigned int framebufferObject;
	unsigned int textureColorbuffer;
	unsigned int quadVertexArrayObject;

	vec3* colorBuffer = new vec3[(long long)width * (long long)height];
	float* depthBuffer = new float[(long long)width * (long long)height];

	map<int, vector<int>> edges;

	shared_ptr<textureResource> texture;
	shared_ptr<cameraObject> camera;
	shared_ptr<lightPoint> light;

	float near = 0.2f;
	float far = 50.0f;
	mat4 modelTransformMatrix = mat4();
	mat4 viewMatrix = mat4();
	mat4 projectionMatrix = perspective(70, (float)width / (float)height, near, far);
	mat4 MVP = mat4();

	struct model
	{
		unsigned int vertexArrayObject = 0;
		int drawCount = 0;
		vertex* vertices = nullptr;
		int* indices = nullptr;
		vec3 position = vec3(0, 0, 0);
		mat4 rotation = mat4();

		model() {};
		model(int const& drawCount, vertex* const& vertices, int* const& indices, vec3 const& position)
			: drawCount(drawCount), vertices(vertices), indices(indices), position(position) {}

		void rotate(float const& deltaTime)
		{
			rotation = rotationy(deltaTime * 0.13f) * rotationy(deltaTime * 0.17f) * rotationz(deltaTime * 0.23f) * rotation;
		}

		mat4 getModelTransform()
		{
			return rotation * mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0), vec4(position.x, position.y, position.z, 1));
		}
	};

	map<int, model> models;
	int modelCount = 0;

	rendererObject(int const& width, int const& height, shared_ptr<textureResource> texture, shared_ptr<cameraObject> camera, shared_ptr<lightPoint> light) 
		: width(width), height(height), texture(texture), camera(camera), light(light)
	{
		bufferSize = (long long)width * (long long)height;
		clearBuffers();
		setupFullscreenQuad();
		setupFramebufferObject();
	}

	~rendererObject()
	{
		delete[] colorBuffer;
		delete[] depthBuffer;
	}

	void clearBuffers()
	{
		std::fill(colorBuffer, colorBuffer + bufferSize, vec3(0, 0, 0));
		std::fill(depthBuffer, depthBuffer + bufferSize, 255);
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
		glGenFramebuffers(1, &framebufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);

		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		auto frameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (frameBufferStatus != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer error: " << frameBufferStatus << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int createModel(vertex* const& vertices, int* const& indices, int const& numIndices, vec3 const& position)
	{
		int modelID = modelCount;
		models[modelCount++] = model(numIndices, vertices, indices, position);

		return modelID;
	}

	void renderToFramebuffer(int const& modelID)
	{
		model m = models[modelID];

		modelTransformMatrix = m.getModelTransform();
		viewMatrix = camera->viewMatrix;
		MVP = modelTransformMatrix * viewMatrix * projectionMatrix;

		for (int i = 0; i < m.drawCount; i += 3)
		{
			vertex a = m.vertices[m.indices[i]];
			vertex b = m.vertices[m.indices[i + 1]];
			vertex c = m.vertices[m.indices[i + 2]];

			rasterizeTriangle(a, b, c);
		}
	}

	void renderFramebufferToScreen()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_FLOAT, colorBuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindVertexArray(quadVertexArrayObject);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void rasterizeTriangle(vertex& a, vertex& b, vertex& c)
	{
		vec3 aWorldPos;
		vec3 bWorldPos;
		vec3 cWorldPos;

		float aw;
		float bw;
		float cw;

		if (!vertexShader(aWorldPos, a, aw))
			return;
		if (!vertexShader(bWorldPos, b, bw))
			return;
		if (!vertexShader(cWorldPos, c, cw))
			return;

		if (dot(aWorldPos - camera->position, a.normal) >= 0)
			return;

		findEdges(toPixelPosition(a.position), toPixelPosition(b.position));
		findEdges(toPixelPosition(b.position), toPixelPosition(c.position));
		findEdges(toPixelPosition(c.position), toPixelPosition(a.position));

		scanlineFillTriangle(a, b, c, aw, bw, cw, aWorldPos, bWorldPos, cWorldPos);
	}

	void setPixel(ivec2 const& pixelPosition, vec3 const& color)
	{	
		colorBuffer[getBufferIndex(pixelPosition)] = color;
	}

	int getBufferIndex(ivec2 const& pixelPosition)
	{
		return pixelPosition.y * width + pixelPosition.x;
	}

	ivec2 toPixelPosition(vec3 const& position)
	{
		float x = (position.x + 1) * 0.5f * width;
		float y = (position.y + 1) * 0.5f * height;
		return ivec2(int(x + 0.5f), int(y + 0.5f));
	}

	void findEdges(ivec2 const& start, ivec2 const& end)
	{
		int x = start.x;
		int y = start.y;
		int w = end.x - x;
		int h = end.y - y;

		if (w == 0 && h == 0)
			edges[start.y].push_back(start.x);
		else if (w == 0)
			findVerticalEdges(start, end);
		else if (h == 0)
			findHorizontalEdges(start, end);
		else if (w == h)
			findDiagonalEdges(start, end);
		else
		{
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
	}

	void findHorizontalEdges(ivec2 const& a, ivec2 const& b)
	{
		int y = a.y;
		int start = min(a.x, b.x);
		int end = max(a.x, b.x);
		for (int x = start; x <= end; x++)
			edges[y].push_back(x);
	}

	void findVerticalEdges(ivec2 const& a, ivec2 const& b)
	{
		int x = a.x;
		int start = min(a.y, b.y);
		int end = max(a.y, b.y);
		for (int y = start; y <= end; y++)
			edges[y].push_back(x);
	}

	void findDiagonalEdges(ivec2 const& a, ivec2 const& b)
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

	void scanlineFillTriangle(vertex const& a, vertex const& b, vertex const& c, float const& aw, float const& bw, float const& cw, vec3 const& aWorldPos, vec3 const& bWorldPos, vec3 const& cWorldPos)
	{
		ivec2 aPixelPos = toPixelPosition(a.position);
		ivec2 bPixelPos = toPixelPosition(b.position);
		ivec2 cPixelPos = toPixelPosition(c.position);
		float totalArea = triangleArea(aPixelPos, bPixelPos, cPixelPos);

		float awInverse = 1.0f / aw;
		vec2 aUV = a.textureCoordinates * awInverse;
		
		float bwInverse = 1.0f / bw;
		vec2 bUV = b.textureCoordinates * bwInverse;
		
		float cwInverse = 1.0f / cw;
		vec2 cUV = c.textureCoordinates * cwInverse;

		for (auto const& edge : edges)
		{
			int y = edge.first;
			int start = *std::min_element(edges[y].begin(), edges[y].end());
			int end = *std::max_element(edges[y].begin(), edges[y].end());

			for (int x = start; x <= end; x++)
			{
				if (x < 0 || x >= width || y < 0 || y >= height)
					continue;

				ivec2 pixelPos = ivec2(x, y);

				float aWeight = triangleArea(bPixelPos, cPixelPos, pixelPos) / totalArea;
				float bWeight = triangleArea(aPixelPos, cPixelPos, pixelPos) / totalArea;
				float cWeight = 1 - aWeight - bWeight;

				float z = a.position.z * aWeight + b.position.z * bWeight + c.position.z * cWeight;
				if (depthBuffer[getBufferIndex(pixelPos)] < z)
					continue;
				depthBuffer[getBufferIndex(pixelPos)] = z;

				float denominator = awInverse * aWeight + bwInverse * bWeight + cwInverse * cWeight;
				float U = (aUV.x * aWeight + bUV.x * bWeight + cUV.x * cWeight) / denominator;
				float V = (aUV.y * aWeight + bUV.y * bWeight + cUV.y * cWeight) / denominator;
				U = min(max(U, 0.0f), 1.0f);
				V = min(max(V, 0.0f), 1.0f);
				vec2 UV = vec2(U, V);

				vec3 worldPos = aWorldPos * aWeight + bWorldPos * bWeight + cWorldPos * cWeight;
				vec3 normal = a.normal * aWeight + b.normal * bWeight + c.normal * cWeight;

				vec3 color = pixelShader(worldPos, UV, normal);
				setPixel(pixelPos, color);
			}
		}
		edges.clear();
	}

	bool vertexShader(vec3& worldPos, vertex& v, float& w)
	{
		worldPos = toVec3(modelTransformMatrix * toVec4(v.position));
		
		vec4 clipPos = viewMatrix * projectionMatrix * toVec4(worldPos);
		if (clipPos.z < near || clipPos.z > far)
			return false;

		w = clipPos.w;
		v.position = vec3(clipPos.x / w, clipPos.y / w, clipPos.z / w);
		v.normal = toVec3(modelTransformMatrix * vec4(v.normal.x, v.normal.y, v.normal.z, 0));

		return true;
	}

	vec3 pixelShader(vec3 const& currentPos, vec2 const& UV, vec3 const& normal)
	{
		int UVx = int(texture->width * UV.x + 0.5f);
		int UVy = int(texture->height * UV.y + 0.5f);

		int index = 4 * (UVy * texture->width + UVx);

		float r = texture->data[index] / 255.0f;
		float g = texture->data[index + 1] / 255.0f;
		float b = texture->data[index + 2] / 255.0f;

		if (r == 0 && g == 0 && b == 0)
			return vec3(0, 0, 0);

		vec3 lightColor = calculateLightPoint(light->position, light->color, light->intensity, normal, currentPos);

		return vec3(lightColor.x * r, lightColor.y * g, lightColor.z * b);
	}

	vec3 calculateLightPoint(vec3 const& lightPos, vec3 const& lightColor, float const& lightIntensity, vec3 const& n, vec3 const& currentPos)
	{
		float ambient = 0.2f;
		vec3 normal = normalize(n);
		vec3 lightDirection = normalize(lightPos - currentPos);
		float diffuse = max(dot(normal, lightDirection), 0.0f);

		float specularLight = 0.4f;
		vec3 viewDirection = normalize(camera->position - currentPos);
		vec3 reflectionDirection = -lightDirection - normal * 2.0f * dot(normal, -lightDirection);
		float specularAmount = powf(max(dot(viewDirection, reflectionDirection), 0.0f), 50);
		float specular = specularAmount * specularLight;

		float distance = length(lightPos - currentPos);
		float attenuation = min(max(lightIntensity / distance, 0.0f), 1.0f);

		return  lightColor * (ambient + diffuse + specular) * attenuation;
	}
};