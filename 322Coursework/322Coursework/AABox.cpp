#include "AABox.h"

bool AABox::calcP(vec3 p) {
	if (p.x > this->pMin.x - 0.001f && p.x < this->pMax.x + 0.001f && p.y > this->pMin.y - 0.001f && p.y + 0.001f && p.z > this->pMin.z < this->pMax.z + 0.001f) {
		return true;
	}
	else {
		return false;
	}
}

bool AABox::Intersection(vec3 origin, vec3 direction)
{
	float tMin = (this->pMin.x - origin.x) / direction.x;
	float tMax = (this->pMax.x - origin.x) / direction.x;

	if (tMin > tMax) {
		swap(tMin, tMax);
	}

	float tYMin = (this->pMin.y - origin.y) / direction.y;
	float tYMax = (this->pMax.y - origin.y) / direction.y;

	if (tYMin > tYMax) {
		swap(tYMin, tYMax);
	}

	if ((tMin > tYMax) || (tYMin > tMax)) {
		return false;
	}

	if (tYMin > tMin) {
		tMin = tYMin;
	}

	if (tYMax < tMax) {
		tMax = tYMax;
	}

	float tZMin = (this->pMin.z - origin.z) / direction.z;
	float tZMax = (this->pMax.z - origin.z) / direction.z;

	if (tZMin > tZMax) {
		swap(tZMin, tZMax);
	}

	if ((tMin > tZMax) || (tZMin > tMax)) {
		return false;
	}

	if (tZMin > tMin) {
		tMin = tZMin;
	}

	if (tZMax < tMax) {
	tMax = tZMax;
	}

	return true;
}
