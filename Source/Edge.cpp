#include "Edge.h"




Edge::Edge(const Interpolant& inter, const Vertex&  minYVert, const Vertex& maxYVert, const int8_t& minYVertIndex)
{
	this->yStart = static_cast<int32_t>(minYVert.pos[1]+0.99f);
	this->yEnd = static_cast<int32_t>(maxYVert.pos[1]+0.99f);

	float yDist = maxYVert.pos[1] - minYVert.pos[1];
	float xDist = maxYVert.pos[0] - minYVert.pos[0];

	float yPreStep = yStart - minYVert.pos[1];
	this->xStep = xDist / yDist;
	this->currentX = minYVert.pos[0] + yPreStep * xStep;

	float xPreStep = currentX - minYVert.pos[0];

	this->elements = inter.getElements(minYVertIndex) + inter.getElementsYStep()*yPreStep + inter.getElementsXStep()*xPreStep;
	this->elementsStep = inter.getElementsYStep() + inter.getElementsXStep()*xStep;

	this->light = inter.getLight(minYVertIndex) + inter.getLightYStep()*yPreStep + inter.getLightXStep()*xPreStep;
	this->lightStep = inter.getLightYStep() + inter.getLightXStep()*xStep;
}


 void Edge::Step()
{
	 this->currentX += xStep;
	 this->elements += elementsStep;
	 this->light += lightStep;
}

Edge::~Edge()
{

}
