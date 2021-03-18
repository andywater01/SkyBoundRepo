#include "SkinnedMesh.h"

VertexPosNormTexJointWeight* VPNTJW = nullptr;
// Custom buffer attribute vector for VAO
const std::vector<BufferAttribute> VertexPosNormTexJointWeight::V_DECL = {
	BufferAttribute(0, 3, GL_FLOAT, false, sizeof(VertexPosNormTexJointWeight), (size_t)&VPNTJW->Position,
	AttribUsage::Position),
	BufferAttribute(2, 3, GL_FLOAT, false, sizeof(VertexPosNormTexJointWeight), (size_t)&VPNTJW->Normal,
	AttribUsage::Normal),
	BufferAttribute(3, 2, GL_FLOAT, false, sizeof(VertexPosNormTexJointWeight), (size_t)&VPNTJW->UV,
	AttribUsage::Texture),
	BufferAttribute(4, 4, GL_FLOAT, false, sizeof(VertexPosNormTexJointWeight), (size_t)&VPNTJW->Joints,
	AttribUsage::User0),
	BufferAttribute(5, 4, GL_FLOAT, false, sizeof(VertexPosNormTexJointWeight), (size_t)&VPNTJW->Weights,
	AttribUsage::User1)
};

GLTFSkinnedMesh::~GLTFSkinnedMesh()
{
	// Free the memory we created for transforms
	for (auto skinData : m_skinData)
	{
		for (int i = 0; i < skinData.second.m_transforms.size(); i++)
		{
			if (skinData.second.m_transforms[i] != nullptr)
			{
				delete skinData.second.m_transforms[i];
				skinData.second.m_transforms[i] = nullptr;
			}
		}
	}
}

bool GLTFSkinnedMesh::LoadFromFile(std::string const path)
{
	// **************************************************************
	// * LoadBinaryFromFile should be used with .glb files			*
	// * (the b is short for binary)								*
	// *															*
	// * LoadASCIIFromFile should be used with .gltf files			*
	// **************************************************************

	// Get file extension
	size_t extIndex = path.find_last_of('.');
	if (extIndex == std::string::npos)
	{
		printf("Error: No file extension found at path %s \n", path.c_str());
		return false;
	}

	tinygltf::TinyGLTF loader;

	std::string err;
	std::string warn;

	bool ret = false;

	// Actually load the data into our model
	std::string extension = path.substr(extIndex + 1);
	if (extension == "glb")
	{
		ret = loader.LoadBinaryFromFile(&m_model, &err, &warn, path);
	}
	else if (extension == "gltf")
	{
		ret = loader.LoadASCIIFromFile(&m_model, &err, &warn, path);
	}
	else
	{
		printf("Error: Attempting to load a .%s file as a gltf/glb!\n", extension.c_str());
		return false;
	}

	// Note, string.empty does not edit the value of the string
	// It just returns true if the string contains no value
	// Take this as a lesson to properly name your functions
	// string.IsEmpty() would've been 1000 times better :(

	// Display and warnings or errors that were encountered
	if (!warn.empty())
	{
		printf("TinyGLTF warning: %s\n", warn.c_str());
	}

	if (!err.empty())
	{
		printf("TinyGLTF error: %s\n", err.c_str());
	}

	if (!ret)
	{
		printf("Failed to load .gltf/.glb!\n");
		return false;
	}

	// Congratualtions! We now have our model loaded! :)
	// Now here comes the fun part, *reading* it 

	// ******************************************
	// * TODO: Add an error check to make sure:	*
	// * - The model's scene is not empty		*
	// * - The default scene index is in bounds *
	// ******************************************
	
	return LoadNodes();
}

SimpleTransform* const GLTFSkinnedMesh::FindNodeSimpleTransform(int const nodeIndex) const
{
	for (auto skinData : m_skinData)
	{
		for (auto transform : skinData.second.m_transforms)
		{
			if (nodeIndex == transform.first)
			{
				return transform.second;
			}
		}
	}

	return nullptr;
}

bool GLTFSkinnedMesh::LoadNodes()
{
	for (int i = 0; i < m_model.nodes.size(); i++)
	{
		tinygltf::Node& node = m_model.nodes[i];

		// Valid mesh found in node
		if (node.mesh > -1)
		{
			// map.count() searches an std::map for the given key passed as the parameter for the function
			// Returns 0 if the map does not contain the key
			// Returns 1 if it does
			if (!m_loadedMeshes.count(node.mesh))
			{
				// Load mesh
				LoadMesh(node.mesh, node.skin);
			}
		}

		// Valid skin was found in the node
		if (node.skin > -1)
		{
			LoadSkin(node.skin);
		}
	}

	return true;
}

// Used to retrieve joint data
// GLTF stores joint data as unsigned shorts
typedef glm::vec<4, unsigned short, glm::highp> UShortVec4;

bool GLTFSkinnedMesh::LoadMesh(int const meshIndex, int const associatedSkin)
{
	tinygltf::Mesh& mesh = m_model.meshes[meshIndex];

	// Loop through mesh primitives
	for (int i = 0; i < mesh.primitives.size(); i++)
	{
		tinygltf::Primitive& primitive = mesh.primitives[i];

		std::vector<glm::vec3> positions =
			RetrieveModelData<glm::vec3>(primitive.attributes["POSITION"], TINYGLTF_TYPE_VEC3);
		std::vector<glm::vec3> normals =
			RetrieveModelData<glm::vec3>(primitive.attributes["NORMAL"], TINYGLTF_TYPE_VEC3);
		std::vector<glm::vec2> uvs =
			RetrieveModelData<glm::vec2>(primitive.attributes["TEXCOORD_0"], TINYGLTF_TYPE_VEC2);

		std::vector<UShortVec4> jointsRaw =
			RetrieveModelData<UShortVec4>(primitive.attributes["JOINTS_0"], TINYGLTF_TYPE_VEC4);
		std::vector<glm::vec4> joints;
		// Combine raw joint data into vec4s
		for (int i = 0; i < jointsRaw.size(); i++)
		{
			joints.push_back(glm::vec4(jointsRaw[i].x, jointsRaw[i].y, jointsRaw[i].z, jointsRaw[i].w));
		}
		std::vector<glm::vec4> weights =
			RetrieveModelData<glm::vec4>(primitive.attributes["WEIGHTS_0"], TINYGLTF_TYPE_VEC4);

		// Combine all the data into vertices for the VAO
		std::vector<VertexPosNormTexJointWeight> vertices(positions.size());
		for (int i = 0; i < vertices.size(); i++)
		{
			vertices[i].Position = positions[i];
			vertices[i].Normal = normals[i];
			vertices[i].UV = uvs[i];
			vertices[i].Joints = joints[i];
			vertices[i].Weights = weights[i];
		}

		// Get triangle indices
		std::vector<unsigned short> indices_short =
			RetrieveModelData<unsigned short>(primitive.indices, TINYGLTF_TYPE_SCALAR);
		std::vector<uint32_t> indices(begin(indices_short), end(indices_short));

		// Create a VAO for our mesh
		// Did you get an error on this line?
		// If glcreatebuffers has a read access violation at this line, make sure you init GLFW and GLAD first!
		// (Trust me I lost a lot of time to that lol)
		VertexBuffer::sptr vbo = VertexBuffer::Create();
		vbo->LoadData(vertices.data(), vertices.size());

		IndexBuffer::sptr ebo = IndexBuffer::Create();
		ebo->LoadData(indices.data(), indices.size());

		VertexArrayObject::sptr vao = VertexArrayObject::Create();
		vao->AddVertexBuffer(vbo, VertexPosNormTexJointWeight::V_DECL);
		vao->SetIndexBuffer(ebo);

		m_loadedMeshes[meshIndex].m_primitives.push_back(vao);
		m_loadedMeshes[meshIndex].m_associatedSkin = associatedSkin;
	}

	return true;
}

bool GLTFSkinnedMesh::LoadSkin(int const skinIndex)
{
	tinygltf::Skin& skin = m_model.skins[skinIndex];
	GLTFSkinData skinData;
	skinData.m_skin = &skin;

	// Load skin joints
	if (skin.joints.size() > 0)
	{
		skinData.m_inverseBindMatrices =
			RetrieveModelData<glm::mat4x4>(skin.inverseBindMatrices, TINYGLTF_TYPE_MAT4);

		// Load joint transforms
		for (int i = 0; i < skin.joints.size(); i++)
		{
			int jointIndex = skin.joints[i];
			SimpleTransform* transform = new SimpleTransform();
			CopyNodeLocalTransform(m_model.nodes[jointIndex], *transform);

			skinData.m_transforms.emplace(jointIndex, transform);
		}

		// Get root transform
		skinData.m_transformRoot = skinData.m_transforms[skin.joints[0]];

		// Setup joint transform parent relationships
		for (int i = 0; i < skin.joints.size(); i++)
		{
			int jointIndex = skin.joints[i];

			// Get indices of children
			std::vector<int> children = m_model.nodes[jointIndex].children;
			for (int c = 0; c < children.size(); c++)
			{
				int childIndex = children[c];

				if (jointIndex == childIndex)
				{
					// The joint has itself listed as a parent! That's a big issue :((((
					abort();
				}

				if (skinData.m_transforms.count(childIndex))
				{
					skinData.m_transforms[jointIndex]->m_children.push_back(
						skinData.m_transforms[childIndex]);
				}
			}
		}
	}

	skinData.RecalculateJoints();

	m_skinData.emplace(skinIndex, skinData);
	return true;
}

template<typename T, typename ComponentType>
std::vector<T> GLTFSkinnedMesh::RetrieveDataFromAccessor(tinygltf::Accessor& const accessor) const
{
	// Bufferviews reference a buffer and contain essential data for reading said buffer
	tinygltf::BufferView bufferView = m_model.bufferViews[accessor.bufferView];

	// Buffers have the actual data!
	tinygltf::Buffer buffer = m_model.buffers[bufferView.buffer];

	ComponentType* const dataPtr =
		reinterpret_cast<ComponentType* const> 
		(buffer.data.data() + accessor.byteOffset + bufferView.byteOffset);

	std::vector<T> dataRetrieved(accessor.count);

	for (int i = 0; i < accessor.count; i++)
	{
		T data;
		// Retrieve the data from the model
		memcpy(&data, dataPtr + i * (sizeof(T) / sizeof(ComponentType)), sizeof(T));

		dataRetrieved[i] = data;
	}

	return dataRetrieved;
}

template<typename T, typename ComponentType>
T GLTFSkinnedMesh::RetrieveDataFromAccessor(tinygltf::Accessor& const accessor, unsigned int const dataIndex) const
{
	// Bufferviews reference a buffer and contain essential data for reading said buffer
	tinygltf::BufferView bufferView = m_model.bufferViews[accessor.bufferView];

	// Buffers have the actual data!
	tinygltf::Buffer buffer = m_model.buffers[bufferView.buffer];

	ComponentType* const dataPtr =
		reinterpret_cast<ComponentType* const>
		(buffer.data.data() + accessor.byteOffset + bufferView.byteOffset);

	if (accessor.count <= dataIndex)
	{
		printf("Error retrieving data from GLTF model!");
		return T();
	}

	T data;
	memcpy(&data, dataPtr + dataIndex * (sizeof(T) / sizeof(ComponentType)), sizeof(T));
	return data;
}

template <typename T>
std::vector<T> GLTFSkinnedMesh::RetrieveModelData(int const accessorIndex, int const accessorType) const
{
	if (accessorIndex > -1)
	{
		// Accessors define how bufferview data is interpreted
		tinygltf::Accessor accessor = m_model.accessors[accessorIndex];
		assert(accessor.type == accessorType);

		if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
		{
			return RetrieveDataFromAccessor<T, unsigned short>(accessor);
		}
		else
		{
			// Assume float if unrecognized component type
			return RetrieveDataFromAccessor<T, float>(accessor);
		}
	}

	printf("Error retrieving data from GLTF model!");
	std::vector<T> nullVal(0);
	return nullVal;
}

template <typename T>
T GLTFSkinnedMesh::RetrieveModelData(int const accessorIndex, int const accessorType, unsigned int const dataIndex) const
{
	if (accessorIndex > -1)
	{
		// Accessors define how bufferview data is interpreted
		tinygltf::Accessor accessor = m_model.accessors[accessorIndex];
		assert(accessor.type == accessorType);

		if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
		{
			return RetrieveDataFromAccessor<T, unsigned short>(accessor, dataIndex);
		}
		else
		{
			// Assume float if unrecognized component type
			return RetrieveDataFromAccessor<T, float>(accessor, dataIndex);
		}
	}

	printf("Error retrieving data from GLTF model!");
	return T();
}

template<typename T>
T GLTFSkinnedMesh::SampleAnimationChannel(tinygltf::AnimationChannel& const channel, float time, tinygltf::Animation& const animation, int const accessorType)
{
	tinygltf::AnimationSampler& sampler = animation.samplers[channel.sampler];

	// Linear and step interpolation animations contain only the keyframes
	// Cubicspline interpolation contains both the keyframes, as well as the tangent handles
	// As such, in Cubicspline animations there is a lot of junk data we need to sort through to sample our animation
	std::string samplerInterpolation = sampler.interpolation;

	int keyframesIndex = sampler.input;

	std::vector<float> keyframeTimes =
		RetrieveModelData<float>(keyframesIndex, TINYGLTF_TYPE_SCALAR);

	if (time < 0)
	{
		time = 0;
	}

	int keyframeLowerBound = 0;
	float t = 0;
	for (int i = 0; i < keyframeTimes.size() - 1; i++)
	{
		if (time > keyframeTimes[i] && time <= keyframeTimes[i + 1])
		{
			keyframeLowerBound = i;

			// Inverse lerp to find the interpolation amount
			t = (time - keyframeTimes[i]) / (keyframeTimes[i + 1] - keyframeTimes[i]);

			break;
		}
	}

	// Clamp our sample time to the end of the animation
	if (time > GetAnimationLength(animation))
	{
		t = 1;
		keyframeLowerBound = keyframeTimes.size() - 2;
	}

	int frame1Index = keyframeLowerBound;
	int frame2Index = keyframeLowerBound + 1;

	// Treat the keyframe data as groups of 3 
	// IF and ONLY IF the interpolation is cublic spline
	if (samplerInterpolation == "CUBICSPLINE")
	{
		frame1Index *= 3;
		frame1Index++;

		frame2Index *= 3;
		frame2Index++;
	}

	T frame1 = RetrieveModelData<T>(sampler.output, accessorType, frame1Index);
	T frame2 = RetrieveModelData<T>(sampler.output, accessorType, frame2Index);

	// Linear interpolation!!
	return (1.0f - t) * frame1 + t * frame2;
}

void GLTFSkinnedMesh::CopyNodeLocalTransform(tinygltf::Node& const node, SimpleTransform& transform)
{
	// Default Values
	glm::vec3 position(0, 0, 0);
	glm::quat rotation(1, 0, 0, 0); // Constructor is in format (w, x, y, z), identity quaternion should be (0, 0, 0, 1)
	glm::vec3 scale(1, 1, 1);

	// Memcpy can't be used here since node.translation etc. are stored as doubles
	if (node.translation.size() > 0)
	{
		for (int i = 0; i < 3; i++)
		{
			position[i] = node.translation[i];
		}
	}

	if (node.rotation.size() > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			rotation[i] = node.rotation[i];
		}
	}

	if (node.scale.size() > 0)
	{
		for (int i = 0; i < 3; i++)
		{
			scale[i] = node.scale[i];
		}
	}

	transform.m_position = position;
	transform.m_rotation = rotation;
	transform.m_scale = scale;
}

void GLTFSkinnedMesh::Draw(Shader::sptr& const shader, glm::mat4& const viewProjection, glm::mat4& const worldTransform)
{
	shader->SetUniformMatrix("u_ModelViewProjection", viewProjection * worldTransform);
	shader->SetUniformMatrix("u_Model", worldTransform);
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(worldTransform)));
	shader->SetUniformMatrix("u_NormalMatrix", normalMatrix);

	shader->Bind();

	for (auto const mesh : m_loadedMeshes)
	{
		int skinIndex = mesh.second.m_associatedSkin;
		if (m_skinData.count(skinIndex))
		{
			m_skinData[skinIndex].RecalculateJoints();
			shader->SetUniformMatrix(shader->GetUniformLocation("u_JointMatrices"),
				m_skinData[skinIndex].m_jointMatrices.data(), MAX_BONES, false);
		}

		for (int i = 0; i < mesh.second.m_primitives.size(); i++)
		{
			mesh.second.m_primitives[i]->Render();
		}
	}

	shader->UnBind();
}

float GLTFSkinnedMesh::GetAnimationLength(tinygltf::Animation& const animation)
{
	float length = 0;
	for (int i = 0; i < animation.samplers.size(); i++)
	{
		tinygltf::AnimationSampler& const sampler = animation.samplers[i];
		tinygltf::Accessor& const accessor = m_model.accessors[sampler.input];

		float time = RetrieveModelData<float>(sampler.input, TINYGLTF_TYPE_SCALAR, accessor.count - 1);

		if (time > length)
		{
			length = time;
		}
	}
	return length;
}

void GLTFSkinnedMesh::UpdateAnimation(tinygltf::Animation& const animation, float deltaTime)
{
	m_animationTime += deltaTime;

	if (m_animationLooping)
	{
		if (m_animationTime > GetAnimationLength(animation))
		{
			m_animationTime = 0;
		}
	}

	for (int i = 0; i < animation.channels.size(); i++)
	{
		tinygltf::AnimationChannel& const channel = animation.channels[i];

		// Check if the target node exists
		SimpleTransform* const transform = FindNodeSimpleTransform(channel.target_node);

		if (!transform)
		{
			continue;
		}

		if (channel.target_path == "translation")
		{
			glm::vec3 translation =
				SampleAnimationChannel<glm::vec3>(channel, m_animationTime, animation, TINYGLTF_TYPE_VEC3);
			transform->m_position = translation;
		}
		else if (channel.target_path == "rotation")
		{
			glm::vec4 rotation =
				SampleAnimationChannel<glm::vec4>(channel, m_animationTime, animation, TINYGLTF_TYPE_VEC4);
			transform->m_rotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
		}
		else if (channel.target_path == "scale")
		{
			glm::vec3 scale =
				SampleAnimationChannel<glm::vec3>(channel, m_animationTime, animation, TINYGLTF_TYPE_VEC3);
			transform->m_scale = scale;
		}
	}
}

void GLTFSkinData::RecalculateJoints()
{
	if (m_jointMatrices.size() == 0)
	{
		m_jointMatrices.resize(m_skin->joints.size());
	}

	m_transformRoot->UpdateGlobalTransform(glm::mat4(1.0f));

	for (int i = 0; i < m_skin->joints.size(); i++)
	{
		int jointIndex = m_skin->joints[i];
		m_jointMatrices[i] = (m_transforms[jointIndex]->m_globalTransform * m_inverseBindMatrices[i]);
	}
}
