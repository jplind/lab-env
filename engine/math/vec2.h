#pragma once
#include <assert.h>

typedef uint32_t uint32;

struct vec2
{
	float x;
	float y;

	vec2() : x(0), y(0) {}
	vec2(float const x, float const y) : x(x), y(y) {}
	vec2(vec2 const& vec) : x(vec.x), y(vec.y) {}

	void operator=(vec2 const& vec)
	{
		x = vec.x;
		y = vec.y;
	}

	vec2 operator-()
	{
		return vec2(-x, -y);
	}

	vec2 operator+(vec2 const& vec)
	{
		return vec2(x + vec.x, y + vec.y);
	}

	vec2 operator-(vec2 const& vec)
	{
		return vec2(x - vec.x, y - vec.y);
	}

	void operator+=(vec2 const& vec)
	{
		x += vec.x;
		y += vec.y;
	}

	void operator-=(vec2 const& vec)
	{
		x -= vec.x;
		y -= vec.y;
	}

	void operator*=(float const scalar)
	{
		x *= scalar;
		y *= scalar;
	}

	vec2 operator*(float const scalar) const
	{
		return vec2(x * scalar, y * scalar);
	}

	bool operator==(vec2 const& vec)
	{
		return x == vec.x && y == vec.y;
	}

	bool operator!=(vec2 const& vec)
	{
		return x != vec.x || y != vec.y;
	}

	float& operator[](uint32 const i)
	{
		assert(i >= 0 && i < 2);

		switch (i)
		{
		default:
		case 0:
			return x;
		case 1:
			return y;
		}
	}
};

vec2 operator-(vec2 const& a, vec2 const& b)
{
	return vec2(a.x - b.x, a.y - b.y);
}

float length(vec2 const& vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

vec2 normalize(vec2 const& vec)
{
	float len = length(vec);
	return vec2(vec.x / len, vec.y / len);
}