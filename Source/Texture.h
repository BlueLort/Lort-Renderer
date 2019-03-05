#pragma once
#include "FilesManager.h"
struct texData {
	int32_t width;
	int32_t height;
	int32_t widthShift;
	int32_t heightShift;
	uint32_t* texColors;
};
class Texture
{
public:
	Texture(const std::string& filePath);
	inline uint32_t getTexColorAt(const uint16_t& x, const uint16_t& y) const {
		return texColors[width*y + x];
	}
	inline uint32_t* getColors()const { return texColors; }
	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
	inline int32_t getWidthShift() const { return widthShift; }
	inline int32_t getHeightShift() const { return heightShift; }
	inline texData getTexData() const {
		texData td;
		td.width = this->width;
		td.height = this->height;
		td.widthShift = this->widthShift;
		td.heightShift = this->heightShift;
		td.texColors = this->texColors;
		return td;
	}
	~Texture();
private:
	int width;
	int height;
	int nChannels;
	int32_t widthShift;
	int32_t heightShift;
	uint32_t* texColors;

};

