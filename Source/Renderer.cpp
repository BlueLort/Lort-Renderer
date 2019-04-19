#include "Renderer.h"
Renderer* Renderer::rendererInstance = new Renderer();

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
	if (zBuffer != nullptr)
	delete zBuffer;
}


void Renderer::clearScreen() const
{
	SDL_LockSurface(surface);


	SDL_FillRect(surface, NULL, clearColor);
	int32_t scrSize = scrHeight * scrWidth;
	for (int32_t i = 0; i < scrSize; i++) {
		//any number larger than 1.0f *FP_FVAL should work but i put INT_MAX to be a symbol for inf.
		zBuffer[i] = INT_MAX;
	}
}

void Renderer::clipRenderTriangle(const Vertex & v1, const Vertex & v2, const Vertex & v3, const Texture& tex) const
{
		bool v1Inside = isInsideViewFrustum(v1);
		bool v2Inside = isInsideViewFrustum(v2);
		bool v3Inside = isInsideViewFrustum(v3);

		if (v1Inside && v2Inside && v3Inside)
		{
			drawTriangle(v1, v2, v3,tex);
			return;
		}

		std::vector<Vertex> vertices;
		std::vector<Vertex> auxillaryVertices;
		vertices.reserve(6);//max is 6 vertices
		auxillaryVertices.reserve(6);


		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);

		if (ClipPolygonAxis(vertices, auxillaryVertices, 0) &&
			ClipPolygonAxis(vertices, auxillaryVertices, 1) &&
			ClipPolygonAxis(vertices, auxillaryVertices, 2))
		{
			Vertex initialVertex = vertices[0];
			int32_t len = vertices.size() - 1;
			for (int32_t i = 1; i <len; i++)
			{
				drawTriangle(initialVertex, vertices[i], vertices[i + 1],tex);
			}
		}
		
	
}

inline bool Renderer::isInsideViewFrustum(const Vertex & v) const
{
	Vec4f absV = abs(v.pos);
	return (absV.getX() <= absV.getW() && absV.getY() <= absV.getW() && absV.getZ() <= absV.getW());
}
inline bool Renderer::ClipPolygonAxis(std::vector<Vertex>& vertices, std::vector<Vertex>& auxillaryVertices, int32_t componentIndex) const
{
	ClipPolygonComponent(vertices, componentIndex, 1.0f, auxillaryVertices);
	vertices.clear();

	if (auxillaryVertices.empty())
	{
		return false;
	}

	ClipPolygonComponent(auxillaryVertices, componentIndex, -1.0f, vertices);
	auxillaryVertices.clear();

	return !vertices.empty();
}


inline void Renderer::ClipPolygonComponent(std::vector<Vertex>& auxillaryVertices, int32_t componentIndex, float componentFactor, std::vector<Vertex>& vertices) const
{
	Vertex previousVertex = auxillaryVertices[auxillaryVertices.size() - 1];
	float previousComponent = previousVertex.pos[componentIndex] * componentFactor;
	bool previousInside = previousComponent <= previousVertex.pos[3];

	int32_t len = auxillaryVertices.size();
	for (int32_t i = 0; i < len; i++) {
		Vertex currentVertex = auxillaryVertices[i];
		float currentComponent = currentVertex.pos[componentIndex] * componentFactor;
		bool currentInside = currentComponent <= currentVertex.pos[3];
		if (currentInside ^ previousInside)
		{
			float lerpAmt = (previousVertex.pos[3] - previousComponent) /
				((previousVertex.pos[3] - previousComponent) -
				(currentVertex.pos[3] - currentComponent));

			vertices.push_back(previousVertex.Lerp(currentVertex, lerpAmt));
		}
		if (currentInside)
		{
			vertices.push_back(currentVertex);
		}

		previousVertex = currentVertex;
		previousComponent = currentComponent;
		previousInside = currentInside;


	}


}


inline void Renderer::drawTriangle(const Vertex&  v1, const Vertex&  v2, const Vertex&  v3, const Texture& tex) const
{
	Mat4x4f toScrSpace = Mat4x4f::getScrSpaceTransform(
		static_cast<float>(halfScrWidth)
		,static_cast<float>(halfScrHeight));
	Mat4x4f mat;


	Vertex V1 = v1.Transform(toScrSpace,mat).PrespDivide();
	Vertex V2 = v2.Transform(toScrSpace,mat).PrespDivide();
	Vertex V3 = v3.Transform(toScrSpace,mat).PrespDivide();
	if (checkDirection(V1, V3, V2))return;//back-face culling
	
	if (V3.pos[1] < V2.pos[1]) {
		std::swap(V3, V2);
	}
	if (V2.pos[1] < V1.pos[1]) {
		std::swap(V2, V1);
	}
	if (V3.pos[1] < V2.pos[1]) {
		std::swap(V3, V2);
	}
	bool dir = checkDirection(V1, V3, V2);

	scanTriangle(V1, V2, V3, dir,tex);
}
inline bool Renderer::checkDirection(const Vertex & v1, const Vertex & v2, const Vertex & v3) const {
	Vec4f v2_v1 = v2.pos - v1.pos;
	Vec4f v3_v1 = v3.pos - v1.pos;
	return cross(v2_v1, v3_v1).getZ() >= 0;
}

inline void Renderer::scanTriangle(const Vertex & minYVert, const Vertex & midYVert, const Vertex & maxYVert, bool direction, const Texture& tex) const
{
	Interpolant inter(minYVert, midYVert, maxYVert);

	Edge topToBot = Edge(inter,minYVert, maxYVert,0);
	Edge topToMid= Edge(inter,minYVert, midYVert,0);
	Edge midToBot = Edge(inter,midYVert, maxYVert,1);
	if (!direction) {
		scanDrawEdges( topToBot, topToMid, inter, tex);
		scanDrawEdges(topToBot, midToBot, inter, tex);
	}
	else {
		scanDrawEdgesInversed( topToBot, topToMid, inter, tex);
		scanDrawEdgesInversed(topToBot, midToBot, inter, tex);
	}
	
}

inline void Renderer::scanDrawEdges( Edge & left, Edge & right, const Interpolant& inter, const Texture& tex) const
{
	int32_t yStart = right.getYStart();
	int32_t yEnd = right.getYEnd();
	uint32_t* currentPixRow = reinterpret_cast<uint32_t*>(surface->pixels) + right.getYStart() * scrWidth;
	for (int32_t y = yStart; y < yEnd; y++)
	{
		drawScanLine( left, right, y,currentPixRow,inter,tex);
		left.Step();
		right.Step();
		currentPixRow += scrWidth;
	}
}

inline void Renderer::scanDrawEdgesInversed(Edge& left, Edge& right, const Interpolant& inter, const Texture& tex) const
{
	int32_t yStart = right.getYStart();
	int32_t yEnd = right.getYEnd();
	uint32_t* currentPixRow = reinterpret_cast<uint32_t*>(surface->pixels) + right.getYStart() * scrWidth;
	for (int32_t y = yStart; y < yEnd; y++)
	{
		drawScanLine( right, left, y,currentPixRow,inter,tex);
		left.Step();
		right.Step();
		currentPixRow += scrWidth;
	}
}

inline void Renderer::drawScanLine(const Edge& left, const Edge& right, const uint32_t & y, uint32_t* currentPixRw, const Interpolant& inter, const Texture& tex) const
{
	int32_t xMin = static_cast<int32_t>(left.getCurrentX()+0.99f);
	int32_t xMax = static_cast<int32_t>(right.getCurrentX()+0.99f);
	float xPrestep = xMin - left.getCurrentX();
	
	Vec4f elementsStep = inter.getElementsXStep();
	float lightStep = inter.getLightXStep();
	///INIT GRADIANTS
	//u v depth oneOverW
	Vec4f elements= left.getElements() + elementsStep * xPrestep;
	float light = left.getLight() + lightStep * xPrestep;
	__m128i elementsFP = _mm_cvtps_epi32((elements*FP_FVAL).getVec());
	__m128i elementsStepFP = _mm_cvtps_epi32((elementsStep*FP_FVAL).getVec());
	elements *= FP_FVAL;
	elementsStep *= FP_FVAL;
	int32_t currentRow = y * (scrWidth);
	for (int32_t x = xMin; x < xMax; x++)
	{
	
		if (elementsFP.m128i_i32[2] < zBuffer[x + currentRow]){
			zBuffer[x + currentRow] = elementsFP.m128i_i32[2];
			///GET INFORMATION
			int16_t X = ((DIVFP(elementsFP.m128i_i32[0] , elementsFP.m128i_i32[3])&UV_WRAP)>>  tex.getWidthShiftDiff());
			int16_t Y = ((DIVFP(elementsFP.m128i_i32[1] , elementsFP.m128i_i32[3])&UV_WRAP)>> tex.getHeightShiftDiff());
			///APPLY TO PIXEL
			int32_t dest = ((Y << tex.getWidthShift()) + X)*4;
			uint8_t a = 255;
			uint8_t r = static_cast<uint8_t>(tex.getColorsByte()[dest+2] *light);
			uint8_t g = static_cast<uint8_t>(tex.getColorsByte()[dest+1] *light);
			uint8_t b = static_cast<uint8_t>(tex.getColorsByte()[dest] *light);
			currentPixRw[x] = (a << 24) | (r << 16) | (g << 8) | (b);
			//currentPixRw[x] = tex.getColors()[(Y<<tex.getWidthShift())+ X];
			//uint32_t col = (uint32_t)(light*255.0f);
			//currentPixRw[x] = 255<<24|col<<16|col<<8|col;

			//currentPixRw[x] = 0xffffffff;
		}
		///INCREASE FACTORS 
		elementsFP = _mm_add_epi32(elementsFP,elementsStepFP);
		light += lightStep;
	}
}


void Renderer::updateScreen() const
{
	SDL_UnlockSurface(surface);
	SDL_UpdateWindowSurface(window);
}
