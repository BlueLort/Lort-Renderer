#pragma once
#include <iostream>
#include <vector>
#include "Structures.h"
#include "stb_image.h"
#include <fstream>
#include <sstream>
#include <unordered_map>

struct OBJModelIndexer {
	int32_t posIndex;
	int32_t texIndex;
	int32_t normIndex;
};
class FilesManager
{
public:
	inline static FilesManager* getFilesManagerInstance() { return filesManagerInstance; }
	~FilesManager();
	uint32_t* readImage(const std::string& filePath,int &width, int &height, int &nrChannels) const;
	void readOBJModel(const std::string& filePath,std::vector<Vertex>& vertices, std::vector<int32_t>& indices) const;
private:
	FilesManager();
	static FilesManager* filesManagerInstance;
	OBJModelIndexer getOBJModelIndexerFromIndicesString(const std::string& s)const;
	int32_t getHash(const OBJModelIndexer& obi)const;
	std::vector<std::string> split(const std::string& s, char delimiter) const;
};

