#pragma once
#include <iostream>
#include "Structures.h"
class Edge
{
public:
	inline uint32_t getYStart() const{ return yStart; }
	inline uint32_t getYEnd() const{ return yEnd; }
	inline float getXStep() const { return xStep; }
	inline float getCurrent() const{ return current; }
	Edge(const Vertex& v1, const Vertex& v2);
	Edge(const Edge& other);
	void Step();
	~Edge();
private:
	uint32_t yStart;
	uint32_t yEnd;
	float xStep;
	float current;
};

