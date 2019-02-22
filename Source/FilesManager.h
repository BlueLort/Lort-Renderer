#pragma once
#include <iostream>
#include <vector>
#include "Structures.h"
#include "stb_image.h"
class FilesManager
{
public:
	inline static FilesManager* getFilesManagerInstance() { return filesManagerInstance; }
	~FilesManager();
	static std::vector< std::pair<uint32_t, Color> >* readImage(const std::string& filePath,int &width, int &height, int &nrChannels);
private:
	FilesManager();
	static FilesManager* filesManagerInstance;
};

