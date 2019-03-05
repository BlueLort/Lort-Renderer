#pragma once
#include <iostream>
#include "Structures.h"
#include "Gradient.h"
class Edge
{
public:
	inline uint32_t getYStart() const{ return yStart; }
	inline uint32_t getYEnd() const{ return yEnd; }
	inline uint32_t getXStep() const { return xStep; }
	inline uint32_t getCurrentX() const{ return currentX; }
	inline uint32_t getTexCoordsU() const { return texCoordsU; }
	inline uint32_t getTexCoordsV() const { return texCoordsV; }
	inline uint32_t getOneOverW() const { return oneOverW; }

	Edge(const Gradient& grad,const Vertex&  maxYVert, const Vertex& minYVert, const int8_t& minYVertIndex);
	void Step();
	~Edge();
private:
	uint32_t yStart;
	uint32_t yEnd;
	uint32_t xStep;
	uint32_t xScale;
	uint32_t yScale;
	uint32_t currentX;
	uint32_t texCoordsU;
	uint32_t texCoordsUStep;
	uint32_t texCoordsV;
	uint32_t texCoordsVStep;
	uint32_t oneOverW;
	uint32_t oneOverWStep;

	
};

