#pragma once
#include "config.h"
#include <stdexcept>

struct vec3
{
	float x;
	float y;
	float z;

	vec3() : x(0), y(0), z(0) {}
	vec3(float const x, float const y, float const z) : x(x), y(y), z(z) {}
	vec3(vec3 const& vec) : x(vec.x), y(vec.y), z(vec.z) {}

	vec3 operator=(vec3 const& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;

		return *this;
	}

	vec3 operator-()
	{
		return vec3(-x, -y, -z);
	}

	vec3 operator+(vec3 const& vec)
	{
		return vec3(x + vec.x, y + vec.y, z + vec.z);
	}

	vec3 operator-(vec3 const& vec)
	{
		return vec3(x - vec.x, y - vec.y, z - vec.z);
	}

	void operator+=(vec3 const& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	void operator-=(vec3 const& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	void operator*=(float const scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	vec3 operator*(float const scalar)
	{
		return vec3(x * scalar, y * scalar, z * scalar);
	}

	bool operator==(vec3 const& vec)
	{
		if (x == vec.x)
		{
			if (y == vec.y)
			{
				if (z == vec.z)
					return true;
			}
		}
		return false;
	}


	bool operator!=(vec3 const& vec)
	{
		if (x == vec.x)
		{
			if (y == vec.y)
			{
				if (z == vec.z)
					return false;
			}
		}
		return true;
	}

	float& operator[](uint32 const i)
	{
		switch (i)
		{
		default:
			throw std::out_of_range("index out of range");
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		}
	}
};

float dot(vec3 const& a, vec3 const& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float length(vec3 const& vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

vec3 cross(vec3 const& a, vec3 const& b)
{
	float x = a.y * b.z - a.z * b.y;
	float y = a.z * b.x - a.x * b.z;
	float z = a.x * b.y - a.y * b.x;
	return vec3(x, y, z);
}

vec3 normalize(vec3 const& vec)
{
	float len = length(vec);
	return vec3(vec.x / len, vec.y / len, vec.z / len);
}