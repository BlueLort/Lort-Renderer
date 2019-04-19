#pragma once
#include <fvec.h>
#include <cmath>
#include <iostream>
#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)
#define LERP(a,b,f) ((b - a)*f + a)

//Fixed Point Arithmetic and constants
#define FP_FVAL 1048576.0f //fixed point float value
#define FP_IVAL 1048576 //fixed point integer value
#define UV_WRAP 1048575 
#define FP_SCL 20 //fixed point precision scale value
#define HALF_FP_SCL 10
#define MULFP(a,b) (( (a>>HALF_FP_SCL)  *  (b>>HALF_FP_SCL) )) 
#define DIVFP(a,b)  (((a<<5) / (b>>5) )<<10)
#include "Vec4f.h"
#include "Mat4x4f.h"
struct Vertex {
	Vec4f pos;
	Vec4f texCoords;
	Vec4f normal;
	Vec4f tangentU;
	Vec4f bitangent;
	Vertex() {
		pos.insert(1.0f,3);
	}
	Vertex(const float& x,const float& y,const float& z
		,const float& u, const float& v
		,const float& nx,const float& ny,const float &nz) {
		this->pos = Vec4f(x, y,z,1.0f);
		this->texCoords = Vec4f(u,v,0.0f,0.0f);
		this->normal = Vec4f(nx, ny, nz,0.0f);
	}
	Vertex(const Vec4f& pos,const Vec4f& texCoords,const Vec4f& normal) {
		this->pos = pos;
		this->texCoords = texCoords;
		this->normal = normal;
	}
	Vertex(const Vec4f& pos, const Vec4f& texCoords) {
		this->pos = pos;
		this->texCoords = texCoords;
		this->normal = Vec4f(0.0f, 1.0f, 0.0f,0.0f);
	}
	Vertex Transform(const Mat4x4f& transMat,const Mat4x4f normalTransMat) const
	{
		return Vertex(transMat.Transform(pos), this->texCoords, normalTransMat.Transform(normal));
	}

	Vertex PrespDivide() const
	{
		Vertex v;
		float w = pos.getW();
		float oneOverW = 1 / w;
		v.pos = pos * oneOverW;
		v.pos.insert(w, 3);
		v.texCoords = this->texCoords;
		v.normal = this->normal;
		return v;
	}
	Vertex Lerp(Vertex other, float lerpAmt)
	{
		return Vertex(lerp(pos,other.pos, lerpAmt),lerp(texCoords,other.texCoords, lerpAmt),lerp(normal,other.normal,lerpAmt));
	}

};

static inline void calculateTangents(Vertex& v1, Vertex& v2, Vertex& v3) {
	Vec4f tangent;
	Vec4f bitangent;
	Vec4f edge1 = v1.pos - v2.pos;
	Vec4f edge2 = v3.pos - v2.pos;
	Vec4f deltaUV1 = v1.texCoords - v2.texCoords;
	Vec4f deltaUV2 = v3.texCoords - v2.texCoords;
	float f = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

	tangent.insert( f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]) ,0);
	tangent.insert( f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]) ,1);
	tangent.insert( f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]) ,2);
	tangent = tangent.normalized();
											 
	bitangent.insert(f * (-deltaUV2[0]* edge1[0] + deltaUV1[0] * edge2[0]), 0);
	bitangent.insert(f * (-deltaUV2[0]* edge1[1] + deltaUV1[0] * edge2[1]), 1);
	bitangent.insert(f * (-deltaUV2[0]* edge1[2] + deltaUV1[0] * edge2[2]), 2);
	bitangent = bitangent.normalized();
	v1.tangentU = tangent; v1.bitangent = bitangent;
	v2.tangentU = tangent; v2.bitangent = bitangent;
	v3.tangentU = tangent; v3.bitangent = bitangent;
}

static inline int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
static inline float clampValue(float input,float min,float max) {
	input = input > max ? max : input;
	input = input < min ? min : input;
	return input;
}