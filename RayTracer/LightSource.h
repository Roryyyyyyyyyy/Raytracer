#pragma once
#include <glm\glm.hpp>

using namespace glm;

class LightSource
{
public:
	vec3 centrePoint;
	float sideLength;
	vec3 intensity;
	float samples;
	vec3 startpoint;
	float sampleSize;

	LightSource(vec3, float, vec3, float);
};