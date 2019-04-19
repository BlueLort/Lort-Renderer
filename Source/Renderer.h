#pragma once
#include <SDL/SDL.h>
#include "Math.h"
#include "Structures.h"
#include "Edge.h"
#include "Texture.h"
#include <iostream>
class Renderer
{
public:
	Renderer();
	Renderer(SDL_Window* window, SDL_Surface* surface, const uint32_t& scrWidth, const uint32_t& scrHeight);
	~Renderer();
	static Renderer* getInstance() { return rendererInstance; }
	inline void setWindow(SDL_Window* window) { this->window = window; }
	inline void setSurface(SDL_Surface* surface) { this->surface = surface; }
	inline void setScreenWidth(const uint32_t& scrWidth) { this->scrWidth = scrWidth; }
	inline void setScreenHeight(const uint32_t& scrWidth) { this->scrHeight = scrHeight; }
	inline void setClearColor(const uint32_t& c) { this->clearColor = c; }
	inline void initZBuffer() { zBuffer = new int32_t[scrHeight*scrWidth]; }
	void updateHalfScrSize() {
		this->halfScrWidth = (scrWidth / 2);
		this->halfScrHeight = (scrHeight / 2);
	}
	void clearScreen() const;
	void clipRenderTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3,const Texture& tex = rendererInstance->myTex)const;
	void updateScreen() const;
private:
	SDL_Window * window = nullptr;
	SDL_Surface* surface = nullptr;
	uint32_t clearColor;//clear color in ARGB FORMAT
	uint32_t scrWidth;
	uint32_t scrHeight;
	uint32_t halfScrWidth;
	uint32_t halfScrHeight;
	static Renderer* rendererInstance;
	int32_t* zBuffer;

	Texture myTex = Texture(std::string("res/brickwall.jpg"));
	bool isInsideViewFrustum(const Vertex& v) const;
	bool ClipPolygonAxis(std::vector<Vertex>& vertices, std::vector<Vertex>&  auxillaryVertices, int32_t componentIndex ) const;
	void ClipPolygonComponent(std::vector<Vertex>&  auxillaryVertices, int32_t componentIndex,
		float componentFactor, std::vector<Vertex>& vertices) const;
	void drawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Texture& tex) const;
	bool checkDirection(const Vertex& v1, const Vertex& v2, const Vertex& v3) const;
	void scanTriangle(const Vertex & v1, const Vertex & v2, const Vertex & v3, bool direction, const Texture& tex) const;
	void scanDrawEdges( Edge& left, Edge& right, const Interpolant& inter, const Texture& tex)const;
	void scanDrawEdgesInversed(Edge& left, Edge& right, const Interpolant& inter, const Texture& tex)const;
	void drawScanLine(const Edge& left, const Edge& right, const uint32_t & y, uint32_t* currentPixRw, const Interpolant& inter, const Texture& tex) const;

	

};

