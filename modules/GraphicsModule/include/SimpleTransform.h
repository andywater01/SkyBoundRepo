#ifndef _SIMPLE_TRANSFORM_H
#define _SIMPLE_TRANSFORM_H

#include <GLM/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/quaternion.hpp>
#include <GLM/gtc/matrix_transform.hpp>

struct SimpleTransform
{
public:
    std::vector<SimpleTransform*> m_children;

    glm::mat4 m_globalTransform;

    glm::vec3 m_position = glm::vec3(0, 0, 0);
    glm::quat m_rotation = glm::quat(1, 0, 0, 0);
    glm::vec3 m_scale = glm::vec3(1, 1, 1);

    glm::mat4 GetLocalTransformMatrix()
    {
        return glm::translate(glm::mat4(1.0f), m_position) *
            glm::scale(glm::mat4(1.0f), m_scale) *
            glm::toMat4(m_rotation);
    }

    void UpdateGlobalTransform(glm::mat4 parentTransform)
    {
        glm::mat4 localTransform = GetLocalTransformMatrix();

        m_globalTransform = parentTransform * localTransform;

        for (SimpleTransform* child : m_children)
        {
            child->UpdateGlobalTransform(m_globalTransform);
        }
    }
};

#endif