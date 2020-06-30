#include "Triangle.h"

Triangle::Triangle(vec3 v1, vec3 v2, vec3 v3, vec3 n0, vec3 n1, vec3 n2, vec3 diffColour, float shine) : Shape(centre, diffColour, shine, specColour)
{
	this->shine = shine;
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->n0 = n0;
	this->n1 = n1;
	this->n2 = n2;
	//normal = (-n0, -n1, -n2);
}

Triangle::~Triangle()
{
}

bool Triangle::Intersection(vec3 origin, vec3 direction, HitInfo& info)
{
	vec3 e1 = this->v2 - this->v1;
	vec3 e2 = this->v3 - this->v1;
	vec3 oa = origin - this->v1;
	vec3 oae1 = cross(oa,e1);
	vec3 de2 = cross(direction, e2);
	float e1de2 = dot(e1, de2);
	float e1de2Inv = 1.0f / e1de2;
	float u = (dot(oa, de2)) * e1de2Inv;
	float v = dot(direction, oae1) * e1de2Inv;

	float t = dot(e2, oae1) * e1de2Inv;

	if (u < 0 || u > 1) {
		return false;
	}
	else if (v < 0 || u + v > 1){
		return false;
	}
	else {
		float w = 1.0f - u - v;
		this->normal = w * this->n0 + u * this->n1 + v * this->n2;
		info.hit = true;
		info.t = t;
		info.diffColour = this->diffColour;
		return true;
	}
}

bool Triangle::ShadowHit(vec3 origin, vec3 direction)
{
	vec3 e1 = this->v2 - this->v1;
	vec3 e2 = this->v3 - this->v1;
	vec3 oa = origin - this->v1;
	vec3 oae1 = cross(oa, e1);
	vec3 de2 = cross(direction, e2);
	float e1de2 = dot(e1, de2);
	float e1de2Inv = 1.0f / e1de2;
	float u = (dot(oa, de2)) * e1de2Inv;
	float v = dot(direction, oae1) * e1de2Inv;
	float w = 1.0f - u - v;
	float t = dot(e2, oae1) * e1de2Inv;

	if (u < 0 || u > 1) {
		return false;
	}
	else if (v < 0 || u + v > 1) {
		return false;
	}
	else {
		return true;
	}
}
