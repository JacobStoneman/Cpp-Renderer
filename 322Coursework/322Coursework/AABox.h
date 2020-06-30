#pragma once
#include <glm/glm/glm.hpp>
#include <vector>
#include "Shape.h"

using namespace glm;
using namespace std;

class AABox
{
public:
	vec3 pMin;
	vec3 pMax;
	bool Intersection(vec3 origin, vec3 direction);
	bool calcP(vec3 p);
	vector<Shape*> meshShapes;
};

