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
	inline void setClearColor(const Color& c) { this->clearColor = c; }
	void updateHalfScrSize() {
		this->halfScrWidth = (uint32_t)(scrWidth / 2);
		this->halfScrHeight = (uint32_t)(scrHeight / 2);
	}
	void clearScreen() const;
	void drawTriangle(const Vertex& v1,const Vertex& v2,const Vertex& v3) const;
	void updateScreen() const;
	void drawPixel(const uint32_t& x, const uint32_t& y, const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a)const;
private:
	SDL_Window*	  window = nullptr;
	SDL_Surface* surface = nullptr;
	Color		  clearColor;//color that clears the screen
	uint32_t	  scrWidth;
	uint32_t	  scrHeight;
	uint32_t	  halfScrWidth;
	uint32_t	  halfScrHeight;
	static Renderer rendererInstance;

	Texture myTex=Texture(std::string("snow_grass2_d.jpg"));

	void scanTriangle(const Vertex & v1, const Vertex & v2, const Vertex & v3, bool direction) const;
	void scanEdges(const Gradient& grad,Edge& e1,Edge& e2, bool direction)const;
	void drawScanLine(const Gradient& grad,const Edge& left, const Edge& right, const uint32_t & y)const;

};

