#pragma once
#include "vec2.h"
#include "ivec2.h"
#include "vec3.h"
#include "ivec3.h"
#include "vec4.h"
#include "mat4.h"

const double PI = 3.14159265358979323846;

float toRadians(const float& degrees)
{
	return degrees * (float)PI / 180.0f;
}

vec3 toVec3(vec4 const& vec)
{
	return vec3(vec.x, vec.y, vec.z);
}

vec4 toVec4(vec3 const& vec)
{
	return vec4(vec.x, vec.y, vec.z, 1.0f);
}