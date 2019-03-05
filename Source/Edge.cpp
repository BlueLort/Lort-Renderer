#include "Edge.h"




Edge::Edge(const Gradient& grad, const Vertex&  minYVert, const Vertex& maxYVert, const int8_t& minYVertIndex)
{
	this->yStart = static_cast<uint32_t>(minYVert.pos.arr[1]+0.99f);
	this->yEnd = static_cast<uint32_t>(maxYVert.pos.arr[1]+0.99f);

	uint32_t xStart = static_cast<uint32_t>(minYVert.pos.arr[0] + 0.99f);
	uint32_t xEnd = static_cast<uint32_t>(maxYVert.pos.arr[0] + 0.99f);

	uint32_t yDist = yEnd - yStart;
	uint32_t xDist = xEnd - xStart;
	
	uint32_t v0 = static_cast<uint32_t>(minYVert.texCoords.arr[1] * PVAL);    
	uint32_t v1 = static_cast<uint32_t>(maxYVert.texCoords.arr[1] * PVAL);    


	this->xScale = 0;
	if (xDist != 0)xScale = (1 << PSCAL) / (xDist);

	this->yScale = 0;
	if (yDist != 0)yScale = (1 << PSCAL) / (yDist);

	this->currentX = (xStart << PSCAL);
	this->xStep = xDist * yScale;


	this->texCoordsU = static_cast<uint32_t>(grad.getTexCoordsU(minYVertIndex)*PVAL);
	this->texCoordsUStep = grad.getTexCoordsUYStep() + MULFP(grad.getTexCoordsUXStep(), xStep);
	this->texCoordsV = static_cast<uint32_t>(grad.getTexCoordsV(minYVertIndex)*PVAL);
	this->texCoordsVStep = grad.getTexCoordsVYStep() + MULFP(grad.getTexCoordsVXStep(), xStep);
	this->oneOverW= static_cast<uint32_t>(grad.getOneOverW(minYVertIndex)*PVAL);
	this->oneOverWStep = grad.getOneOverWYStep() + MULFP(grad.getOneOverWXStep(), xStep);

}


void Edge::Step()
{
	currentX = currentX + xStep;
	texCoordsU = texCoordsU + texCoordsUStep;
	texCoordsV = texCoordsV + texCoordsVStep;
	oneOverW = oneOverW + oneOverWStep;

}

Edge::~Edge()
{

}
