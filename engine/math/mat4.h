#pragma once
#include "vec3.h"
#include "vec4.h"
#include "config.h"
#include <stdexcept>

struct mat4
{
	vec4 m[4];

	mat4()
	{
		m[0] = vec4(1, 0, 0, 0);
		m[1] = vec4(0, 1, 0, 0);
		m[2] = vec4(0, 0, 1, 0);
		m[3] = vec4(0, 0, 0, 1);
	}

	mat4(vec4 const& r0, vec4 const& r1, vec4 const& r2, vec4 const& r3)
	{
		m[0] = r0;
		m[1] = r1;
		m[2] = r2;
		m[3] = r3;
	}

	mat4(mat4 const& mat)
	{
		m[0] = mat.m[0];
		m[1] = mat.m[1];
		m[2] = mat.m[2];
		m[3] = mat.m[3];
	}

	mat4 operator=(mat4 const& mat)
	{
		m[0] = mat.m[0];
		m[1] = mat.m[1];
		m[2] = mat.m[2];
		m[3] = mat.m[3];

		return *this;
	}

	bool operator==(mat4 const& mat)
	{
		if (m[0] == mat.m[0])
		{
			if (m[1] == mat.m[1])
			{
				if (m[2] == mat.m[2])
				{
					if (m[3] == mat.m[3])
						return true;
				}
			}
		}
		return false;
	}

	bool operator!=(mat4 const& mat)
	{
		if (m[0] == mat.m[0])
		{
			if (m[1] == mat.m[1])
			{
				if (m[2] == mat.m[2])
				{
					if (m[3] == mat.m[3])
						return false;
				}
			}
		}
		return true;
	}

	vec4& operator[](uint32 const i)
	{
		switch (i)
		{
		default:
			throw std::out_of_range("index out of range");
		case 0:
			return m[0];
		case 1:
			return m[1];
		case 2:
			return m[2];
		case 3:
			return m[3];
		}
	}
};

mat4 transpose(mat4 const& mat)
{
	vec4 r0 = vec4(mat.m[0].x, mat.m[1].x, mat.m[2].x, mat.m[3].x);
	vec4 r1 = vec4(mat.m[0].y, mat.m[1].y, mat.m[2].y, mat.m[3].y);
	vec4 r2 = vec4(mat.m[0].z, mat.m[1].z, mat.m[2].z, mat.m[3].z);
	vec4 r3 = vec4(mat.m[0].w, mat.m[1].w, mat.m[2].w, mat.m[3].w);

	return mat4(r0, r1, r2, r3);
}

mat4 operator*(mat4 const& a, mat4 const& b)
{
	vec4 c0 = vec4(b.m[0].x, b.m[1].x, b.m[2].x, b.m[3].x);
	vec4 c1 = vec4(b.m[0].y, b.m[1].y, b.m[2].y, b.m[3].y);
	vec4 c2 = vec4(b.m[0].z, b.m[1].z, b.m[2].z, b.m[3].z);
	vec4 c3 = vec4(b.m[0].w, b.m[1].w, b.m[2].w, b.m[3].w);

	vec4 r0 = vec4(dot(a.m[0], c0), dot(a.m[0], c1), dot(a.m[0], c2), dot(a.m[0], c3));
	vec4 r1 = vec4(dot(a.m[1], c0), dot(a.m[1], c1), dot(a.m[1], c2), dot(a.m[1], c3));
	vec4 r2 = vec4(dot(a.m[2], c0), dot(a.m[2], c1), dot(a.m[2], c2), dot(a.m[2], c3));
	vec4 r3 = vec4(dot(a.m[3], c0), dot(a.m[3], c1), dot(a.m[3], c2), dot(a.m[3], c3));

	return mat4(r0, r1, r2, r3);
}

vec4 operator*(mat4 const& mat, vec4 const& vec)
{
	mat4 tMat = transpose(mat);
	return vec4(dot(tMat.m[0], vec), dot(tMat.m[1], vec), dot(tMat.m[2], vec), dot(tMat.m[3], vec));
}

float determinant(mat4 const& m)
{
	return  
		m.m[3].x * m.m[2].y * m.m[1].z * m.m[0].w - m.m[2].x * m.m[3].y * m.m[1].z * m.m[0].w -
		m.m[3].x * m.m[1].y * m.m[2].z * m.m[0].w + m.m[1].x * m.m[3].y * m.m[2].z * m.m[0].w +
		m.m[2].x * m.m[1].y * m.m[3].z * m.m[0].w - m.m[1].x * m.m[2].y * m.m[3].z * m.m[0].w -
		m.m[3].x * m.m[2].y * m.m[0].z * m.m[1].w + m.m[2].x * m.m[3].y * m.m[0].z * m.m[1].w +
		m.m[3].x * m.m[0].y * m.m[2].z * m.m[1].w - m.m[0].x * m.m[3].y * m.m[2].z * m.m[1].w -
		m.m[2].x * m.m[0].y * m.m[3].z * m.m[1].w + m.m[0].x * m.m[2].y * m.m[3].z * m.m[1].w +
		m.m[3].x * m.m[1].y * m.m[0].z * m.m[2].w - m.m[1].x * m.m[3].y * m.m[0].z * m.m[2].w -
		m.m[3].x * m.m[0].y * m.m[1].z * m.m[2].w + m.m[0].x * m.m[3].y * m.m[1].z * m.m[2].w +
		m.m[1].x * m.m[0].y * m.m[3].z * m.m[2].w - m.m[0].x * m.m[1].y * m.m[3].z * m.m[2].w -
		m.m[2].x * m.m[1].y * m.m[0].z * m.m[3].w + m.m[1].x * m.m[2].y * m.m[0].z * m.m[3].w +
		m.m[2].x * m.m[0].y * m.m[1].z * m.m[3].w - m.m[0].x * m.m[2].y * m.m[1].z * m.m[3].w -
		m.m[1].x * m.m[0].y * m.m[2].z * m.m[3].w + m.m[0].x * m.m[1].y * m.m[2].z * m.m[3].w;
}

mat4 inverse(mat4 const& m)
{
	float A2323 = m.m[2].z * m.m[3].w - m.m[2].w * m.m[3].z;
	float A1323 = m.m[2].y * m.m[3].w - m.m[2].w * m.m[3].y;
	float A1223 = m.m[2].y * m.m[3].z - m.m[2].z * m.m[3].y;
	float A0323 = m.m[2].x * m.m[3].w - m.m[2].w * m.m[3].x;
	float A0223 = m.m[2].x * m.m[3].z - m.m[2].z * m.m[3].x;
	float A0123 = m.m[2].x * m.m[3].y - m.m[2].y * m.m[3].x;
	float A2313 = m.m[1].z * m.m[3].w - m.m[1].w * m.m[3].z;
	float A1313 = m.m[1].y * m.m[3].w - m.m[1].w * m.m[3].y;
	float A1213 = m.m[1].y * m.m[3].z - m.m[1].z * m.m[3].y;
	float A2312 = m.m[1].z * m.m[2].w - m.m[1].w * m.m[2].z;
	float A1312 = m.m[1].y * m.m[2].w - m.m[1].w * m.m[2].y;
	float A1212 = m.m[1].y * m.m[2].z - m.m[1].z * m.m[2].y;
	float A0313 = m.m[1].x * m.m[3].w - m.m[1].w * m.m[3].x;
	float A0213 = m.m[1].x * m.m[3].z - m.m[1].z * m.m[3].x;
	float A0312 = m.m[1].x * m.m[2].w - m.m[1].w * m.m[2].x;
	float A0212 = m.m[1].x * m.m[2].z - m.m[1].z * m.m[2].x;
	float A0113 = m.m[1].x * m.m[3].y - m.m[1].y * m.m[3].x;
	float A0112 = m.m[1].x * m.m[2].y - m.m[1].y * m.m[2].x;

	float det = 
		+ m.m[0].x * (m.m[1].y * A2323 - m.m[1].z * A1323 + m.m[1].w * A1223)
		- m.m[0].y * (m.m[1].x * A2323 - m.m[1].z * A0323 + m.m[1].w * A0223)
		+ m.m[0].z * (m.m[1].x * A1323 - m.m[1].y * A0323 + m.m[1].w * A0123)
		- m.m[0].w * (m.m[1].x * A1223 - m.m[1].y * A0223 + m.m[1].z * A0123);

	if (det == 0)
		return mat4();

	det = 1 / det;

	vec4 r0 = vec4(
		det * (m.m[1].y * A2323 - m.m[1].z * A1323 + m.m[1].w * A1223),
		det * -(m.m[0].y * A2323 - m.m[0].z * A1323 + m.m[0].w * A1223),
		det * (m.m[0].y * A2313 - m.m[0].z * A1313 + m.m[0].w * A1213),
		det * -(m.m[0].y * A2312 - m.m[0].z * A1312 + m.m[0].w * A1212));

	vec4 r1 = vec4(
		det * -(m.m[1].x * A2323 - m.m[1].z * A0323 + m.m[1].w * A0223),
		det * (m.m[0].x * A2323 - m.m[0].z * A0323 + m.m[0].w * A0223),
		det * -(m.m[0].x * A2313 - m.m[0].z * A0313 + m.m[0].w * A0213),
		det * (m.m[0].x * A2312 - m.m[0].z * A0312 + m.m[0].w * A0212));

	vec4 r2 = vec4(
		det * (m.m[1].x * A1323 - m.m[1].y * A0323 + m.m[1].w * A0123),
		det * -(m.m[0].x * A1323 - m.m[0].y * A0323 + m.m[0].w * A0123),
		det * (m.m[0].x * A1313 - m.m[0].y * A0313 + m.m[0].w * A0113),
		det * -(m.m[0].x * A1312 - m.m[0].y * A0312 + m.m[0].w * A0112));

	vec4 r3 = vec4(
		det * -(m.m[1].x * A1223 - m.m[1].y * A0223 + m.m[1].z * A0123),
		det * (m.m[0].x * A1223 - m.m[0].y * A0223 + m.m[0].z * A0123),
		det * -(m.m[0].x * A1213 - m.m[0].y * A0213 + m.m[0].z * A0113),
		det * (m.m[0].x * A1212 - m.m[0].y * A0212 + m.m[0].z * A0112));

	return mat4(r0, r1, r2, r3);
}

mat4 rotationx(float const rad)
{
	return transpose(mat4(vec4(1, 0, 0, 0), vec4(0, cos(rad), -sin(rad), 0), vec4(0, sin(rad), cos(rad), 0), vec4(0, 0, 0, 1)));
}

mat4 rotationy(float const rad)
{
	return transpose(mat4(vec4(cos(rad), 0, sin(rad), 0), vec4(0, 1, 0, 0), vec4(-sin(rad), 0, cos(rad), 0), vec4(0, 0, 0, 1)));
}

mat4 rotationz(float const rad)
{
	return transpose(mat4(vec4(cos(rad), -sin(rad), 0, 0), vec4(sin(rad), cos(rad), 0, 0), vec4(0, 0, 1, 0), vec4(0, 0, 0, 1)));
}

mat4 rotationaxis(vec3 const& vec, float const rad)
{
	vec3 nVec = normalize(vec);
	float c = cos(rad);
	float s = sin(rad);
	float C = 1 - c;
	float x = nVec.x;
	float y = nVec.y;
	float z = nVec.z;

	return transpose(mat4(
		vec4(x * x + (y * y + z * z) * c, x * y * C - z * s, x * z * C + y * s, 0),
		vec4(x * y * C + z * s, y * y + (x * x + z * z) * c, y * z * C - x * s, 0),
		vec4(x * z * C - y * s, y * z * C + x * s, z * z + (x * x + y * y) * c, 0),
		vec4(0, 0, 0, 1)));
}