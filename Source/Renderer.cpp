#include "Renderer.h"
Renderer Renderer::rendererInstance =Renderer();

Renderer::Renderer() : scrWidth(1024),scrHeight(768)
{
}


Renderer::Renderer(SDL_Window * window, SDL_Surface* surface, const uint32_t& scrWidth, const uint32_t& scrHeight)
{
	this->window = window;
	this->surface = surface;
	this->scrWidth = scrWidth;
	this->scrHeight = scrHeight;
	this->halfScrWidth = 1/2*scrWidth;
	this->halfScrHeight = 1/2*scrHeight;
}

Renderer::~Renderer()
{
}


void Renderer::clearScreen() const
{
	SDL_LockSurface(surface);
	for (uint32_t i = 0; i < scrWidth*scrHeight; i++) {
		*(static_cast<uint32_t*>(surface->pixels) + i) = SDL_MapRGBA(surface->format, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	}
}


void Renderer::drawTriangle(const Vertex&  v1, const Vertex&  v2, const Vertex&  v3) const
{
	MAT4 toScrSpace = MAT4::getScrSpaceTransform(halfScrWidth, halfScrHeight);

	Vertex V1 = v1.Transform(toScrSpace).PrespDivide();
	Vertex V2 = v2.Transform(toScrSpace).PrespDivide();
	Vertex V3 = v3.Transform(toScrSpace).PrespDivide();

	if (V3.position.SSE_VEC[1] < V2.position.SSE_VEC[1]) {
		std::swap(V3, V2);
	}
	if (V2.position.SSE_VEC[1] < V1.position.SSE_VEC[1]) {
		std::swap(V2, V1);
	}
	if (V3.position.SSE_VEC[1] < V2.position.SSE_VEC[1]) {
		std::swap(V3, V2);
	}
	VEC4 p1_p2 = V2.position - V1.position;
	VEC4 p1_p3 = V3.position - V1.position;
	float crossAns = p1_p2.SSE_VEC[0] * p1_p3.SSE_VEC[1] - p1_p2.SSE_VEC[1] * p1_p3.SSE_VEC[0];
	bool dir = 0;
	if (crossAns < 0)dir = 1;

	scanTriangle(V1, V2, V3, dir);
}
void Renderer::scanTriangle(const Vertex & v1, const Vertex & v2, const Vertex & v3, bool direction) const
{
	Edge topToBot = Edge(v1, v3);
	Edge topToMid= Edge(v1, v2);
	Edge midToBot = Edge(v2, v3);

	scanEdges(topToBot, midToBot, direction);
	scanEdges(topToBot, topToMid, direction);
	
}

void Renderer::scanEdges(const Edge & e1, const Edge & e2, bool direction) const
{
	Edge left = e1;
	Edge right = e2;
	if (direction)
	{
		std::swap(left, right);
	}
	uint32_t yStart = e2.getYStart();
	uint32_t yEnd = e2.getYEnd();
	for (uint32_t i = yStart; i < yEnd; i++)
	{
		drawScanLine(left, right, i);
		left.Step();
		right.Step();
	}
}
void Renderer::drawScanLine(const Edge& left, const Edge& right, const uint32_t & j) const
{
	uint32_t xMin = (uint32_t)ceilf(left.getCurrent());
	uint32_t xMax = (uint32_t)ceilf(right.getCurrent());

	for (uint32_t i = xMin; i < xMax; i++)
	{
		drawPixel(i, j, (uint8_t)0xFF, (uint8_t)0xFF, (uint8_t)0xFF, (uint8_t)0xFF);
	}
}
void Renderer::drawPixel(const uint32_t & x, const uint32_t & y, const uint8_t & r, const uint8_t & g, const uint8_t & b, const uint8_t & a) const
{
	*(static_cast<uint32_t*>(surface->pixels) + surface->w * y + x) = SDL_MapRGBA(surface->format, r, g, b, a);
}


void Renderer::updateScreen() const
{
	SDL_UnlockSurface(surface);
	SDL_UpdateWindowSurface(window);
}
