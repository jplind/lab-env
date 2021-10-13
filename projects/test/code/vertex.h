#pragma once
#include "math/math.h"

struct vertex
{
	vec3 position;
	vec2 textureCoordinates;
	vec3 normal;

	vertex() : position(vec3()), textureCoordinates(vec2()), normal(vec3()) {}
	vertex(vec3 const& position, vec2 const& textureCoordinates, vec3 const& normal) : position(position), textureCoordinates(textureCoordinates), normal(normal) {}
};