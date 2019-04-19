#include "FilesManager.h"

FilesManager* FilesManager::filesManagerInstance =new FilesManager();



FilesManager::FilesManager()
{
}


FilesManager::~FilesManager()
{
	delete filesManagerInstance;
}

uint32_t* FilesManager::readImage(const std::string & filePath, int & width, int & height, int & nrChannels) const
{
	
	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
	uint32_t* imageArray = new uint32_t[width*height];
	uint32_t k=0;
	if (data)
	{
		unsigned bytePerPixel = nrChannels;
		uint8_t* pixelOffset;
		uint8_t r, g, b, a;
		for (int32_t y = 0; y < height; y++) {
			for (int32_t x = 0; x <width; x++) {
				pixelOffset = data + (width*y + x) * bytePerPixel;
				r = pixelOffset[0];
				g = pixelOffset[1];
				b = pixelOffset[2];
				a = nrChannels >= 4 ? pixelOffset[3] : 0xff;
				//SDL FORMAT IS ARGB
				imageArray[k++]= (a <<24 ) | ( r << 16) | ( g << 8) | (b);

			}
		}
	}
	else {
		std::cout << "COULDN'T READ IMAGE \n File Path:"+filePath << std::endl;
	}
	return imageArray;
}

void FilesManager::readOBJModel(const std::string & filePath, std::vector<Vertex>& vertices,std::vector<int32_t>& indices) const
{

	std::vector<Vec4f> Positions;
	std::vector<Vec4f> TexCoords;
	std::vector<Vec4f> Normals;
	std::vector<int32_t> Indices;
	std::unordered_map<int32_t, int32_t> IndexerMap;
	std::ifstream OBJFile(filePath);
	std::string line;
	Vec4f TexCoords_default;//if tex coords not included
	Vec4f Normal_default = Vec4f(0.0f, 1.0f, 0.0f,0.0f);//if normal not included
	TexCoords.push_back(TexCoords_default);
	Normals.push_back(Normal_default);

	if (OBJFile.is_open())
	{
		while (getline(OBJFile, line))
		{

			std::vector<std::string> tokens = split(line,' ');
			if (tokens.size() <= 1 || tokens[0][0] == '#')continue;
			if (tokens[0] == "v") {
				Positions.push_back(Vec4f(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]), 1.0f));
			}
			else if (tokens[0] == "vt") {
				TexCoords.push_back(Vec4f(stof(tokens[1]), stof(tokens[2]),0.0f,0.0f));
			}
			else if (tokens[0] == "vn") {
				Normals.push_back(Vec4f(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]),0.0f).normalized());
			}
			else if (tokens[0] == "f") {
				static int32_t indexCounter = 0;
				int32_t nVerts = tokens.size()-3;//to determine it's a 4verts face or 3 verts face
				for (int32_t i = 0; i < nVerts; i++) {
					OBJModelIndexer obi = getOBJModelIndexerFromIndicesString(tokens[1]);
						int32_t hash = getHash(obi);
						if (IndexerMap.find(hash) == IndexerMap.end()) {
							vertices.emplace_back(Vertex(Positions[obi.posIndex], TexCoords[obi.texIndex], Normals[obi.normIndex]));
							IndexerMap.emplace(std::pair<int32_t, int32_t>(hash, indexCounter++));
						}
						Indices.push_back(hash);
					for (int32_t j = 2; j <= 3; j++) {
						obi = getOBJModelIndexerFromIndicesString(tokens[j+i]);
						hash = getHash(obi);
						if (IndexerMap.find(hash) == IndexerMap.end()) {
							vertices.emplace_back(Vertex(Positions[obi.posIndex], TexCoords[obi.texIndex], Normals[obi.normIndex]));
							IndexerMap.emplace(std::pair<int32_t, int32_t>(hash, indexCounter++));
						}
						Indices.push_back(hash);
					}
				}
			}
		}
		OBJFile.close();
	}
	else {
		std::cout << " COULDN'T READ THE OBJ FILE !! \nFile Path:" + filePath << std::endl;
		return;
	}
	int32_t len = Indices.size();
	for (int32_t i = 0; i < len; i++) {
		indices.push_back(IndexerMap.find(Indices[i])->second);
	}
}

OBJModelIndexer FilesManager::getOBJModelIndexerFromIndicesString(const std::string & s)const
{
	std::vector<std::string> faceComponents = split(s, '/');
	OBJModelIndexer obi;
	if (faceComponents.size() > 1) {
		obi.posIndex = stoul(faceComponents[0]) - 1;
		obi.texIndex = 0;//default tex index
		obi.normIndex = 0;//default norm index
		if (!faceComponents[1].empty()) {
			obi.texIndex = stoul(faceComponents[1]);
		}
		if (faceComponents.size() > 2) {
			obi.normIndex = stoul(faceComponents[2]);
		}
	}
	return obi;
}

int32_t FilesManager::getHash(const OBJModelIndexer & obi) const
{
	//2 prime numbers for good hashing
	constexpr const int32_t MULTIPLIER =337;
	constexpr const int32_t BASE = 241;
	int32_t hash = BASE;
	hash = ((MULTIPLIER * hash + obi.posIndex)*MULTIPLIER + obi.texIndex)*MULTIPLIER + obi.normIndex;
	return hash;
}

inline std::vector<std::string> FilesManager::split(const std::string & s, char delimiter) const
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

