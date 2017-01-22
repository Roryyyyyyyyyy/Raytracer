#include "LightSource.h"
#include <glm\glm.hpp>


LightSource::LightSource(vec3 pos, float len, vec3 intense, float sample)
{
	centrePoint = pos;
	sideLength = len;
	intensity = intense;
	samples = sample;
	sampleSize = sideLength / sample;
	startpoint = vec3(centrePoint.x - (sideLength / 2), centrePoint.y, centrePoint.z + (sideLength / 2));
}