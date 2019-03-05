#include "Mesh.h"



Mesh::Mesh(const std::string& filePath,const bool& texIncluded , const bool& normalIncluded)
{
	bool texNorm[2];
	texNorm[0] = texIncluded;
	texNorm[1] = normalIncluded;
	FilesManager::getFilesManagerInstance()->readOBJModel(filePath, vertices, texNorm);
}

void Mesh::render(const Renderer& renderer) const
{
	for (int32_t i = 0; i < vertices.size(); i= i + 3) {
		renderer.drawTriangle(
			  vertices[i].Transform(MVP)
			, vertices[i + 1].Transform(MVP)
			, vertices[i + 2].Transform(MVP)
		);
	}
}


Mesh::~Mesh()
{
	vertices.clear();
}
