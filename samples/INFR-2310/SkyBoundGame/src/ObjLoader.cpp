#include "ObjLoader.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

// Borrowed from https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
#pragma region String Trimming

// trim from start (in place)
static inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

#pragma endregion 

bool ObjLoader::LoadFromFile(const std::string& filename, std::vector<glm::vec3>& outVec, std::vector<glm::vec2>& outUV, std::vector<glm::vec3>& outNorm)
{
	
	glm::vec3 scale;
	glm::vec4 color = glm::vec4(1.0f);

	glm::vec3 pos;
	glm::vec3 vNorm;
	glm::vec2 vText;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	std::vector<GLint> vPos_indicies;
	std::vector<GLint> vText_indicies;
	std::vector<GLint> vNorm_indicies;


	GLint temp_glint = 0;



	// Open our file in binary mode
	std::ifstream file;
	file.open(filename, std::ios::binary);

	// If our file fails to open, we will throw an error
	if (!file) {
		throw std::runtime_error("Failed to open file");
	}

	
	MeshBuilder<VertexPosNormTexCol> mesh;
	std::string line;

	//New vertex array
	std::vector<VertexPosNormTex> vertices;

	// TODO: Load from file
	while (std::getline(file, line)) {
		trim(line);

		if (line.substr(0, 1) == "#")
		{
			//Comment, no-op
		}
		// If its not a # symbol do this:
		// We can do equality check this way since the left side is a string and not a char*
		else 
		{
			
			

			//Pos
			//ss >> pos.x >> pos.y >> pos.z;

			//Scale
			//ss >> scale.x >> scale.y >> scale.z;

			//Angle
			//ss >> eulerDeg.x >> eulerDeg.y >> eulerDeg.z;

			//Colour
			/*if (ss.rdbuf()->in_avail() > 0) {
				ss >> color.r >> color.g >> color.b;
			}
			if (ss.rdbuf()->in_avail() > 0) {
				ss >> color.a;
			}*/

			if (line.substr(0, 2) == "v ")
			{
				std::istringstream ss = std::istringstream(line.substr(2));
				ss >> pos.x >> pos.y >> pos.z;
				positions.push_back(pos);
			}

			else if (line.substr(0, 2) == "vt")
			{
				std::istringstream ss = std::istringstream(line.substr(3));
				ss >> vText.x >> vText.y;
				uvs.push_back(vText);
				
			}

			else if (line.substr(0, 2) == "vn")
			{
				std::istringstream ss = std::istringstream(line.substr(3));
				ss >> vNorm.x >> vNorm.y, vNorm.z;
				normals.push_back(vNorm);
			}
			//Reading faces
			else if (line.substr(0, 2) == "f ")
			{
				std::istringstream ss = std::istringstream(line.substr(2));
				int counter = 0;
				int v, uv, n;
				char a;

				for (int x = 0; x <= 2; x++)
				{
					ss >> v >> a >> uv >> a >> n;
					vPos_indicies.push_back(v);
					vText_indicies.push_back(uv);
					vNorm_indicies.push_back(n);
				}
				//std::string vtx1, vtx2, ve;
			}
			else
			{

			}

			
		}

		

	}
	// Loads in the indices
	for (size_t i = 0; i < vPos_indicies.size(); i++)
	{
		unsigned int vertexNum = vPos_indicies[i];
		unsigned int uvNum = vText_indicies[i];
		unsigned int normalNum = vNorm_indicies[i];
		glm::vec3 Vertex = positions[vertexNum - 1];
		glm::vec2 Uv = uvs[uvNum - 1];
		glm::vec3 Normal = normals[normalNum - 1];
		outVec.push_back(Vertex);
		outUV.push_back(Uv);
		outNorm.push_back(Normal);
	}
	std::cout << "load successful";
	

	return true;
}