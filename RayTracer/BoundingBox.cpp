#include "BoundingBox.h"
#include <iostream>

BoundingBox::BoundingBox(vec3 min, vec3 max)
{
	pMin = min;
	pMax = max;
}

bool BoundingBox::Intersects(vec3 rayDir, vec3 origin)
{




	//////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////


	float txMin = (pMin.x - origin.x) / rayDir.x;
	float txMax = (pMax.x - origin.x) / rayDir.x;

	if (txMin > txMax)
	{
		std::swap(txMin, txMax);
	}

	float tyMin = (pMin.y - origin.y) / rayDir.y;
	float tyMax = (pMax.y - origin.y) / rayDir.y;

	if (tyMin > tyMax)
	{
		std::swap(tyMin, tyMax);
	}

	if ((txMin > tyMax) || (tyMin > txMax))
	{
		return false;
	}

	if (tyMin > txMin)
	{
		txMin = tyMin;
	}

	if (tyMax < txMax)
	{
		tyMax = tyMax;
	}

	float tzMin = (pMin.z - origin.z) / rayDir.z;
	float tzMax = (pMax.z - origin.z) / rayDir.z;

	if (tzMin > tzMax)
	{
		std::swap(tzMin, tzMax);
	}

	if ((txMin > tzMax) || (tzMin > txMax))
	{
		return false;
	}

	if (tzMin > txMin)
	{
		txMin = tzMin;
	}

	if (tzMax < tzMax)
	{
		txMax = tzMax;
	}

	//std::cout << "hit" << std::endl;
	return true;


	//////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	/*float epsilon = 0.001f;
	float t1;
	float t2;
	float t3;
	float t4;
	float t5;
	float t6;

	vec3 p1;
	vec3 p2;
	vec3 p3;
	vec3 p4;
	vec3 p5;
	vec3 p6;
	
	if (rayDir.x)
	{
		t1 = pMin.x - origin.x / rayDir.x;
		t2 = pMin.x - origin.x / rayDir.x;
	}
	if (rayDir.y)
	{
		t1 = pMin.y - origin.y / rayDir.y;
		t2 = pMin.y - origin.y / rayDir.y;
	}
	if (rayDir.z)
	{
		t1 = pMin.z - origin.z / rayDir.z;
		t2 = pMin.z - origin.z / rayDir.z;
	}

	p1 = origin + rayDir * t1;
	p2 = origin + rayDir * t2;
	p3 = origin + rayDir * t3;
	p4 = origin + rayDir * t4;
	p5 = origin + rayDir * t5;
	p6 = origin + rayDir * t6;

	if (p1.x > pMin.x - epsilon && p1.x < pMax.x + epsilon &&
		p1.y > pMin.y - epsilon && p1.y < pMax.y + epsilon &&
		p1.z > pMin.z - epsilon && p1.z < pMax.z + epsilon)
	{
		std::cout << "hit" << std::endl;
		return true;
	}
	return false;*/
}