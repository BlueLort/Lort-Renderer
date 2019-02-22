#pragma once
#include <iostream>
#include "Structures.h"
#include "Gradient.h"
class Edge
{
public:
	inline uint32_t getYStart() const{ return yStart; }
	inline uint32_t getYEnd() const{ return yEnd; }
	inline float getXStep() const { return xStep; }
	inline float getCurrentX() const{ return currentX; }
	inline VEC4 getColor() const { return color; }
	inline VEC4 getColorStep() const { return colorStep; }
	inline VEC2 getTexCoords() const { return texCoords; }
	inline VEC2 getTexCoordsStep() const { return texCoordsStep; }
	inline float getOneOverW() const { return oneOverW; }
	inline float getOneOverWStep() const { return oneOverWStep; }

	Edge(const Gradient& grad,const Vertex&  maxYVert, const Vertex& minYVert, const int8_t& minYVertIndex);
	void Step();
	~Edge();
private:
	uint32_t yStart;
	uint32_t yEnd;
	float xStep;
	float currentX;
	VEC4 color;
	VEC4 colorStep;
	float oneOverW;
	float oneOverWStep;
	VEC2 texCoords;
	VEC2 texCoordsStep;
	
};

