#pragma once
#include "Texture.h"
#include "Camera.h"
#include "Renderer.h"



class Plane
{
public:
	inline Plane(const std::string& hMap, float MaxH = 4.0f, float Width = 32.0f, float Depth = 32.0f, uint32_t N = 32, uint32_t M = 32) :hMapTex(hMap)
	{
		
		maxHeight = MaxH;
		n = N;
		m = M;
		depth = Depth;
		width = Width;
		init();
	}
	inline ~Plane() { vertices.clear(); indices.clear(); };
	Plane(const Plane& copy) = delete;
	inline float getMaxHeight() const { return maxHeight; }
	inline float getWidth() const { return width; }
	inline float getDepth() const { return depth; }
	inline Mat4x4f getVP() const { return VP; }
	inline void setVP(const Mat4x4f& VP) { this->VP = VP; }
	inline std::vector<Vertex> getVertices() { return vertices; }
	inline std::vector<unsigned int> getIndices() { return indices; }

	inline void setMaxHeight(float newHeight) { maxHeight = newHeight; }
	inline void setWidth(float newWidth) { width = newWidth; }
	inline void setDepth(float newDepth) { depth = newDepth; }

	void render(const Renderer* renderer)const;
private:
	Mat4x4f VP;
	Mat4x4f Model;//identity
	float maxHeight;
	float width;
	float depth;
	float hmap_DX;
	float hmap_DZ;
	uint32_t n;
	uint32_t m;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
    void init();
	Texture hMapTex;
	Texture  diffTex{ std::string("res/desert_mntn_d.jpg") };
	void genPlaneHeightMap();
	float getHeightMapValue(int32_t x, int32_t z) const;
	Vec4f getNormal(float x, float z, float offsetX, float offsetZ) const;


};

