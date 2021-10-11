#pragma once
#include "math/math.h"

struct lightPoint
{
	vec3 position;
	vec3 color;
	float intensity;

	lightPoint() : position(vec3()), color(vec3()), intensity(0) {}
	lightPoint(vec3 const& position, vec3 const& color, float const& intensity) : position(position), color(color), intensity(intensity) {}
};