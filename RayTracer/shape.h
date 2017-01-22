#pragma once
#include "BoundingBox.h"
#include <glm/glm.hpp>
using namespace glm;

class Shape
{
public:
	float shinyness;
	vec3 location;
	vec3 colour;
	Shape(vec3, vec3, float);
	float t;
	virtual bool Intersects(vec3, vec3);
	virtual vec3 CalcNormal(vec3);

};

class Sphere: public Shape
{
public:
	float radius;
	Sphere(vec3, vec3, float, float);
	bool Intersects(vec3, vec3);
	vec3 CalcNormal(vec3);
};

class Plane : public Shape
{
public:
	vec3 normal;
	Plane(vec3, vec3, vec3, float);
	bool Intersects(vec3, vec3);
	vec3 CalcNormal(vec3);
};

class Triangle : public Shape
{
public:
	vec3 n0;
	vec3 n1;
	vec3 n2;
	vec3 pointTwo;
	vec3 pointThree;
	Triangle(vec3, vec3, vec3, vec3, vec3, vec3, vec3, float);
	bool Intersects(vec3, vec3);
	vec3 CalcNormal(vec3);
	float u;
	float v;
	float w;
};