#pragma once
#include <assert.h>

typedef uint32_t uint32;

struct vec4
{
	float x;
	float y;
	float z;
	float w;

	vec4() : x(0), y(0), z(0), w(0) {}
	vec4(float const x, float const y, float const z, float const w) : x(x), y(y), z(z), w(w) {}
	vec4(vec4 const& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}

	void operator=(vec4 const& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
	}

	vec4 operator-()
	{
		return vec4(-x, -y, -z, -w);
	}

	vec4 operator+(vec4 const& vec)
	{
		return vec4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
	}

	vec4 operator-(vec4 const& vec)
	{
		return vec4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
	}

	void operator+=(vec4 const& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;
	}

	void operator-=(vec4 const& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;
	}

	void operator*=(float const scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
	}

	vec4 operator*(float const scalar)
	{
		return vec4(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	bool operator==(vec4 const& vec)
	{
		return x == vec.x && y == vec.y && z == vec.z && w == vec.w;

	}

	bool operator!=(vec4 const& vec)
	{
		return x != vec.x || y != vec.y || z != vec.z || w != vec.w;
	}

	float& operator[](uint32 const i)
	{
		assert(i >= 0 && i < 4);

		switch (i)
		{
		default:
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		}
	}
};

float dot(vec4 const& a, vec4 const& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float length(vec4 const& vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}

vec4 normalize(vec4 const& vec)
{
	float len = length(vec);
	return vec4(vec.x / len, vec.y / len, vec.z / len, vec.w / len);
}
