#pragma once
#include <iostream>
#include <vector>
#include "Structures.h"
#include "stb_image.h"
#include <fstream>
#include <regex>
class FilesManager
{
public:
	inline static FilesManager* getFilesManagerInstance() { return filesManagerInstance; }
	~FilesManager();
	Color* readImage(const std::string& filePath,int &width, int &height, int &nrChannels) const;
	
	
	void readOBJModel(const std::string& filePath,std::vector<Vertex>& vertices, const bool Tex_Norm_Included[2]) const;
private:
	FilesManager();
	static FilesManager* filesManagerInstance;
	void readPositionsOBJ(std::string fileData,  std::vector<VEC4>& VerticesPositions,const std::regex& reg) const;
	void readTexCoordsOBJ(std::string fileData,  std::vector<VEC2>& VerticesTexCoords, const std::regex& reg) const;
	void readNormalsOBJ(std::string fileData,  std::vector<VEC3>& VerticesNormals, const std::regex& reg) const;
};

