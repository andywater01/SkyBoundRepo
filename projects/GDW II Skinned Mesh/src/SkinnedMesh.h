#ifndef _SKINNED_MESH_H
#define _SKINNED_MESH_H

#include <string>

#include <tiny_gltf.h>
#include <GLM/glm.hpp>

#include "Graphics/VertexArrayObject.h"
#include "Graphics/Shader.h"

#include "SimpleTransform.h"

// Ensure this is the same number as your vertex shader
#define MAX_BONES 20

// Custom vertex for VAO to store per-vertex joint and weight information
struct VertexPosNormTexJointWeight
{
public:
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 UV;
    glm::vec4 Joints;
    glm::vec4 Weights;

    VertexPosNormTexJointWeight() :
        Position(glm::vec3(0.0f)),
        Normal(glm::vec3(0.0f)),
        UV(glm::vec2(0.0f)),
        Joints(glm::vec4(0.0f)),
        Weights(glm::vec4(0.0f)) {}

    VertexPosNormTexJointWeight(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& uv,
        const glm::vec4& joints, const glm::vec4& weights) :
        Position(glm::vec3(pos)),
        Normal(glm::vec3(norm)),
        UV(glm::vec2(uv)),
        Joints(glm::vec4(joints)),
        Weights(glm::vec4(weights)) {}

    static const std::vector<BufferAttribute> V_DECL;
};

struct GLTFMesh
{
public:
    std::vector<VertexArrayObject::sptr> m_primitives;
    int m_associatedSkin;
};

struct GLTFSkinData
{
public:
    GLTFSkinData() {}

    void RecalculateJoints();

    tinygltf::Skin* m_skin;

    std::vector<glm::mat4x4> m_inverseBindMatrices;
    std::vector<glm::mat4x4> m_jointMatrices;

    SimpleTransform* m_transformRoot;
    std::map<int, SimpleTransform*> m_transforms;
};

// Forward declare shader class
class Shader;

class GLTFSkinnedMesh
{
public:

    ~GLTFSkinnedMesh();

    bool LoadFromFile(std::string const path);

    template<typename T, typename ComponentType>
    std::vector<T> RetrieveDataFromAccessor(tinygltf::Accessor& const accessor) const;
    template<typename T, typename ComponentType>
    T RetrieveDataFromAccessor(tinygltf::Accessor& const accessor, unsigned int const dataIndex) const;

    template <typename T>
    std::vector<T> RetrieveModelData(int const accessorIndex, int const accessorType) const;
    template <typename T>
    T RetrieveModelData(int const accessorIndex, int const accessorType, unsigned int const dataIndex) const;

    void CopyNodeLocalTransform(tinygltf::Node& const node, SimpleTransform& transform);

    void Draw(Shader::sptr& const shader, glm::mat4& const viewProjection, glm::mat4& const worldTransform);

    template<typename T>
    T SampleAnimationChannel(tinygltf::AnimationChannel& const channel, float time,
        tinygltf::Animation& const animation, int const accessorType = TINYGLTF_TYPE_VEC3);

    tinygltf::Animation& const GetAnimation(int const animationIndex)
    {
        return m_model.animations[animationIndex];
    }

    float GetAnimationLength(tinygltf::Animation& const animation);
    void UpdateAnimation(tinygltf::Animation& const animation, float deltaTime);

    void SetLooping(bool const shouldLoop) { m_animationLooping = shouldLoop; }

    SimpleTransform* const FindNodeSimpleTransform(int const nodeIndex) const;

private:
    bool LoadNodes();
    bool LoadMesh(int const meshIndex, int const associatedSkin);
    bool LoadSkin(int const skinIndex);

    tinygltf::Model m_model;

    // GLTF Models could contain multiple meshes and skins
    std::map<int, GLTFMesh> m_loadedMeshes;
    std::map<int, GLTFSkinData> m_skinData;

    bool m_animationLooping = false;
    float m_animationTime = 0.0f;

	VertexArrayObject::sptr vao = VertexArrayObject::Create();
};

#endif // !_SKINNED_MESH_H

