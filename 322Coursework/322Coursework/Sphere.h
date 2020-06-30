#pragma once
#include "Shape.h"

class Sphere : 
	public Shape
{
public:
	Sphere(vec3 centre, float radius, vec3 diffColour, float shine);
	~Sphere();
	float radius;
	bool Intersection(vec3 origin, vec3 direction, HitInfo& info);
	bool ShadowHit(vec3 origin, vec3 direction);
	void calcNormal();
};

