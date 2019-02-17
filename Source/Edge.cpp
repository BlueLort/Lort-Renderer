#include "Edge.h"




Edge::Edge(const Vertex & v1, const Vertex & v2)
{
	this->yStart = (uint32_t)ceil(v1.position.arr[1]);
	this->yEnd = (uint32_t)ceil(v2.position.arr[1]);

	float yDist = v2.position.arr[1] - v1.position.arr[1];
	float xDist = v2.position.arr[0] - v1.position.arr[0];

	float yPrestep = yStart - v1.position.arr[1];
	xStep = (float)xDist / (float)yDist;
	current = v1.position.arr[0] + yPrestep * xStep;
}

Edge::Edge(const Edge & other)
{
	this->yStart = other.getYStart();
	this->yEnd = other.getYEnd();
	this->current = other.getCurrent();
	this->xStep = other.getXStep();
}

void Edge::Step()
{
	current += xStep;
}

Edge::~Edge()
{

}
