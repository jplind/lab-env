#pragma once
#include "vec3.h"
#include "vec4.h"
#include <assert.h>

typedef uint32_t uint32;

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

	void operator=(mat4 const& mat)
	{
		m[0] = mat.m[0];
		m[1] = mat.m[1];
		m[2] = mat.m[2];
		m[3] = mat.m[3];
	}

	bool operator==(mat4 const& mat)
	{
		return m[0] == mat.m[0] && m[1] == mat.m[1] && m[2] == mat.m[2] && m[3] == mat.m[3];
	}

	bool operator!=(mat4 const& mat)
	{
		return m[0] != mat.m[0] || m[1] != mat.m[1] || m[2] != mat.m[2] || m[3] != mat.m[3];
	}

	vec4& operator[](uint32 const i)
	{
		assert(i >= 0 && i < 4);

		switch (i)
		{
		default:
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
	mat4 tb = transpose(b);

	vec4 r0 = vec4(dot(a.m[0], tb[0]), dot(a.m[0], tb[1]), dot(a.m[0], tb[2]), dot(a.m[0], tb[3]));
	vec4 r1 = vec4(dot(a.m[1], tb[0]), dot(a.m[1], tb[1]), dot(a.m[1], tb[2]), dot(a.m[1], tb[3]));
	vec4 r2 = vec4(dot(a.m[2], tb[0]), dot(a.m[2], tb[1]), dot(a.m[2], tb[2]), dot(a.m[2], tb[3]));
	vec4 r3 = vec4(dot(a.m[3], tb[0]), dot(a.m[3], tb[1]), dot(a.m[3], tb[2]), dot(a.m[3], tb[3]));

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
	return mat4(vec4(1, 0, 0, 0), vec4(0, cosf(rad), sinf(rad), 0), vec4(0, -sinf(rad), cosf(rad), 0), vec4(0, 0, 0, 1));
}

mat4 rotationy(float const rad)
{
	return mat4(vec4(cosf(rad), 0, -sinf(rad), 0), vec4(0, 1, 0, 0), vec4(sinf(rad), 0, cosf(rad), 0), vec4(0, 0, 0, 1));
}

mat4 rotationz(float const rad)
{
	return mat4(vec4(cosf(rad), sinf(rad), 0, 0), vec4(-sinf(rad), cosf(rad), 0, 0), vec4(0, 0, 1, 0), vec4(0, 0, 0, 1));
}

mat4 rotationaxis(vec3 const& vec, float const rad)
{
	vec3 nVec = normalize(vec);
	float c = cosf(rad);
	float s = sinf(rad);
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

mat4 perspective(float const fovy, float const aspect, float const near, float const far)
{
	float yScale = 1.0f / tanf(fovy / 2.0f);
	float xScale = yScale / aspect;

	mat4 viewMatrix = mat4(vec4(xScale, 0, 0, 0), vec4(0, yScale, 0, 0), vec4(0, 0, far / (far - near), 1), vec4(0, 0, -far * near / (far - near), 0));

	return viewMatrix;
}

mat4 lookat(vec3 const& eye, vec3 const& at, vec3 const& up)
{
	vec3 zAxis = normalize(at - eye);
	vec3 xAxis = normalize(cross(zAxis, up));
	vec3 yAxis = cross(xAxis, zAxis);

	zAxis = -zAxis;

	mat4 viewMatrix = mat4(vec4(xAxis.x, xAxis.y, xAxis.z, 0), vec4(yAxis.x, yAxis.y, yAxis.z, 0), vec4(zAxis.x, zAxis.y, zAxis.z, 0), vec4(0, 0, 0, 1))
		* mat4(vec4(1, 0, 0, eye.x), vec4(0, 1, 0, eye.y), vec4(0, 0, 1, eye.z), vec4(0, 0, 0, 1))
		* mat4(vec4(-1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, -1, 0), vec4(0, 0, 0, 1));

	viewMatrix = transpose(viewMatrix);

	return viewMatrix;
}