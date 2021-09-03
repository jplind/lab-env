#include "config.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include <iostream>
using namespace std;

void vec3Test()
{
	printf("__VEC3__\n");

	vec3 v1(1, 2, 3);
	vec3 v2(3, 5, 7);

	printf("v1: (%.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z);
	printf("v2: (%.1f, %.1f, %.1f)\n", v2.x, v2.y, v2.z);
	printf("v1 + v2: (%.1f, %.1f, %.1f)\n\n", (v1 + v2).x, (v1 + v2).y, (v1 + v2).z);

	printf("v1: (%.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z);
	printf("v2: (%.1f, %.1f, %.1f)\n", v2.x, v2.y, v2.z);
	printf("v1 - v2: (%.1f, %.1f, %.1f)\n\n", (v1 - v2).x, (v1 - v2).y, (v1 - v2).z);

	printf("v1: (%.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z);
	v1 *= 3;
	printf("v1 *= 3: (%.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z);
	v1 = v1 * 2;
	printf("v1 * 2: (%.1f, %.1f, %.1f)\n\n", v1.x, v1.y, v1.z);

	printf("v1: (%.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z);
	printf("v2: (%.1f, %.1f, %.1f)\n", v2.x, v2.y, v2.z);
	printf("v1 == v2: %s\n", v1 == v2 ? "true" : "false");
	printf("v1 != v2: %s\n", v1 != v2 ? "true" : "false");
	printf("v1 == v1: %s\n", v1 == v1 ? "true" : "false");
	printf("v1 != v1: %s\n\n", v1 != v1 ? "true" : "false");

	printf("v1: (%.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z);
	printf("v1[0]: %.1f\n", v1[0]);
	printf("v1[1]: %.1f\n", v1[1]);
	printf("v1[2]: %.1f\n\n", v1[2]);

	printf("v1: (%.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z);
	printf("v2: (%.1f, %.1f, %.1f)\n", v2.x, v2.y, v2.z);
	printf("v1 dot v2: %.1f\n", dot(v1, v2));
	printf("v1 length: %.1f\n", length(v1));
	vec3 v1xv2 = cross(v1, v2);
	printf("v1 cross v2: (%.1f, %.1f, %.1f)\n", v1xv2.x, v1xv2.y, v1xv2.z);
	vec3 v1norm = normalize(v1);
	printf("v1 normalized: (%.2f, %.2f, %.2f)\n", v1norm.x, v1norm.y, v1norm.z);
	vec3 v2norm = normalize(v2);
	printf("v2 normalized: (%.2f, %.2f, %.2f)\n\n", v2norm.x, v2norm.y, v2norm.z);
}

void vec4Test()
{
	printf("__VEC4__\n");

	vec4 v1(1, 2, 3, 4);
	vec4 v2(3, 5, 7, 9);

	printf("v1: (%.1f, %.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z, v1.w);
	printf("v2: (%.1f, %.1f, %.1f, %.1f)\n", v2.x, v2.y, v2.z, v2.w);
	printf("v1 + v2: (%.1f, %.1f, %.1f, %.1f)\n\n", (v1 + v2).x, (v1 + v2).y, (v1 + v2).z, (v1 + v2).w);

	printf("v1: (%.1f, %.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z, v1.w);
	printf("v2: (%.1f, %.1f, %.1f, %.1f)\n", v2.x, v2.y, v2.z, v2.w);
	printf("v1 - v2: (%.1f, %.1f, %.1f, %.1f)\n\n", (v1 - v2).x, (v1 - v2).y, (v1 - v2).z, (v1 - v2).w);

	printf("v1: (%.1f, %.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z, v1.w);
	v1 *= 3;
	printf("v1 *= 3: (%.1f, %.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z, v1.w);
	v1 = v1 * 2;
	printf("v1 * 2: (%.1f, %.1f, %.1f, %.1f)\n\n", v1.x, v1.y, v1.z, v1.w);

	printf("v1: (%.1f, %.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z, v1.w);
	printf("v2: (%.1f, %.1f, %.1f, %.1f)\n", v2.x, v2.y, v2.z, v2.w);
	printf("v1 == v2: %s\n", v1 == v2 ? "true" : "false");
	printf("v1 != v2: %s\n", v1 != v2 ? "true" : "false");
	printf("v1 == v1: %s\n", v1 == v1 ? "true" : "false");
	printf("v1 != v1: %s\n\n", v1 != v1 ? "true" : "false");

	printf("v1: (%.1f, %.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z, v1.w);
	printf("v1[0]: %.1f\n", v1[0]);
	printf("v1[1]: %.1f\n", v1[1]);
	printf("v1[2]: %.1f\n", v1[2]);
	printf("v1[3]: %.1f\n\n", v1[3]);

	printf("v1: (%.1f, %.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z, v1.w);
	printf("v2: (%.1f, %.1f, %.1f, %.1f)\n", v2.x, v2.y, v2.z, v2.w);
	printf("v1 dot v2: %.1f\n", dot(v1, v2));
	printf("v1 length: %.1f\n", length(v1));
	vec4 v1norm = normalize(v1);
	printf("v1 normalized: (%.2f, %.2f, %.2f, %.2f)\n", v1norm.x, v1norm.y, v1norm.z, v1norm.w);
	vec4 v2norm = normalize(v2);
	printf("v2 normalized: (%.2f, %.2f, %.2f, %.2f)\n\n", v2norm.x, v2norm.y, v2norm.z, v2norm.w);
}

void mat4Test()
{
	printf("__MAT4__\n");
	mat4 m1 = mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0), vec4(1, 2, 3, 1));
	mat4 m2 = mat4(vec4(3, 2, 2, 2), vec4(4, 2, 3, 2), vec4(1, 2, 3, 5), vec4(5, 3, 2, 1));

	mat4 m3 = m1 * m2;

	cout << m3.m[0].x << "--" << m3.m[0].y << "--" << m3.m[0].z << "--" << m3.m[0].w << endl;
	cout << m3.m[1].x << "--" << m3.m[1].y << "--" << m3.m[1].z << "--" << m3.m[1].w << endl;
	cout << m3.m[2].x << "--" << m3.m[2].y << "--" << m3.m[2].z << "--" << m3.m[2].w << endl;
	cout << m3.m[3].x << "--" << m3.m[3].y << "--" << m3.m[3].z << "--" << m3.m[3].w << endl;

	printf("\n%.1f\n", determinant(m1));
	printf("%.1f\n\n", determinant(m2));

	printf("m1 == m2: %s\n", m1 == m2 ? "true" : "false");
	printf("m1 != m2: %s\n\n", m1 != m2 ? "true" : "false");

	vec4 pZero = vec4(0, 0, 0, 1);
	vec4 result = m1 * pZero;
	printf("%.1f, %.1f, %.1f, %.1f\n\n", result.x, result.y, result.z, result.w);

	mat4 m4 = m1 * rotationx(1.0f);
	cout << m4.m[0].x << "--" << m4.m[0].y << "--" << m4.m[0].z << "--" << m4.m[0].w << endl;
	cout << m4.m[1].x << "--" << m4.m[1].y << "--" << m4.m[1].z << "--" << m4.m[1].w << endl;
	cout << m4.m[2].x << "--" << m4.m[2].y << "--" << m4.m[2].z << "--" << m4.m[2].w << endl;
	cout << m4.m[3].x << "--" << m4.m[3].y << "--" << m4.m[3].z << "--" << m4.m[3].w << endl << endl;

	mat4 m5 = mat4();
	mat4 m6 = mat4(vec4(2.0f, 0.0f, 0.0f, 0.0f),
		vec4(0.0f, 2.0f, 0.0f, 0.0f),
		vec4(0.0f, 0.0f, 2.0f, 0.0f),
		vec4(0.0f, 0.0f, 0.0f, 1.0f));
	vec4 value(2.0f, 0.0f, 0.0f, 0.0f);
	m5[0] = value;
	value = vec4(0.0f, 2.0f, 0.0f, 0.0f);
	m5[1] = value;
	value = vec4(0.0f, 0.0f, 2.0f, 0.0f);
	m5[2] = value;
	value = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m5[3] = value;

	cout << m5.m[0].x << "--" << m5.m[0].y << "--" << m5.m[0].z << "--" << m5.m[0].w << endl;
	cout << m5.m[1].x << "--" << m5.m[1].y << "--" << m5.m[1].z << "--" << m5.m[1].w << endl;
	cout << m5.m[2].x << "--" << m5.m[2].y << "--" << m5.m[2].z << "--" << m5.m[2].w << endl;
	cout << m5.m[3].x << "--" << m5.m[3].y << "--" << m5.m[3].z << "--" << m5.m[3].w << endl << endl;

	vec3 rotaxis = normalize(vec3(1.0f, 0.2f, 2.0f));
	mat4 m7 = rotationaxis(rotaxis, -2.53652f);

	cout << m7.m[0].x << "--" << m7.m[0].y << "--" << m7.m[0].z << "--" << m7.m[0].w << endl;
	cout << m7.m[1].x << "--" << m7.m[1].y << "--" << m7.m[1].z << "--" << m7.m[1].w << endl;
	cout << m7.m[2].x << "--" << m7.m[2].y << "--" << m7.m[2].z << "--" << m7.m[2].w << endl;
	cout << m7.m[3].x << "--" << m7.m[3].y << "--" << m7.m[3].z << "--" << m7.m[3].w << endl << endl;

	mat4 m8 = mat4(vec4(-0.460861f, -0.434427f, 0.773873f, 0.000000f),
		vec4(0.579067f, -0.807997f, -0.108734f, 0.000000f),
		vec4(0.672524f, 0.398013f, 0.623936f, 0.000000f),
		vec4(0.000000f, 0.000000f, 0.000000f, 1.000000f));

	cout << m8.m[0].x << "--" << m8.m[0].y << "--" << m8.m[0].z << "--" << m8.m[0].w << endl;
	cout << m8.m[1].x << "--" << m8.m[1].y << "--" << m8.m[1].z << "--" << m8.m[1].w << endl;
	cout << m8.m[2].x << "--" << m8.m[2].y << "--" << m8.m[2].z << "--" << m8.m[2].w << endl;
	cout << m8.m[3].x << "--" << m8.m[3].y << "--" << m8.m[3].z << "--" << m8.m[3].w << endl << endl;

	mat4 m10 = mat4(vec4(1, 0, 0, 1), vec4(0, 2, 1, 2), vec4(2, 1, 0, 1), vec4(2, 0, 1, 4));
	mat4 m9 = inverse(m10);

	cout << m9.m[0].x << "," << m9.m[0].y << "," << m9.m[0].z << "," << m9.m[0].w << endl;
	cout << m9.m[1].x << "," << m9.m[1].y << "," << m9.m[1].z << "," << m9.m[1].w << endl;
	cout << m9.m[2].x << "," << m9.m[2].y << "," << m9.m[2].z << "," << m9.m[2].w << endl;
	cout << m9.m[3].x << "," << m9.m[3].y << "," << m9.m[3].z << "," << m9.m[3].w << endl << endl;
}

int
main(int argc, const char** argv)
{
	/*vec3Test();
	vec4Test();*/
	mat4Test();

	return 0;
}