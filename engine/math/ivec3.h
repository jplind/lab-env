#pragma once
#include <assert.h>

typedef uint32_t uint32;

struct ivec3
{
	int x;
	int y;
	int z;

	ivec3() : x(0), y(0), z(0) {}
	ivec3(int const x, int const y, int const z) : x(x), y(y), z(z) {}
	ivec3(ivec3 const& vec) : x(vec.x), y(vec.y), z(vec.z) {}

	void operator=(ivec3 const& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	ivec3 operator-()
	{
		return ivec3(-x, -y, -z);
	}

	ivec3 operator+(ivec3 const& vec)
	{
		return ivec3(x + vec.x, y + vec.y, z + vec.z);
	}

	ivec3 operator-(ivec3 const& vec)
	{
		return ivec3(x - vec.x, y - vec.y, z - vec.z);
	}

	void operator+=(ivec3 const& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	void operator-=(ivec3 const& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	bool operator==(ivec3 const& vec)
	{
		return x == vec.x && y == vec.y && z == vec.z;
	}

	bool operator!=(ivec3 const& vec)
	{
		return x != vec.x || y != vec.y || z != vec.z;
	}

	int& operator[](uint32 const i)
	{
		assert(i >= 0 && i < 3);

		switch (i)
		{
		default:
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		}
	}
};

bool operator<(const ivec3& lhs, const ivec3& rhs)
{
	return lhs.x < rhs.x ||
		lhs.x == rhs.x && (lhs.y < rhs.y || lhs.y == rhs.y && lhs.z < rhs.z);
}

ivec3 operator-(ivec3 const& a, ivec3 const& b)
{
	return ivec3(a.x - b.x, a.y - b.y, a.z - b.z);
}