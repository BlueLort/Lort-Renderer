#pragma once
#include "FilesManager.h"
class Texture
{
public:
	inline Texture(const std::string& filePath) {
		this->texColors = FilesManager::getFilesManagerInstance()->readImage(filePath, width, height, nChannels);
		this->texColorsByte = reinterpret_cast<uint8_t*>(texColors);
		this->widthShift = static_cast<int32_t>(log2(width));
		this->heightShift = static_cast<int32_t>(log2(height));
		this->widthShiftDiff = FP_SCL - widthShift;
		this->heightShiftDiff = FP_SCL - heightShift;
	}
	inline uint32_t getTexColorAt(const uint32_t& x, const uint32_t& y) const {
		return texColors[(y<<widthShift)+ x];
	}
	inline uint32_t* getColors() const { return texColors; }
	inline uint8_t* getColorsByte() const { return texColorsByte; }
	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
	inline int32_t getWidthShift() const { return widthShift; }
	inline int32_t getHeightShift() const { return heightShift; }
	inline int32_t getWidthShiftDiff() const { return widthShiftDiff; }
	inline int32_t getHeightShiftDiff() const { return heightShiftDiff; }
	inline ~Texture() {  delete[] texColors; }
private:
	int32_t width;
	int32_t height;
	int32_t nChannels;
	int32_t widthShift;
	int32_t heightShift;
	int32_t widthShiftDiff;
	int32_t heightShiftDiff;
	uint32_t* texColors=nullptr;
	uint8_t* texColorsByte = nullptr;
};

