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


	SDL_FillRect(surface, NULL,clearColor);
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
	Gradient grad=Gradient(minYVert, midYVert, maxYVert);

	Edge topToBot = Edge(grad,minYVert, maxYVert, 0);
	Edge topToMid = Edge(grad,minYVert, midYVert, 0);
	Edge midToBot = Edge(grad,midYVert, maxYVert, 1);

	
	
	if (!direction) {
		drawEdgeToEdge(grad,topToBot, topToMid);
		drawEdgeToEdge(grad,topToBot, midToBot);
	}
	else {
		drawEdgeToEdgeInversed(grad,topToBot, topToMid);
		drawEdgeToEdgeInversed(grad,topToBot, midToBot);
	}

}
void Renderer::drawEdgeToEdge(const Gradient& grad, Edge & left_tallEdge, Edge & right_shortEdge) const
{
	uint32_t yStart = right_shortEdge.getYStart();
	uint32_t yEnd = right_shortEdge.getYEnd();
	uint32_t* currentPixRow = reinterpret_cast<uint32_t*>(surface->pixels) + right_shortEdge.getYStart() * scrWidth;
	texData TD = myTex.getTexData();
	for (int32_t y = yStart; y < yEnd; y++)
	{
		drawLineEdgeToEdge(grad,left_tallEdge, right_shortEdge, y,TD,currentPixRow);
		left_tallEdge.Step();
		right_shortEdge.Step();
		currentPixRow = currentPixRow + scrWidth;
	}

}
void Renderer::drawEdgeToEdgeInversed(const Gradient& grad, Edge & left_tallEdge, Edge & right_shortEdge) const
{
	uint32_t yStart = right_shortEdge.getYStart();
	uint32_t yEnd = right_shortEdge.getYEnd();
	uint32_t* currentPixRow = reinterpret_cast<uint32_t*>(surface->pixels) + right_shortEdge.getYStart() * scrWidth;
	texData TD = myTex.getTexData();

	for (int32_t y = yStart; y < yEnd; y++)
	{
		drawLineEdgeToEdge(grad,right_shortEdge,left_tallEdge, y,TD,currentPixRow);
		left_tallEdge.Step();
		right_shortEdge.Step();
		currentPixRow = currentPixRow + scrWidth;
	}

}
void Renderer::drawLineEdgeToEdge(const Gradient& grad, Edge & left, Edge & right,int32_t& y,texData& td, uint32_t* currentPixRow) const
{
	uint32_t xMin = left.getCurrentX();
	uint32_t xMax = right.getCurrentX();

	uint32_t texCoordsUXStep = grad.getTexCoordsUXStep();
	uint32_t texCoordsVXStep = grad.getTexCoordsVXStep();
	uint32_t oneOverWXStep = grad.getOneOverWXStep();
	uint32_t texCoordsU = left.getTexCoordsU();
	uint32_t texCoordsV = left.getTexCoordsV();
	uint32_t oneOverW = left.getOneOverW();

	for (int32_t x = xMin; x < xMax; x += 1 << PSCAL) {

		uint32_t texX = (((DIVFP(texCoordsU,oneOverW)) & UV_Wrap) << td.widthShift) >> PSCAL;
		uint32_t texY = (((DIVFP(texCoordsV,oneOverW)) & UV_Wrap) << td.heightShift) >> PSCAL;

		currentPixRow[x >> PSCAL] = td.texColors[texY * td.width + texX];
		//currentPixRow[x >> PSCAL] = 0xffffffff;
		texCoordsU = texCoordsU + texCoordsUXStep;
		texCoordsV = texCoordsV + texCoordsVXStep;
		oneOverW = oneOverW + oneOverWXStep;

	}
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
