#pragma once
#include "FilesManager.h"
class Texture
{
public:
	Texture(const std::string& filePath);
	inline Color getTexColorAt(const uint16_t& x, const uint16_t& y) const {
		return texColors[width*y + x];
	}
	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }

	~Texture();
private:
	int width;
	int height;
	int nChannels;
	 Color* texColors;

};

