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
    colArr[0] = VEC4(minYVert.col.r, minYVert.col.g, minYVert.col.b, minYVert.col.a);
	colArr[1] = VEC4(midYVert.col.r, midYVert.col.g, midYVert.col.b, midYVert.col.a);
    colArr[2] = VEC4(maxYVert.col.r, maxYVert.col.g, maxYVert.col.b, maxYVert.col.a);
	oneOverW[0] = 1.0f/minYVert.pos.arr[3];
	oneOverW[1] = 1.0f/midYVert.pos.arr[3];
	oneOverW[2] = 1.0f/maxYVert.pos.arr[3];
	texCoords[0] = minYVert.texCoords * oneOverW[0];
	texCoords[1] = midYVert.texCoords * oneOverW[1];
	texCoords[2] = maxYVert.texCoords * oneOverW[2];


	float x0 = minYVert.pos.arr[0];	float y0 = minYVert.pos.arr[1];
	float x1 = midYVert.pos.arr[0];	float y1 = midYVert.pos.arr[1];
	float x2 = maxYVert.pos.arr[0];	float y2 = maxYVert.pos.arr[1];


	float oneOver_dx = 1.0f /(((x1 - x2)*(y0 -y2))-((x0 - x2)*(y1 - y2)));
	float oneOver_dy = - oneOver_dx;

	VEC4 dColorDX = (((colArr[1] - colArr[2])*(y0 - y2)) - ((colArr[0] - colArr[2])*(y1 - y2)));
	VEC4 dColorDY = (((colArr[1] - colArr[2])*(x0 - x2)) - ((colArr[0] - colArr[2])*(x1 - x2)));
	VEC2 dTexCoordsDX = (((texCoords[1] - texCoords[2])*(y0 - y2)) - ((texCoords[0] - texCoords[2])*(y1 - y2)));
	VEC2 dTexCoordsDY = (((texCoords[1] - texCoords[2])*(x0 - x2)) - ((texCoords[0] - texCoords[2])*(x1 - x2)));
	float dOneOverWDX = (((oneOverW[1] - oneOverW[2])*(y0 - y2)) - ((oneOverW[0] - oneOverW[2])*(y1 - y2)));
	float dOneOverWDY = (((oneOverW[1] - oneOverW[2])*(x0 - x2)) - ((oneOverW[0] - oneOverW[2])*(x1 - x2)));


	this->colorXStep = dColorDX * oneOver_dx;
	this->colorYStep = dColorDY * oneOver_dy;
	this->texCoordsXStep = dTexCoordsDX * oneOver_dx;
	this->texCoordsYStep = dTexCoordsDY * oneOver_dy;
	this->oneOverWXStep = dOneOverWDX * oneOver_dx;
	this->oneOverWYStep = dOneOverWDY * oneOver_dy;



}

Gradient::~Gradient()
{
}
