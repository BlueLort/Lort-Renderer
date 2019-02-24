#include "FilesManager.h"

FilesManager* FilesManager::filesManagerInstance =new FilesManager();



FilesManager::FilesManager()
{
}


FilesManager::~FilesManager()
{
	delete filesManagerInstance;
}

Color* FilesManager::readImage(const std::string & filePath, int & width, int & height, int & nrChannels) const
{
	
	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
	Color* imageArray = new Color[width*height];
	uint32_t k=0;
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
				imageArray[k++]= Color(r, g, b, a);

			}
		}
	}
	else {
		std::cout << "COULDN'T READ Image" << std::endl;
	}
	return imageArray;
}

void FilesManager::readOBJModel(const std::string & filePath, std::vector<Vertex>& vertices,const bool Tex_Norm_Included[2]) const
{
	std::ifstream OBJFile(filePath);
	std::string fileData= "",line;
	if (OBJFile.is_open())
	{
		while (getline(OBJFile, line))
		{
			fileData+= line;
		}
		OBJFile.close();
	}
	else {
		std::cout << " COULDN'T READ THE OBJ FILE !! \nFile Path:" + filePath << std::endl;
		return;
	}
	std::vector<VEC4> Positions;
	std::vector<VEC2> TexCoords;
	std::vector<VEC3> Normals;


	//Get Vertices Positions
	std::regex reg = std::regex("v +([-\.1234567890]+) +([-\.1234567890]+) +([-\.1234567890]+)");
	readPositionsOBJ(fileData,Positions,reg);

	//RE-ADJUST VECTORS MEMORY RESERVATION ACCORDING TO VERTICES COUNT
	if (Tex_Norm_Included[0])TexCoords.reserve(Positions.size());
	if (Tex_Norm_Included[1])Normals.reserve(Positions.size());

	VEC2 TexCoords_default = VEC2(0.0f, 0.0f);//if tex coords not included
	VEC3 Normal_default = VEC3(0.0f, 1.0f, 0.0f);//if normal not included
	TexCoords.push_back(TexCoords_default);
	Normals.push_back(Normal_default);

	//Get Vertices TexCoords
	if (Tex_Norm_Included[0]) {
		reg = std::regex("vt +([-\.1234567890]+) +([-\.1234567890]+)");
		readTexCoordsOBJ(fileData, TexCoords, reg);
	}

	//Get Vertices Normals
	if (Tex_Norm_Included[1]) {
		reg = std::regex("vn +([-\.1234567890]+) +([-\.1234567890]+) +([-\.1234567890]+)");
		readNormalsOBJ(fileData, Normals, reg);
	}

	//Construct the vertices data (ignoring the indices mapping from this project)
	std::string d = "[0-9]";//match digit as '\d' does not work on some compilers
	reg = std::regex("f +("+d+"+)\/*("+d+"*)\/*("+d+"*)\/* +("+d+"+)\/*("+d+"*)\/*("+d+"*)\/* +("+d+"+)\/*("+d+"*)\/*("+d+"*)\/*");
	std::smatch match;
	while (std::regex_search(fileData, match, reg)) {
		uint32_t vp1 = static_cast<uint32_t>(stoul(match.str(1)));
		uint32_t vp2 = static_cast<uint32_t>(stoul(match.str(4)));
		uint32_t vp3 = static_cast<uint32_t>(stoul(match.str(7)));
			
		uint32_t vtx1 = (!Tex_Norm_Included[0] ? 0 :  static_cast<uint32_t>(stoul(match.str(2))));
		uint32_t vtx2 = (!Tex_Norm_Included[0] ? 0 :  static_cast<uint32_t>(stoul(match.str(5))));
		uint32_t vtx3 = (!Tex_Norm_Included[0] ? 0 :  static_cast<uint32_t>(stoul(match.str(8))));
				 
		uint32_t vn1 = (!Tex_Norm_Included[1] ? 0 : static_cast<uint32_t>(stoul(match.str(3))));
		uint32_t vn2 = (!Tex_Norm_Included[1] ? 0 : static_cast<uint32_t>(stoul(match.str(6))));
		uint32_t vn3 = (!Tex_Norm_Included[1] ? 0 : static_cast<uint32_t>(stoul(match.str(9))));
		//positions vector is always filled however texcoords and normals are not.
		//So i have added a default data at index 0 in both texcoords and normals vectors.
		//so because of that
		//now indices of vertex positions should be -1 (counting from 1) and texcoords and normals are not.
		vertices.push_back(Vertex(Positions[vp1-1], TexCoords[vtx1], Normals[vn1]));
		vertices.push_back(Vertex(Positions[vp2-1], TexCoords[vtx2], Normals[vn2]));
		vertices.push_back(Vertex(Positions[vp3-1], TexCoords[vtx3], Normals[vn3]));
		fileData = match.suffix().str();
	}

}

void FilesManager::readPositionsOBJ(std::string fileData, std::vector<VEC4>& VerticesPositions, const std::regex & reg) const
{
	std::smatch match;
	while (std::regex_search(fileData, match, reg)) {
		VEC4 pos;
		pos.arr[0] = stof(match.str(1));
		pos.arr[1] = stof(match.str(2));
		pos.arr[2] = stof(match.str(3));
		pos.arr[3] = 1.0f;
		VerticesPositions.push_back(pos);
		fileData = match.suffix().str();
	}
}

void FilesManager::readTexCoordsOBJ(std::string fileData,std::vector<VEC2>& VerticesTexCoords, const std::regex & reg) const
{
	std::smatch match;
	while (std::regex_search(fileData, match, reg)) {
		VEC2 texCoords;
		texCoords.arr[0] = stof(match.str(1));
		texCoords.arr[1] = stof(match.str(2));
		VerticesTexCoords.push_back(texCoords);
		fileData = match.suffix().str();
	}
}

void FilesManager::readNormalsOBJ(std::string fileData,std::vector<VEC3>& VerticesNormals, const std::regex & reg) const
{
	std::smatch match;
	while (std::regex_search(fileData, match, reg)) {
		VEC3 norm;
		norm.arr[0] = stof(match.str(1));
		norm.arr[1] = stof(match.str(2));
		norm.arr[2] = stof(match.str(3));
		VerticesNormals.push_back(norm);
		fileData = match.suffix().str();
	}
}
