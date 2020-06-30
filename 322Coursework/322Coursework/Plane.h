#pragma once
#include "Shape.h"
class Plane : 
	public Shape
{
public:
	Plane(vec3 centre, vec3 normal, vec3 diffColour, float shine);
	~Plane();
	bool Intersection(vec3 origin, vec3 direction, HitInfo& info);
	bool ShadowHit(vec3 origin, vec3 direction);
};

