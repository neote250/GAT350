#pragma once


#include "Color.h"

class Tracer
{
public:
	static color3_t Trace(class Scene& scene, const struct ray_t& ray, float minDistance, float maxDistance);
};