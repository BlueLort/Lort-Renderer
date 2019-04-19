#pragma once
#include <iostream>
#include "Structures.h"
#include "Interpolant.h"
class Edge
{
public:
	inline uint32_t getYStart() const{ return yStart; }
	inline uint32_t getYEnd() const{ return yEnd; }
	inline float getXStep() const { return xStep; }
	inline float getCurrentX() const{ return currentX; }
	inline Vec4f getElements() const { return elements; }
	inline float getLight() const { return light; }
	inline Vec4f getElementsStep() const { return elementsStep; }
	inline float getLightStep() const { return lightStep; }
	Edge(const Interpolant& inter,const Vertex&  maxYVert, const Vertex& minYVert, const int8_t& minYVertIndex);
	void Step();
	~Edge();
private:
	int32_t yStart;
	int32_t yEnd;
	float xStep;
	float currentX;

	Vec4f elements;//u v depth oneOverW
	float light;
	Vec4f elementsStep;//uStep vStep depthStep oneOverWStep;
	float lightStep;
};

