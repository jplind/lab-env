#include "vec4.h"
#pragma once

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
	}


	mat4 operator*(mat4 const& mat)
	{
		vec4 c0 = vec4(mat.m[0].x, mat.m[1].x, mat.m[2].x, mat.m[3].x);
		vec4 c1 = vec4(mat.m[0].y, mat.m[1].y, mat.m[2].y, mat.m[3].y);
		vec4 c2 = vec4(mat.m[0].z, mat.m[1].z, mat.m[2].z, mat.m[3].z);
		vec4 c3 = vec4(mat.m[0].w, mat.m[1].w, mat.m[2].w, mat.m[3].w);

		vec4 r0 = vec4(dot(m[0], c0), dot(m[0], c1), dot(m[0], c2), dot(m[0], c3));
		vec4 r1 = vec4(dot(m[1], c0), dot(m[1], c1), dot(m[1], c2), dot(m[1], c3));
		vec4 r2 = vec4(dot(m[2], c0), dot(m[2], c1), dot(m[2], c2), dot(m[2], c3));
		vec4 r3 = vec4(dot(m[3], c0), dot(m[3], c1), dot(m[3], c2), dot(m[3], c3));

		return mat4(r0, r1, r2, r3);
	}

	//+ operator==(rhs : mat4 const&) : bool
	//+ operator!=(rhs : mat4 const&) : bool
	//+ operator[](i : ui32 const) : vec4& // range [0,3]
};

//determinant(m: mat4 const&) :f32
//inverse(m : mat4 const&) : mat4 // returns identity upon failure
//transpose(m : mat4 const&) : mat4
//rotationx(rad : f32 const) : mat4 // radians
//rotationy(rad : f32 const) : mat4
//rotationz(rad : f32 const) : mat4
//rotationaxis(v : vec3 const&, rad : f32 const) : mat4
