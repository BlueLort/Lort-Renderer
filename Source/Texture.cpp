#include "Texture.h"




Texture::Texture(const std::string & filePath)
{
	this->texColors = FilesManager::getFilesManagerInstance()->readImage(filePath,width,height,nChannels);
	this->widthShift = log2(width);
	this->heightShift = log2(height);
}

Texture::~Texture()
{
	delete [] texColors;
}
