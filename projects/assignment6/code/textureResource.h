#pragma once
#define STB_IMAGE_IMPLEMENTATION
#pragma warning(disable : 26451 6262)
#include "stb_image.h"
#pragma warning(default : 26451 6262)

struct textureResource
{
	unsigned char* data;
	int width;
	int height;
	int BPP;

	textureResource(std::string const& filePath) 
	{
		data = stbi_load(filePath.c_str(), &width, &height, &BPP, 4);
	}

	~textureResource()
	{
		delete[] data;
	}
};