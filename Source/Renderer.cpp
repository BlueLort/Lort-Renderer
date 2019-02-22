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
	//get color in format ARGB as clearColor 32bit is RGBA 
	uint32_t clearColor32Bit_ARGB = clearColor.get32BitValue()<<24;//get Alpha at front
	clearColor32Bit_ARGB |= clearColor.get32BitValue()>>8;//Remove alpha from the RGBA to be RGB

	SDL_FillRect(surface, NULL,clearColor32Bit_ARGB);
}


void Renderer::drawTriangle(const Vertex&  v1, const Vertex&  v2, const Vertex&  v3) const
{
	MAT4 toScrSpace = MAT4::getScrSpaceTransform(
		static_cast<float>(halfScrWidth)
		,static_cast<float>(halfScrHeight));

	Vertex V1 = v1.Transform(toScrSpace).PrespDivide();
	Vertex V2 = v2.Transform(toScrSpace).PrespDivide();
	Vertex V3 = v3.Transform(toScrSpace).PrespDivide();

	if (V3.pos.arr[1] < V2.pos.arr[1]) {
		std::swap(V3, V2);
	}
	if (V2.pos.arr[1] < V1.pos.arr[1]) {
		std::swap(V2, V1);
	}
	if (V3.pos.arr[1] < V2.pos.arr[1]) {
		std::swap(V3, V2);
	}
	VEC4 p1_p2 = V2.pos - V1.pos;
	VEC4 p1_p3 = V3.pos - V1.pos;
	float crossAns = p1_p2.arr[0] * p1_p3.arr[1] - p1_p2.arr[1] * p1_p3.arr[0];
	bool dir = 0;
	if (crossAns < 0)dir = 1;

	scanTriangle(V1, V2, V3, dir);
}
void Renderer::scanTriangle(const Vertex & minYVert, const Vertex & midYVert, const Vertex & maxYVert, bool direction) const
{
	Gradient grad(minYVert, midYVert, maxYVert);

	Edge topToBot = Edge(grad,minYVert, maxYVert,0);
	Edge topToMid= Edge(grad,minYVert, midYVert,0);
	Edge midToBot = Edge(grad,midYVert, maxYVert,1);

	scanEdges(grad,topToBot, topToMid, direction);
	scanEdges(grad,topToBot, midToBot, direction);
	
	
}

void Renderer::scanEdges(const Gradient& grad,Edge & e1,Edge & e2, bool direction) const
{
	uint32_t yStart =  e2.getYStart();
	uint32_t yEnd = e2.getYEnd();
	for (uint32_t y = yStart; y < yEnd; y++)
	{
		if(!direction)
		drawScanLine(grad,e1, e2, y);
		else {
			drawScanLine(grad,e2, e1, y);
		}
		e1.Step();
		e2.Step();
	}
}
void Renderer::drawScanLine(const Gradient& grad,const Edge& left, const Edge& right, const uint32_t & y) const
{
	uint32_t xMin = static_cast<uint32_t>(ceil(left.getCurrentX()));
	uint32_t xMax = static_cast<uint32_t>(ceil(right.getCurrentX()));

	float xPrestep = xMin - left.getCurrentX();


	//VEC4 color = left.getColor() + grad.getColorXStep()*xPrestep;
	VEC2 texCoords = left.getTexCoords() + grad.getTexCoordsXStep()*xPrestep;
	float oneOverW = left.getOneOverW() + grad.getOneOverWXStep()*xPrestep;
	for (uint32_t x = xMin; x < xMax; x++)
	{
		float W = 1.0f / oneOverW;
		uint16_t X = (int)((texCoords.arr[0] * W) * (float)(myTex.getWidth() - 1) + 0.5f);
		uint16_t Y = (int)((texCoords.arr[1] * W) * (float)(myTex.getHeight() - 1) + 0.5f);
		Color c = myTex.getTexColorAt(X,Y);
		//uint8_t r = static_cast<uint8_t>(color.arr[0]);
		//uint8_t g = static_cast<uint8_t>(color.arr[1]);
		//uint8_t b = static_cast<uint8_t>(color.arr[2]);
		//uint8_t a = static_cast<uint8_t>(color.arr[3]);
		drawPixel(x, y ,c.r, c.g, c.b ,c.a);

		//color = color + grad.getColorXStep();
		texCoords = texCoords + grad.getTexCoordsXStep();
		oneOverW = oneOverW + grad.getOneOverWXStep();
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
