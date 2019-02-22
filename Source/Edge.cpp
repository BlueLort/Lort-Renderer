#include "Edge.h"




Edge::Edge(const Gradient& grad, const Vertex&  minYVert, const Vertex& maxYVert, const int8_t& minYVertIndex)
{
	this->yStart = static_cast<uint32_t>(ceil(minYVert.pos.arr[1]));
	this->yEnd = static_cast<uint32_t>(ceil(maxYVert.pos.arr[1]));

	float yDist = maxYVert.pos.arr[1] - minYVert.pos.arr[1];
	float xDist = maxYVert.pos.arr[0] - minYVert.pos.arr[0];

	float yPreStep = yStart - minYVert.pos.arr[1];
	this->xStep = xDist / yDist;
	this->currentX = minYVert.pos.arr[0] + yPreStep * xStep;

	float xPreStep = currentX - minYVert.pos.arr[0];

	this->color = grad.getColor(minYVertIndex) + grad.getColorYStep()*yPreStep + grad.getColorXStep()*xPreStep;
	this->colorStep = grad.getColorYStep() + grad.getColorXStep()*xStep;
	this->texCoords = grad.getTexCoords(minYVertIndex) + grad.getTexCoordsYStep()*yPreStep + grad.getTexCoordsXStep()*xPreStep;
	this->texCoordsStep = grad.getTexCoordsYStep() + grad.getTexCoordsXStep()*xStep;
	this->oneOverW= grad.getOneOverW(minYVertIndex) + grad.getOneOverWYStep()*yPreStep + grad.getOneOverWXStep()*xPreStep;
	this->oneOverWStep = grad.getOneOverWYStep() + grad.getOneOverWXStep()*xStep;
}


void Edge::Step()
{
	this->currentX = currentX + xStep;
	this->color = color + colorStep;
	this->texCoords = texCoords + texCoordsStep;
	this->oneOverW = oneOverW + oneOverWStep;
	
}

Edge::~Edge()
{

}
