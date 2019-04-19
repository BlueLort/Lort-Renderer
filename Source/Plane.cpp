#include "Plane.h"


void Plane::init()
{
	uint32_t vCount = n * m;
	uint32_t fCount = 2 * (n - 1) * (m - 1);
	vertices.resize(vCount);
	indices.resize(3 * fCount);
	genPlaneHeightMap();
	
	uint32_t k = 0;
	for (uint32_t i = 0; i < m - 1; ++i)
	{
		for (uint32_t j = 0; j < n - 1; ++j)
		{
			indices[k] = i * n + j;
			indices[k + 1] = i * n + j + 1;
			indices[k + 2] = (i + 1)*n + j;
			indices[k + 3] = (i + 1)*n + j;
			indices[k + 4] = i * n + j + 1;
			indices[k + 5] = (i + 1)*n + j + 1;
			//calculate tangents
			calculateTangents(vertices[indices[k]], vertices[indices[k + 1]], vertices[indices[k + 2]]);
			calculateTangents(vertices[indices[k + 3]], vertices[indices[k + 4]], vertices[indices[k + 5]]);

			k += 6; // next quad

		}

	}
}



void Plane::render(const Renderer* renderer)const
{
	int32_t len = indices.size();
	for (int32_t i = 0; i < len; i = i + 3) {
		renderer->clipRenderTriangle(
			vertices[indices[i]].Transform(VP,Model)
			, vertices[indices[i + 1]].Transform(VP,Model)
			, vertices[indices[i + 2]].Transform(VP,Model)
			,diffTex);
	}
}



inline float Plane::getHeightMapValue(int32_t x, int32_t z) const
{
	//get R from ARGB format (R is enough because it's a grayscale image)
	return (((hMapTex.getColors()[(z << hMapTex.getWidthShift()) + x]<<24)>>24) / 255.0f )* maxHeight -maxHeight/2.0f;
}


void Plane::genPlaneHeightMap()
{
	hmap_DX = hMapTex.getWidth() / width;
	hmap_DZ = hMapTex.getHeight() / depth;


	float dx = width / (n - 1);
	float dz = depth / (m - 1);
	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;
	///full image over all terrain
	///FACTOR * _n || FACTOR * _n to divide texture between tiles
	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);
	float x, y, z;

	for (unsigned int i = 0; i < m; ++i)
	{
		z = halfDepth - i * dz;
		int32_t mappedZ = map(static_cast<int32_t>(((z)* hmap_DZ)),
			static_cast<int32_t>((halfDepth* hmap_DZ)),
			static_cast<int32_t>((-halfDepth * hmap_DZ)),
			0,
			static_cast<int32_t>(hMapTex.getHeight()-1));
		for (unsigned int j = 0; j < n; ++j)
		{
			x = -halfWidth + j * dx;
			//better approach is to Lerp(linear interpolate) between 2 height map numbers if the _n&_m >height map width & depth
			//but im keeping it simple here
			int32_t mappedX = map(static_cast<int32_t>(((x)* hmap_DX)),
				static_cast<int32_t>((-halfWidth * hmap_DX)),
				static_cast<int32_t>((halfWidth* hmap_DX)),
				0,
				static_cast<int32_t>(hMapTex.getWidth()-1));

			y = getHeightMapValue(mappedX, mappedZ);

			vertices[i*n + j].pos = Vec4f(x, y, z,1.0f);

			//used for lighting.
			Vec4f Normal;
			Normal = getNormal(static_cast<float>(mappedX), static_cast<float>(mappedZ), dx, dz);

			vertices[i*n + j].normal = Normal;


			// used for texturing.
			vertices[i*n + j].texCoords = Vec4f(j * du, i * dv,0.0f,0.0f);
		}
	}


}


inline Vec4f Plane::getNormal(float x, float z, float offsetX, float offsetZ) const
{
	if(z-offsetZ<0||x-offsetX<1|| z+offsetZ>hMapTex.getHeight() || x+offsetX>hMapTex.getHeight())return Vec4f(0.0f, 1.0f, 0.0f, 0.0f);
	float heightL, heightR, heightD, heightU;
	heightL = getHeightMapValue(static_cast<int32_t>(((x - offsetX)* hmap_DX)),static_cast<int32_t>((z* hmap_DZ)));
	heightR = getHeightMapValue(static_cast<int32_t>(((x + offsetX)* hmap_DX)),static_cast<int32_t>((z* hmap_DZ)));
	heightD = getHeightMapValue(static_cast<int32_t>((x* hmap_DX)), static_cast<int32_t>((z - offsetZ)* hmap_DZ));
	heightU = getHeightMapValue(static_cast<int32_t>((x* hmap_DX)),static_cast<int32_t>((z + offsetZ)* hmap_DZ));
	Vec4f Normal = Vec4f(heightL - heightR,4.0f, heightD - heightU,0.0f);
	return Normal.normalized();
}