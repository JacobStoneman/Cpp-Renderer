#include "Sphere.h"

Sphere::Sphere(vec3 centre, float radius, vec3 diffColour,float shine) : Shape(centre, diffColour, shine, specColour)
{
	this->shine = shine;
	this->radius = radius;
}

Sphere::~Sphere()
{
}

bool Sphere::Intersection(vec3 origin, vec3 direction, HitInfo& info)
{
	vec3 ray = origin + direction;
	vec3 l = this->centre - origin;
	float ca = dot(l, direction);
	if (ca < 0) {
		return false;
	}
	float s2 = dot(l, l) - dot(ca, ca);
	if (s2 > (this->radius * this->radius)) {
		return false;
	}
	float hc = sqrt((this->radius * this->radius) - s2);
	float t0 = ca - hc;
	float t1 = ca + hc;
	this->p = origin + t0 * direction;
	info.hit = true;
	info.t = t0;
	info.diffColour = this->diffColour;
	calcNormal();
	return true;
}

bool Sphere::ShadowHit(vec3 origin, vec3 direction)
{
	vec3 ray = origin + direction;
	vec3 l = this->centre - origin;
	float ca = dot(l, direction);
	if (ca < 0) {
		return false;
	}
	float s2 = dot(l, l) - dot(ca, ca);
	if (s2 > (this->radius * this->radius)) {
		return false;
	}
	return true;
}

void Sphere::calcNormal() {
	this->normal = this->p - this->centre;
}
