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

	oneOverW[0] = 1.0f/minYVert.pos.arr[3];
	oneOverW[1] = 1.0f /midYVert.pos.arr[3];
	oneOverW[2] = 1.0f /maxYVert.pos.arr[3];
	texCoordsU[0] = minYVert.texCoords.arr[0] * oneOverW[0];
	texCoordsU[1] = midYVert.texCoords.arr[0] * oneOverW[1];
	texCoordsU[2] = maxYVert.texCoords.arr[0] * oneOverW[2];
	texCoordsV[0] = minYVert.texCoords.arr[1] * oneOverW[0];
	texCoordsV[1] = midYVert.texCoords.arr[1] * oneOverW[1];
	texCoordsV[2] = maxYVert.texCoords.arr[1] * oneOverW[2];

	float x0 = minYVert.pos.arr[0];	float y0 = minYVert.pos.arr[1];
	float x1 = midYVert.pos.arr[0];	float y1 = midYVert.pos.arr[1];
	float x2 = maxYVert.pos.arr[0];	float y2 = maxYVert.pos.arr[1];


	float oneOver_dx = 1.0f /(((x1 - x2)*(y0 -y2))-((x0 - x2)*(y1 - y2)));
	float oneOver_dy = - oneOver_dx;

	float dTexCoordsUDX = (((texCoordsU[1] - texCoordsU[2])*(y0 - y2)) - ((texCoordsU[0] - texCoordsU[2])*(y1 - y2)));
	float dTexCoordsUDY = (((texCoordsU[1] - texCoordsU[2])*(x0 - x2)) - ((texCoordsU[0] - texCoordsU[2])*(x1 - x2)));
	float dTexCoordsVDX = (((texCoordsV[1] - texCoordsV[2])*(y0 - y2)) - ((texCoordsV[0] - texCoordsV[2])*(y1 - y2)));
	float dTexCoordsVDY = (((texCoordsV[1] - texCoordsV[2])*(x0 - x2)) - ((texCoordsV[0] - texCoordsV[2])*(x1 - x2)));
	float dOneOverWDX = (((oneOverW[1] - oneOverW[2])*(y0 - y2)) - ((oneOverW[0] - oneOverW[2])*(y1 - y2)));
	float dOneOverWDY = (((oneOverW[1] - oneOverW[2])*(x0 - x2)) - ((oneOverW[0] - oneOverW[2])*(x1 - x2)));


	this->texCoordsUXStep = static_cast<uint32_t>(dTexCoordsUDX * oneOver_dx*PVAL);
	this->texCoordsUYStep = static_cast<uint32_t>(dTexCoordsUDY * oneOver_dy*PVAL);
	this->texCoordsVXStep = static_cast<uint32_t>(dTexCoordsVDX * oneOver_dx*PVAL);
	this->texCoordsVYStep = static_cast<uint32_t>(dTexCoordsVDY * oneOver_dy*PVAL);
	this->oneOverWXStep = static_cast<uint32_t>(dOneOverWDX * oneOver_dx*PVAL);
	this->oneOverWYStep = static_cast<uint32_t>(dOneOverWDY * oneOver_dy*PVAL);


}

Gradient::~Gradient()
{
}
