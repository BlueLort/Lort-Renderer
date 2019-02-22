#include "FilesManager.h"

FilesManager* FilesManager::filesManagerInstance =new FilesManager();


FilesManager::FilesManager()
{
}


FilesManager::~FilesManager()
{
	delete filesManagerInstance;
}

std::vector< std::pair<uint32_t, Color> >* FilesManager::readImage(const std::string & filePath, int & width, int & height, int & nrChannels)
{
	std::vector< std::pair<uint32_t, Color> >* imageArray=new std::vector< std::pair<uint32_t, Color> >;
	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
	imageArray->reserve(width*height);
	if (data)
	{
		unsigned bytePerPixel = nrChannels;
		unsigned char* pixelOffset;
		unsigned char r, g, b, a;
		for (uint16_t y = 0; y < height; y++) {
			for (uint16_t x = 0; x <width; x++) {
				pixelOffset = data + (width*y + x) * bytePerPixel;
				r = pixelOffset[0];
				g = pixelOffset[1];
				b = pixelOffset[2];
				a = nrChannels >= 4 ? pixelOffset[3] : 0xff;
				imageArray->push_back(std::pair<uint32_t,Color>(width * y + x, Color(r, g, b, a)));

			}
		}
	}
	else {
		std::cout << "COULDN'T READ Image" << std::endl;
	}
	return imageArray;
}