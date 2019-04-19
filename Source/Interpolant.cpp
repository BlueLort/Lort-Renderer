#include "Interpolant.h"
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

 Interpolant::Interpolant(const Vertex & minYVert, const Vertex & midYVert, const Vertex & maxYVert)
{
	float oneOverW = 1.0f / minYVert.pos[3];
	this->elements[0].setVec(minYVert.texCoords[0] * oneOverW, //u
		minYVert.texCoords[1] * oneOverW, // v
		minYVert.pos[2], // depth
		oneOverW); //oneOverW
	oneOverW = 1.0f / midYVert.pos[3];
	this->elements[1].setVec(midYVert.texCoords[0] * oneOverW,//u
		midYVert.texCoords[1] * oneOverW,// v
		midYVert.pos[2],// depth
		oneOverW);//oneOverW
	oneOverW = 1.0f / maxYVert.pos[3];
	this->elements[2].setVec(maxYVert.texCoords[0] * oneOverW,//u
		maxYVert.texCoords[1] * oneOverW,// v
		maxYVert.pos[2],// depth
		oneOverW);//oneOverW

	float x0 = minYVert.pos[0];	float y0 = minYVert.pos[1];
	float x1 = midYVert.pos[0];	float y1 = midYVert.pos[1];
	float x2 = maxYVert.pos[0];	float y2 = maxYVert.pos[1];

	float dotVal = dot(minYVert.normal, lightDir);
	light[0] = clampValue(dotVal, 0.0f, 1.0f) * 0.95f + 0.05f;
	dotVal = dot(midYVert.normal, lightDir);			  
	light[1] = clampValue(dotVal, 0.0f, 1.0f) * 0.95f + 0.05f;
	dotVal = dot(maxYVert.normal, lightDir);			  
	light[2] = clampValue(dotVal, 0.0f, 1.0f) * 0.95f + 0.05f;

	float oneOver_dx = 1.0f /(((x1 - x2)*(y0 -y2))-((x0 - x2)*(y1 - y2)));
	float oneOver_dy = - oneOver_dx;

	elementsXStep= (((elements[1] - elements[2])*(y0 - y2)) - ((elements[0] - elements[2])*(y1 - y2))) * oneOver_dx;
	elementsYStep= (((elements[1] - elements[2])*(x0 - x2)) - ((elements[0] - elements[2])*(x1 - x2))) * oneOver_dy;
	lightXStep = (((light[1] - light[2])*(y0 - y2)) - ((light[0] - light[2])*(y1 - y2))) * oneOver_dx;
	lightYStep = (((light[1] - light[2])*(x0 - x2)) - ((light[0] - light[2])*(x1 - x2))) * oneOver_dy;



}

Interpolant::~Interpolant()
{
}
