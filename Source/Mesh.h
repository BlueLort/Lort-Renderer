#pragma once
#include "FilesManager.h"
#include "Renderer.h"
class Mesh
{
public:
	Mesh(const std::string& filePath);
	Mesh(const Mesh& copy) = delete;
	void render(const Renderer* renderer) const;
	inline Mat4x4f getMVP() const { return VP; }
	inline Mat4x4f getModel() const { return Model; }
	inline void setVP(const Mat4x4f& VP) { this->VP = VP; }
	inline void setModel(const Mat4x4f& M) { this->Model = M; }
	~Mesh();
private:
	Mat4x4f VP;
	Mat4x4f Model;
	std::vector<Vertex> vertices;
	std::vector<int32_t> indices;
};

