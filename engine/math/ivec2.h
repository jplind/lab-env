#pragma once
#include <assert.h>

typedef uint32_t uint32;

struct ivec2
{
	int x;
	int y;
	int z;

	ivec2() : x(0), y(0) {}
	ivec2(int const x, int const y ) : x(x), y(y) {}
	ivec2(ivec2 const& vec) : x(vec.x), y(vec.y) {}

	void operator=(ivec2 const& vec)
	{
		x = vec.x;
		y = vec.y;
	}

	ivec2 operator-()
	{
		return ivec2(-x, -y);
	}

	ivec2 operator+(ivec2 const& vec)
	{
		return ivec2(x + vec.x, y + vec.y);
	}

	ivec2 operator-(ivec2 const& vec)
	{
		return ivec2(x - vec.x, y - vec.y);
	}

	void operator+=(ivec2 const& vec)
	{
		x += vec.x;
		y += vec.y;
	}

	void operator-=(ivec2 const& vec)
	{
		x -= vec.x;
		y -= vec.y;
	}

	bool operator==(ivec2 const& vec)
	{
		return x == vec.x && y == vec.y;
	}

	bool operator!=(ivec2 const& vec)
	{
		return x != vec.x || y != vec.y;
	}

	int& operator[](uint32 const i)
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