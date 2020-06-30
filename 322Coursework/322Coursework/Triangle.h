#pragma once
#include "Shape.h"
class Triangle :
	public Shape
{
public:
	Triangle(vec3 v1, vec3 v2, vec3 v3, vec3 n0, vec3 n1, vec3 n2,vec3 diffColour, float shine);
	vec3 v1;
	vec3 v2;
	vec3 v3;
	vec3 n0;
	vec3 n1;
	vec3 n2;
	~Triangle();
	bool Intersection(vec3 origin, vec3 direction, HitInfo& info);
	bool ShadowHit(vec3 origin, vec3 direction);
};

