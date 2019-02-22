#pragma once
#include "Structures.h"
class Gradient
{
public:
	inline VEC4 getColorXStep() const { return colorXStep; }
	inline VEC4 getColorYStep() const { return colorYStep; }
	inline VEC2 getTexCoordsXStep() const { return texCoordsXStep; }
	inline VEC2 getTexCoordsYStep() const { return texCoordsYStep; }
	inline float getOneOverWXStep() const { return oneOverWXStep; }
	inline float getOneOverWYStep() const { return oneOverWYStep; }
	inline VEC4 getColor(const int8_t& index) const { return colArr[index]; }
	inline VEC2 getTexCoords(const int8_t& index) const { return texCoords[index]; }
	inline float getOneOverW(const int8_t& index) const { return oneOverW[index]; }

	Gradient(const Vertex& minYVert, const Vertex& midYVert, const Vertex& maxYVert);
	~Gradient();
private:
	VEC4 colArr[3];
	float oneOverW[3];//For Perspective Texturing
	VEC2 texCoords[3];


	VEC4 colorXStep;
	VEC4 colorYStep;
	VEC2 texCoordsXStep;
	VEC2 texCoordsYStep;
	float oneOverWXStep;
	float oneOverWYStep;

};

