#pragma once
#include "render/window.h"

struct cameraObject
{
	shared_ptr<shaderObject> shader;

	cameraObject(shared_ptr<shaderObject> shader) : shader(shader) {}

	void update()
	{

	}
};