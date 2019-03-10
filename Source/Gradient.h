#pragma once
#include "Structures.h"
class Gradient
{
public:
	inline int32_t getOneOverWXStep() const { return WXStep; }
	inline int32_t getOneOverWYStep() const { return WYStep; }
		   
	inline int32_t getTexCoordsUXStep() const { return UXStep; }
	inline int32_t getTexCoordsUYStep() const { return UYStep; }
	inline int32_t getTexCoordsVXStep() const { return  VXStep;}
	inline int32_t getTexCoordsVYStep() const { return  VYStep; }

	inline float getOneOverW(const int8_t& index) const { return oneOverW[index]; }
	inline float getTexCoordsU(const int8_t& index) const { return oneOverW[index]; }
	inline float getTexCoordsV(const int8_t& index) const { return oneOverW[index]; }

	Gradient(const Vertex& minYVert, const Vertex& midYVert, const Vertex& maxYVert);
	~Gradient();
private:
	float oneOverW[3];//For Perspective Texturing
	float texCoordsU[3];
	float texCoordsV[3];

	int32_t UXStep;
	int32_t UYStep;
	int32_t VXStep;
	int32_t VYStep;
	int32_t WXStep;
	int32_t WYStep;

};

