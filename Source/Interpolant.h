#pragma once
#include "Structures.h"
class Interpolant
{
public:
	inline Vec4f getElements(const int32_t& idx) const { return elements[idx]; }
	inline float getLight(const int32_t& idx)const { return light[idx]; }
	inline Vec4f getElementsXStep()const { return elementsXStep; }
	inline Vec4f getElementsYStep()const { return elementsYStep; }
	inline float getLightXStep()const { return lightXStep; }
	inline float getLightYStep()const { return lightYStep; }

	Interpolant(const Vertex& minYVert, const Vertex& midYVert, const Vertex& maxYVert);
	~Interpolant();
private:
	Vec4f elements[3];//u v depth oneoverw
	float light[3];
	Vec4f lightDir{ 0.5f/1.224f,1.0f/1.224f ,0.5f/1.224f,0.0f };
	float lightXStep;
	float lightYStep;
	Vec4f elementsXStep;//u v oneoverw depth (xSteps)
	Vec4f elementsYStep;////u v oneoverw depth (ySteps)

};

