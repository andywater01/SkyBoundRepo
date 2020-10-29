#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "GLM/glm.hpp"
#include "GLM/gtx/quaternion.hpp"

#include <vector>

//Simple implementation of a transform component.


class Transform
{
public:

	glm::vec3 m_pos;
	glm::vec3 m_scale;
	glm::quat m_rotation;

	Transform();
	Transform(const Transform& other) = default;
	virtual ~Transform();


	void DoFK();


	const glm::mat4& RecomputeGlobal();


	const glm::mat4& GetGlobal() const;


	glm::mat3 GetNormal() const;


	void SetParent(Transform* parent);

protected:

	Transform* m_parent;
	std::vector<Transform*> m_children;

	glm::mat4 m_global;

	
	void AddChild(Transform* child);
	void RemoveChild(Transform* child);
};


