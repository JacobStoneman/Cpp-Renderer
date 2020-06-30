#include "Shape.h"

Shape::Shape(vec3 centre, vec3 diffColour, float shine, vec3 specColour)
{
	this->specColour = vec3(0.7,0.7,0.7); 
	this->shine = shine;
	this->centre = centre;
	this->diffColour = diffColour; 
}

Shape::~Shape()
{
}
