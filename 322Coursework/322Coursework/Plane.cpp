#include "Plane.h"

Plane::Plane(vec3 centre, vec3 normal, vec3 diffColour, float shine) : Shape(centre, diffColour, shine, specColour)
{
	this->shine = shine;
	this->normal = normal;
}

Plane::~Plane()
{
}

bool Plane::Intersection(vec3 origin, vec3 direction, HitInfo& info)
{
	float t = dot((this->centre - origin), this->normal) / dot(direction, normal);
	if (t <= 0) {
		return false;
	}
	this->p = origin + t * direction;
	info.hit = true;
	info.t = t;
	info.diffColour = this->diffColour;
	return true;
}

bool Plane::ShadowHit(vec3 origin, vec3 direction)
{
	float t = dot((this->centre - origin), this->normal) / dot(direction, normal);
	if (t <= 0) {
		return false;
	}
	return true;
}