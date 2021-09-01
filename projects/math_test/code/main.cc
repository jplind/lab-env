#include "config.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include <iostream>
using namespace std;

void vec3Test()
{
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
	printf("v2 normalized: (%.2f, %.2f, %.2f)\n", v2norm.x, v2norm.y, v2norm.z);
}

void vec4Test()
{
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
	printf("v2 normalized: (%.2f, %.2f, %.2f, %.2f)\n", v2norm.x, v2norm.y, v2norm.z, v2norm.w);
}

int
main(int argc, const char** argv)
{
	/*printf("__VEC3__\n");
	vec3Test();
	printf("\n__VEC4__\n");
	vec4Test();*/

	mat4 m1 = mat4(vec4(1, 2, 3, 4), vec4(5, 4, 6, 4), vec4(3, 3, 3, 2), vec4(1, 2, 1, 3));
	mat4 m2 = mat4(vec4(3, 2, 2, 2), vec4(4, 2, 3, 2), vec4(1, 2, 3, 5), vec4(5, 3, 2, 1));

	mat4 m3 = m1 * m2;

	cout << m3.m[0].x << "--" << m3.m[0].y << "--" << m3.m[0].z << "--" << m3.m[0].w << endl;
	cout << m3.m[1].x << "--" << m3.m[1].y << "--" << m3.m[1].z << "--" << m3.m[1].w << endl;
	cout << m3.m[2].x << "--" << m3.m[2].y << "--" << m3.m[2].z << "--" << m3.m[2].w << endl;
	cout << m3.m[3].x << "--" << m3.m[3].y << "--" << m3.m[3].z << "--" << m3.m[3].w << endl;

	return 0;
}