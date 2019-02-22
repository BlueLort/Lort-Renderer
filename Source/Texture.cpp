#include "Texture.h"




Texture::Texture(const std::string & filePath)
{
	texColors = FilesManager::getFilesManagerInstance()->readImage(filePath,width,height,nChannels);
}

Texture::~Texture()
{
	texColors->clear();
	delete texColors;
}
