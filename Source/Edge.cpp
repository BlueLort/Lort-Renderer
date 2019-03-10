#include "Edge.h"




Edge::Edge(const Gradient& grad, const Vertex&  minYVert, const Vertex& maxYVert, const int8_t& minYVertIndex)
{
	this->yStart = static_cast<uint32_t>(minYVert.pos.arr[1]+0.99f);
	this->yEnd = static_cast<uint32_t>(maxYVert.pos.arr[1]+0.99f);

	uint32_t xStart = static_cast<uint32_t>(minYVert.pos.arr[0] + 0.99f);
	uint32_t xEnd = static_cast<uint32_t>(maxYVert.pos.arr[0] + 0.99f);

	int32_t yDist = yEnd - yStart;
	int32_t xDist = xEnd - xStart;

	this->xScale = 0;
	if (xDist != 0)xScale = (FP_IVAL) / (xDist);
	
	this->yScale = 0;
	if (yDist != 0)yScale = (FP_IVAL) / (yDist);
	
	this->xStep = xDist * yScale;
	this->currentX = (xStart << FP_SCL);
	



	this->texCoordsU = static_cast<int32_t>(grad.getTexCoordsU(minYVertIndex)*FP_FVAL);
	this->texCoordsUStep = grad.getTexCoordsUYStep() + MULFP(grad.getTexCoordsUXStep(), xStep);
	this->texCoordsV = static_cast<int32_t>(grad.getTexCoordsV(minYVertIndex)*FP_FVAL);
	this->texCoordsVStep = grad.getTexCoordsVYStep() + MULFP(grad.getTexCoordsVXStep(), xStep);
	this->oneOverW = static_cast<int32_t>(grad.getOneOverW(minYVertIndex)*FP_FVAL);
	this->oneOverWStep = grad.getOneOverWYStep() + MULFP(grad.getOneOverWXStep(), xStep);

}


void Edge::Step()
{
	currentX += xStep;
	texCoordsU += texCoordsUStep;
	texCoordsV += texCoordsVStep;
	oneOverW += oneOverWStep;

}

Edge::~Edge()
{

}
