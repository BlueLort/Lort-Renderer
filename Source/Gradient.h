#pragma once
#include "Structures.h"
class Gradient
{
public:
	inline uint32_t getOneOverWXStep() const { return oneOverWXStep; }
	inline uint32_t getOneOverWYStep() const { return oneOverWYStep; }
		   
	inline uint32_t getTexCoordsUXStep() const { return texCoordsUXStep; }
	inline uint32_t getTexCoordsUYStep() const { return texCoordsUYStep; }
	inline uint32_t getTexCoordsVXStep() const { return  texCoordsVXStep;}
	inline uint32_t getTexCoordsVYStep() const { return  texCoordsVYStep; }

	inline float getOneOverW(const int8_t& index) const { return oneOverW[index]; }
	inline float getTexCoordsU(const int8_t& index) const { return oneOverW[index]; }
	inline float getTexCoordsV(const int8_t& index) const { return oneOverW[index]; }

	Gradient(const Vertex& minYVert, const Vertex& midYVert, const Vertex& maxYVert);
	~Gradient();
private:
	float oneOverW[3];//For Perspective Texturing
	float texCoordsU[3];
	float texCoordsV[3];

	uint32_t texCoordsUXStep;
	uint32_t texCoordsUYStep;
	uint32_t texCoordsVXStep;
	uint32_t texCoordsVYStep;
	uint32_t oneOverWXStep;
	uint32_t oneOverWYStep;

};

