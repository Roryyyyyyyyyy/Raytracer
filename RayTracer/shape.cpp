#include "shape.h"
#include <iostream>

Shape::Shape(vec3 loc, vec3 col, float shiny)
{
	location = loc;
	colour = col;
	shinyness = shiny;
}

bool Shape::Intersects(vec3 origin, vec3 dir)
{
	return false;
}

vec3 Shape::CalcNormal(vec3 intersect)
{
	return vec3(0, 0, 0);
}

Sphere::Sphere(vec3 loc, vec3 col, float rad, float shiny) : Shape(loc, col, shiny)
{
	location = loc;
	colour = col;
	radius = rad;
	shinyness = shiny;
}

bool Sphere::Intersects(vec3 origin, vec3 dir)
{
	vec3 l = location - origin;
	float tca = dot(l, dir);
	if (tca < 0)
	{
		return false;
	}
	float s = dot(l, l) - (tca * tca);
	if (s > radius)
	{
		return false;
	}
	float thc = sqrt((radius * radius) - (s * s));

	float t0 = tca - thc;
	float t1 = tca + thc;
	if (t0 < t1)
	{
		t = t0;
	}
	else
	{
		t = t1;
	}
		return true;
}

vec3 Sphere::CalcNormal(vec3 intersect)
{
	vec3 norm = normalize(intersect - location);
	return norm;
}

Plane::Plane(vec3 loc, vec3 col, vec3 norm, float shiny) : Shape(loc, col, shiny)
{
	location = loc;
	colour = col;
	normal = norm;
	shinyness = shiny;
}

bool Plane::Intersects(vec3 origin, vec3 dir)
{
	float denominator = dot(dir, normal);
	
	if (denominator > 0)
	{
		return false;
	}
	else
	{
		t = dot((location - origin), normal) / denominator;
		if (t <= 0)
		{
			return false;
		}
		return true;
	}
	
	
}

vec3 Plane::CalcNormal(vec3 intersect)
{
	return normalize(normal);
}

Triangle::Triangle(vec3 loc, vec3 norm1, vec3 two, vec3 norm2, vec3 three, vec3 norm3, vec3 col, float shiny) : Shape(loc, col, shiny)
{
	location = loc;
	colour = col;
	pointTwo = two;
	pointThree = three;
	shinyness = shiny;
	n0 = norm1;
	n1 = norm2;
	n2 = norm3;
}

bool Triangle::Intersects(vec3 origin, vec3 dir)
{
	vec3 e1 = pointTwo - location;
	vec3 e2 = pointThree - location;

	u = dot((origin - location), cross(dir, e2)) / dot(e1, cross(dir, e2));
	v = dot(dir, cross((origin - location), e1)) / dot(e1, cross(dir, e2));
	w = 1 - u - v;

	if (u < 0 || u > 1)
	{
		return false;
	}
	else if (v < 0 || (u + v) > 1)
	{
		return false;
	}


	t = dot(e2, cross((origin - location), e1)) / dot(e1, cross(dir, e2));
	if (t <= 0)
	{
		return false;
	}
	return true;
}

vec3 Triangle::CalcNormal(vec3 intersect)
{

	vec3 norm = w*n0 + u*n1 + v*n2;

	return norm;

}