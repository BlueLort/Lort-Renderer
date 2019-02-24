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

	if (!checkDirection(v1, v2, v3))return;//back-face culling

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
	bool dir = checkDirection(V1, V2, V3);

	scanTriangle(V1, V2, V3, dir);
}
void Renderer::scanTriangle(const Vertex & minYVert, const Vertex & midYVert, const Vertex & maxYVert, bool direction) const
{
	Gradient grad(minYVert, midYVert, maxYVert);

	Edge topToBot = Edge(grad,minYVert, maxYVert,0);
	Edge topToMid= Edge(grad,minYVert, midYVert,0);
	Edge midToBot = Edge(grad,midYVert, maxYVert,1);
	if (!direction) {
		scanDrawEdges(topToBot, topToMid);
		scanDrawEdges(topToBot, midToBot);
	}
	else {
		scanDrawEdgesInversed(topToBot, topToMid);
		scanDrawEdgesInversed(topToBot, midToBot);
	}
	
}

void Renderer::scanDrawEdges(Edge & left, Edge & right) const
{
	uint32_t yStart = right.getYStart();
	uint32_t yEnd = right.getYEnd();

	for (uint32_t y = yStart; y < yEnd; y++)
	{
		drawScanLine(left, right, y);
		left.Step();
		right.Step();
	}
}

void Renderer::scanDrawEdgesInversed(Edge & left, Edge & right) const
{
	uint32_t yStart = right.getYStart();
	uint32_t yEnd = right.getYEnd();

	for (uint32_t y = yStart; y < yEnd; y++)
	{
		drawScanLine(right, left, y);
		left.Step();
		right.Step();
	}
}

void Renderer::drawScanLine(const Edge& left, const Edge& right, const uint32_t & y) const
{
	uint32_t xMin = static_cast<uint32_t>(ceil(left.getCurrentX()));
	uint32_t xMax = static_cast<uint32_t>(ceil(right.getCurrentX()));

	float xPrestep = xMin - left.getCurrentX();
	float xDist = right.getCurrentX() - left.getCurrentX();

	///INIT GRADIANTS' STEP 
	///(here to avoid floating point errors which results in array out of bounds)
	//VEC4 colorStep = (right.getColor() - left.getColor()) / xDist;
	VEC2 texCoordsStep = (right.getTexCoords() - left.getTexCoords()) / xDist;
	float oneOverWStep = (right.getOneOverW() - left.getOneOverW()) / xDist;

	///INIT GRADIANTS
	//VEC4 color = left.getColor() + colorStep*xPrestep;
	VEC2 texCoords = left.getTexCoords() + texCoordsStep*xPrestep;
	float oneOverW = left.getOneOverW() + oneOverWStep*xPrestep;
	for (uint32_t x = xMin; x < xMax; x++)
	{
		///GET INFORMATION
		float W = 1.0f / oneOverW;
		uint16_t X = static_cast<uint16_t>(((texCoords.arr[0] * W) * static_cast<float>((myTex.getWidth() - 1) + 0.5f)));
		uint16_t Y = static_cast<uint16_t>(((texCoords.arr[1] * W) * static_cast<float>((myTex.getHeight() - 1) + 0.5f)));
		Color c = myTex.getTexColorAt(X,Y);
		//uint8_t r = static_cast<uint8_t>(color.arr[0]);
		//uint8_t g = static_cast<uint8_t>(color.arr[1]);
		//uint8_t b = static_cast<uint8_t>(color.arr[2]);
		//uint8_t a = static_cast<uint8_t>(color.arr[3]);
		
		///APPLY TO PIXEL
		drawPixel(x, y ,c.r, c.g, c.b ,c.a);

		///INCREASE FACTORS 
		//color = color + colorStep;
		texCoords = texCoords + texCoordsStep;
		oneOverW = oneOverW + oneOverWStep;
	}
}

void Renderer::drawPixel(const uint32_t & x, const uint32_t & y, const uint8_t & r, const uint8_t & g, const uint8_t & b, const uint8_t & a) const
{
	*(static_cast<uint32_t*>(surface->pixels) + surface->w * y + x) = SDL_MapRGBA(surface->format, r, g, b, a);
}
inline bool Renderer::checkDirection(const Vertex & v1, const Vertex & v2, const Vertex & v3) const {
	VEC4 p1_p2 = v2.pos - v1.pos;
	VEC4 p1_p3 = v3.pos - v1.pos;
	float crossZ = p1_p2.Cross(p1_p3).arr[2];// Z is the axis to tell us if the shape facing the camera or not
	return (crossZ < 0.0f);
}

void Renderer::updateScreen() const
{
	SDL_UnlockSurface(surface);
	SDL_UpdateWindowSurface(window);
}
