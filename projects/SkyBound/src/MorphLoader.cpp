#include "MorphLoader.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Utilities/StringUtils.h"




MeshBuilder<VertexPosNormTexCol> MorphLoader::LoadFromFile(const std::string& filename, const glm::vec4& inColor)
{
	// Open our file in binary mode
	std::ifstream file;
	file.open(filename, std::ios::binary);

	// If our file fails to open, we will throw an error
	if (!file) {
		throw std::runtime_error("Failed to open file");
	}

	// Stores attributes
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;

	// We'll use bitmask keys and a map to avoid duplicate vertices
	std::unordered_map<uint64_t, uint32_t> indexMap;

	// We'll leverage the mesh builder class
	MeshBuilder<VertexPosNormTexCol> mesh;
	//std::vector<MeshBuilder<VertexPosNormTexCol>> mesh;


	// Temporaries for loading data
	glm::vec3 temp;
	glm::ivec3 vertexIndices;

	std::string line;
	// Iterate as long as there is content to read
	while (file.peek() != EOF) {
		std::string command;
		file >> command;

		// Load in vertex positions
		if (command == "v") {
			file >> temp.x >> temp.y >> temp.z;
			positions.push_back(temp);
		}
		// Load in vertex normals
		else if (command == "vn") {
			file >> temp.x >> temp.y >> temp.z;
			normals.push_back(temp);
		}
		// Load in UV coordinates
		else if (command == "vt") {
			file >> temp.x >> temp.y;
			textureCoords.push_back(temp);
		}
		// Load in face lines
		else if (command == "f") {
			// Read the entire line, trim it, and stuff it into a string stream
			std::string line;
			std::getline(file, line);
			trim(line);
			std::stringstream stream = std::stringstream(line);

			// We'll store the edges in case we added a quad
			uint32_t edges[4];
			int ix = 0;
			// Iterate over up to 4 sets of attributes
			for (; ix < 4; ix++) {
				if (stream.peek() != EOF) {
					// Load in the faces, split up by slashes
					char tempChar;
					vertexIndices = glm::ivec3(0);
					stream >> vertexIndices.x >> tempChar >> vertexIndices.y >> tempChar >> vertexIndices.z;

					// The OBJ format can have negative values, which are a reference from the last added attributes
					if (vertexIndices.x < 0) { vertexIndices.x = positions.size() - 1 + vertexIndices.x; }
					if (vertexIndices.y < 0) { vertexIndices.y = textureCoords.size() - 1 + vertexIndices.y; }
					if (vertexIndices.z < 0) { vertexIndices.z = normals.size() - 1 + vertexIndices.z; }

					// We can construct a key using a bitmask of the attribute indices
					// This let's us quickly look up a combination of attributes to see if it's already been added
					// Note that this limits us to 2,097,150 unique attributes for positions, normals and textures
					const uint64_t mask = 0b0'000000000000000000000'000000000000000000000'111111111111111111111;
					uint64_t key = ((vertexIndices.x & mask) << 42) | ((vertexIndices.y & mask) << 21) | (vertexIndices.z & mask);

					// Find the index associated with the combination of attributes
					auto it = indexMap.find(key);

					// If it exists, we push the index to our indices
					if (it != indexMap.end()) {
						edges[ix] = it->second;
					}
					else {
						// Construct a new vertex using the indices for the vertex
						VertexPosNormTexCol vertex;
						vertex.Position = positions[vertexIndices.x - 1];
						vertex.UV = vertexIndices.y != 0 ? textureCoords[vertexIndices.y - 1] : glm::vec2(0.0f);
						vertex.Normal = vertexIndices.z != 0 ? normals[vertexIndices.z - 1] : glm::vec3(0.0f, 0.0f, 1.0f);
						vertex.Color = inColor;

						// Add to the mesh, get index of the added vertex
						uint32_t index = mesh.AddVertex(vertex);
						// Cache the index based on our key
						indexMap[key] = index;
						// Add index to mesh, and add to edges list for if we are using quads
						edges[ix] = index;
					}
				}
				else {
					break;
				}
			}
			// Handling for trangle faces
			if (ix == 3) {
				mesh.AddIndexTri(edges[0], edges[1], edges[2]);
			}
			// Handling for quad faces
			else if (ix == 4) {
				mesh.AddIndexTri(edges[0], edges[1], edges[2]);
				mesh.AddIndexTri(edges[0], edges[2], edges[3]);
			}
		}
	}

	// Note: with actual OBJ files you're going to run into the issue where faces are composited of different indices
	// You'll need to keep track of these and create vertex entries for each vertex in the face
	// If you want to get fancy, you can track which vertices you've already added

	
	return mesh;
}

MorphLoader::~MorphLoader()
{
}

void MorphLoader::UpdateData(int frameIndex0, int frameIndex1, float t)
{
	m_vao = VertexArrayObject::Create();



	//VertexBuffer::sptr vboPos = VertexBuffer::Create();
	//vboPos->LoadData(loadedPositions.data(), loadedPositions.size());

	//VertexBuffer::sptr vboNorm = VertexBuffer::Create();
	//vboNorm->LoadData(loadedNormals.data(), loadedNormals.size());

	//VertexBuffer::sptr vboUV = VertexBuffer::Create();
	//vboUV->LoadData(loadedNormals.data(), loadedNormals.size());


}



void MorphLoader::blendTo(const std::string& fileName, float delay, int frame)
{
}

void MorphLoader::Init()
{
	//m_shader = Shader::Create();
	//m_shader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	//m_shader->LoadShaderPartFromFile("shaders/frag_blinn_phong_textured.glsl", GL_FRAGMENT_SHADER);
	//m_shader->Link();
}

void MorphLoader::Unload()
{
}

void MorphLoader::beginDraw()
{
}

void MorphLoader::Update(float dt)
{
	float t;

	if (m_anim->m_frameTime > 0.0f)
	{
		m_timer += dt;

		if (m_timer > m_anim->m_frameTime)
		{
			m_timer -= m_anim->m_frameTime;

			m_anim->frameIndex += 1;

			if (m_anim->frameIndex >= m_anim->frames.size())
				m_anim->frameIndex = 0;
		}

		m_timer = fmod(m_timer, m_anim->m_frameTime);

		t = m_timer / m_anim->m_frameTime;
	}
	else
	{
		t = 0.0f;
	}

	size_t f0Index;
	size_t f1Index;

	f1Index = m_anim->frameIndex;

	if (f1Index == 0)
	{
		f0Index = m_anim->frames.size() - 1;
	}
	else
	{
		f0Index = f1Index - 1;
	}


}

void MorphLoader::Draw(const glm::mat4& model)
{
	
	//m_defaultQueue.push_back({ m_t, m_vao, model, 0 });
	
}

void MorphLoader::SetFrames(const std::vector<std::unique_ptr<MeshBuilder<VertexPosNormTexCol>>>& frames)
{
	m_anim->frames.clear();
	m_anim->frames.reserve(frames.size());

	for (int x = 0; x < frames.size(); x++)
	{

		//m_anim->frames.push_back();
	}
}

void MorphLoader::SetFrameTime(float frameTime)
{

}
