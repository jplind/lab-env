#include "config.h"
#include "vec3.h"
#include <iostream>
using namespace std;

int
main(int argc, const char** argv)
{
	Math::vec3 v1(1, 2, 3);
	Math::vec3 v2(3, 5, 7);

	printf("v1: (%.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z);
	printf("v2: (%.1f, %.1f, %.1f)\n", v2.x, v2.y, v2.z);
	printf("v1 + v2: (%.1f, %.1f, %.1f)\n\n", (v1 + v2).x, (v1 + v2).y, (v1 + v2).z);

	printf("v1: (%.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z);
	printf("v2: (%.1f, %.1f, %.1f)\n", v2.x, v2.y, v2.z);
	printf("v1 - v2: (%.1f, %.1f, %.1f)\n\n", (v1 - v2).x, (v1 - v2).y, (v1 - v2).z);

	printf("v1: (%.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z);
	v1 *= 3;
	printf("v1 *= 3: (%.1f, %.1f, %.1f)\n", v1.x, v1.y, v1.z);
	v1 * 2;
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
	printf("v1 dot v2: %.1f\n", Math::dot(v1, v2));
	printf("v1 length: %.1f\n", Math::length(v1));
	Math::vec3 v1xv2 = Math::cross(v1, v2);
	printf("v1 cross v2: (%.1f, %.1f, %.1f)\n", v1xv2.x, v1xv2.y, v1xv2.z);
	Math::vec3 v1norm = Math::normalize(v1);
	printf("v1 normalized: (%.2f, %.2f, %.2f)\n", v1norm.x, v1norm.y, v1norm.z);
	Math::vec3 v2norm = Math::normalize(v2);
	printf("v2 normalized: (%.2f, %.2f, %.2f)\n", v2norm.x, v2norm.y, v2norm.z);

	return 0;
}