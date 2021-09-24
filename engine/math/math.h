#pragma once
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

const double PI = 3.14159265358979323846;

float toRadians(const float& degrees)
{
	return degrees * (float)PI / 180.0f;
}