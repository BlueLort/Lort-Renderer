#include "Mesh.h"

Mesh::Mesh(const std::string& filePath)
{
	FilesManager::getFilesManagerInstance()->readOBJModel(filePath, vertices, indices);
}

void Mesh::render(const Renderer* renderer) const
{
	int32_t len = indices.size();
	Mat4x4f MVP = VP * Model;
	for (int32_t i = 0; i < len; i= i + 3) {
		renderer->clipRenderTriangle(
			  vertices[indices[i]].Transform(MVP,Model)
			, vertices[indices[i + 1]].Transform(MVP,Model)
			, vertices[indices[i + 2]].Transform(MVP,Model)
		);
	}
}


Mesh::~Mesh()
{
	vertices.clear();
	indices.clear();
}
