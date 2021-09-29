#pragma once
#include "render/window.h"
#define STB_IMAGE_IMPLEMENTATION
#pragma warning(disable : 26451 6262)
#include "stb_image.h"
#pragma warning(default : 26451 6262)

struct textureResource
{
	GLuint texture;
	std::string filePath;
	unsigned char* buffer;
	int width;
	int height;
	int BPP;

	textureResource(const std::string& filePath) : filePath(filePath) 
	{
		buffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 4);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (buffer)
			stbi_image_free(buffer);
	}

	~textureResource()
	{
		glDeleteTextures(1, &texture);
	}

	void bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};