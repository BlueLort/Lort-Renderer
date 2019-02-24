#pragma once
#include "FilesManager.h"
#include "Renderer.h"
class Mesh
{
public:
	Mesh(const std::string& filePath,const bool& normalIncluded,const bool& texIncluded);
	void render(const Renderer& renderer) const;
	inline MAT4 getMVP() const { return MVP; }
	void setMVP(const MAT4& MVP) { this->MVP = MVP; }
	~Mesh();
private:
	MAT4 MVP;
	std::vector<Vertex> vertices;
};

