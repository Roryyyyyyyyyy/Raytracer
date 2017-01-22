#pragma once
#include <glm\glm.hpp>
#include "BoundingBox.h"
using namespace glm;

class BoundingBox
{
public:
	vec3 pMin;
	vec3 pMax;

	BoundingBox(vec3, vec3);
	bool Intersects(vec3, vec3);
};