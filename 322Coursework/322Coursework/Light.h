#pragma once

#include <glm/glm/glm.hpp>
#include "Shape.h"
#include "AABox.h"
#include <vector>
using namespace glm;
using namespace std;

class Light
{
public:
	static vec3 lightPos;
	Light();
	~Light();
	static vec3 Shading(vec3 ia, vec3 intensity, vec3 rayDirection, vector<Shape*> object, int objIndex, vector<AABox*> boxes, int boxIndex, vec3 origin, bool shadowOn);
};

