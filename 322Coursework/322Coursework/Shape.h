#pragma once
#include <iostream>
#include <glm/glm/glm.hpp>
using namespace std;
using namespace glm;

struct HitInfo {
	float t;
	vec3 diffColour;
	bool hit;
};

class Shape
{
public:
	Shape(vec3 centre = vec3(0,0,0), vec3 diffColour = vec3(0,0,0), float shine = 0, vec3 specColour = vec3(0.7,0.7,0.7));
	~Shape();
	vec3 normal;
	vec3 p;
	vec3 centre;
	vec3 diffColour;
	vec3 specColour;
	float shine;
	virtual bool Intersection(vec3 origin, vec3 direction, HitInfo& info) { cout << "Object has no way of intersecting"; return false; };
	virtual bool ShadowHit(vec3 origin, vec3 direction) { /*cout << "Object has no way of checking shadows";*/ return false; };
};


