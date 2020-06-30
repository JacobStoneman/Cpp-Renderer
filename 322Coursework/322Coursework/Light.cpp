#include "Light.h"

vec3 Light::lightPos(1, 4, 1);

glm::vec3 Light::Shading(vec3 ia, vec3 intensity, vec3 rayDirection, std::vector<Shape*> objects, int objIndex, vector<AABox*> boxes, int boxIndex, vec3 origin, bool shadowOn)
{
	vec3 ca = objects[objIndex]->diffColour * ia;
	vec3 l = lightPos - objects[objIndex]->p;
	vec3 n = objects[objIndex]->normal;

	vec3 shadowOrigin = objects[objIndex]->p + 0.00001f;
	vec3 shadowRay = normalize(lightPos - shadowOrigin);

	if (shadowOn) {
		for (int i = 0; i < boxes.size(); i++) {
			if (boxes[i]->Intersection(shadowOrigin, shadowRay)) {
				for (int j = 0; j < boxes[i]->meshShapes.size(); j++) {
					if (boxes[i]->meshShapes[j]->ShadowHit(shadowOrigin, shadowRay)) {
						return ca;
					}
				}
			}
		}
	}

	vec3 cd = objects[objIndex]->diffColour * intensity * max(0.0f, dot(normalize(l), normalize(n)));

	vec3 r = 2 * dot(l, n) * n - l;
	vec3 cs = objects[objIndex]->specColour * intensity * pow(max(0.0f, dot(-normalize(r), rayDirection)), objects[objIndex]->shine);
	vec3 result = ca + cd + cs;
	return result;
}
