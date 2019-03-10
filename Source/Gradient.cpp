#include "Gradient.h"
/*
		         
                 		
    		 p0-----p4
    	    /| \   /
    	   / |   p1
    	  /  |  / 
    	 /   p3
    	/   / 
	   /  /										<----- assume that is a line
      / /
     //
    p3

*/
///INTERPOLATING ACCORDING TO THE SHAPE ABOVE...
// p1,p2,p3,p4 same line.
// p0,p1,p2 are the triangle points, p4 & p3 helping points.
// dc/dx = (c4-c0)/(x4-x0)   ,  dc/dy = (c3-c0)/(y3-y0);
// From these we can make our interpolation formula.

Gradient::Gradient(const Vertex & minYVert, const Vertex & midYVert, const Vertex & maxYVert)
{

	oneOverW[0] = 1.0f / minYVert.pos.arr[3];
	oneOverW[1] = 1.0f / midYVert.pos.arr[3];
	oneOverW[2] = 1.0f / maxYVert.pos.arr[3];
	texCoordsU[0] = minYVert.texCoords.arr[0] * oneOverW[0];
	texCoordsU[1] = midYVert.texCoords.arr[0] * oneOverW[1];
	texCoordsU[2] = maxYVert.texCoords.arr[0] * oneOverW[2];
	texCoordsV[0] = minYVert.texCoords.arr[1] * oneOverW[0];
	texCoordsV[1] = midYVert.texCoords.arr[1] * oneOverW[1];
	texCoordsV[2] = maxYVert.texCoords.arr[1] * oneOverW[2];

	int32_t x0 = static_cast<int32_t>(minYVert.pos.arr[0] + 0.99f);	
	int32_t x1 = static_cast<int32_t>(midYVert.pos.arr[0] + 0.99f);	
	int32_t x2 = static_cast<int32_t>(maxYVert.pos.arr[0] + 0.99f);	
	int32_t y0 = static_cast<int32_t>(minYVert.pos.arr[1] + 0.99f);
	int32_t y1 = static_cast<int32_t>(midYVert.pos.arr[1] + 0.99f);
	int32_t y2 = static_cast<int32_t>(maxYVert.pos.arr[1] + 0.99f);

	float oneOver_dx = 1.0f /(((x1 - x2)*(y0 -y2))-((x0 - x2)*(y1 - y2)));
	float oneOver_dy = - oneOver_dx;

	float DUDX = (((texCoordsU[1] - texCoordsU[2])*(y0 - y2)) - ((texCoordsU[0] - texCoordsU[2])*(y1 - y2)));
	float DUDY = (((texCoordsU[1] - texCoordsU[2])*(x0 - x2)) - ((texCoordsU[0] - texCoordsU[2])*(x1 - x2)));
	float DVDX = (((texCoordsV[1] - texCoordsV[2])*(y0 - y2)) - ((texCoordsV[0] - texCoordsV[2])*(y1 - y2)));
	float DVDY = (((texCoordsV[1] - texCoordsV[2])*(x0 - x2)) - ((texCoordsV[0] - texCoordsV[2])*(x1 - x2)));
	float DWDX = (((oneOverW[1] - oneOverW[2])*(y0 - y2)) - ((oneOverW[0] - oneOverW[2])*(y1 - y2)));
	float DWDY = (((oneOverW[1] - oneOverW[2])*(x0 - x2)) - ((oneOverW[0] - oneOverW[2])*(x1 - x2)));


	this->UXStep = static_cast<uint32_t>(DUDX * oneOver_dx * FP_FVAL);
	this->UYStep = static_cast<uint32_t>(DUDY * oneOver_dy * FP_FVAL);
	this->VXStep = static_cast<uint32_t>(DVDX * oneOver_dx * FP_FVAL);
	this->VYStep = static_cast<uint32_t>(DVDY * oneOver_dy * FP_FVAL);
	this->WXStep = static_cast<uint32_t>(DWDX * oneOver_dx * FP_FVAL);
	this->WYStep = static_cast<uint32_t>(DWDY * oneOver_dy * FP_FVAL);


}

Gradient::~Gradient()
{
}
