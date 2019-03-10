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
	static Renderer* getInstance() {return &rendererInstance;}
	inline void setWindow(SDL_Window* window) { this->window = window; }
	inline void setSurface(SDL_Surface* surface) { this->surface = surface; }
	inline void setScreenWidth(const uint32_t& scrWidth) { this->scrWidth = scrWidth;}
	inline void setScreenHeight(const uint32_t& scrWidth) { this->scrHeight = scrHeight;}
	inline void setClearColor(const uint32_t& c) { this->clearColor = c; }
	void updateHalfScrSize() {
		this->halfScrWidth = (uint32_t)(scrWidth / 2);
		this->halfScrHeight = (uint32_t)(scrHeight / 2);
	}
	void clearScreen() const;
	void drawTriangle(const Vertex& v1,const Vertex& v2,const Vertex& v3) const;
	void updateScreen() const;
private:
	SDL_Window* window = nullptr;
	SDL_Surface* surface = nullptr;
	uint32_t clearColor;//clear color in ARGB FORMAT
	uint32_t scrWidth;
	uint32_t scrHeight;
	uint32_t halfScrWidth;
	uint32_t halfScrHeight;
	static Renderer rendererInstance;
    Texture myTex=Texture(std::string("brickwall.jpg"));
	//utility
	int32_t UV_Wrap = (1 << FP_SCL) - 1;


	void scanTriangle(const Vertex & minYVert, const Vertex & midYVert, const Vertex & maxYVert,const bool& direction) const;
	void drawEdgeToEdge(const Gradient& grad, Edge & left_shortEdge, Edge & right_tallEdge) const;
	void drawEdgeToEdgeInversed(const Gradient& grad, Edge & left_tallEdge, Edge & right_shortEdge) const;
	void drawLineEdgeToEdge(const Gradient& grad, Edge & left_tallEdge, Edge & right_shortEdge,const int32_t& y,const texData& td,uint32_t* currentPixRw) const;
	bool checkDirection(const Vertex& v1, const Vertex& v2, const Vertex& v3) const;

};

