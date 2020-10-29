#include "Transform.h"
#include "GLM/gtx/transform.hpp"


Transform::Transform()
{
	m_parent = nullptr;

	m_pos = glm::vec3(0.0f);
	m_scale = glm::vec3(1.0f);
	m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	m_global = glm::mat4(1.0f);
}

Transform::~Transform()
{
	SetParent(nullptr);
}

void Transform::DoFK()
{
	glm::mat4 local = glm::translate(m_pos) *
		glm::toMat4(glm::normalize(m_rotation)) *
		glm::scale(m_scale);

	
	if (m_parent != nullptr)
		m_global = m_parent->m_global * local;

	else
		m_global = local;

	
	for (auto* child : m_children)
	{
		child->DoFK();
	}
}

const glm::mat4& Transform::RecomputeGlobal()
{

	glm::mat4 local = glm::translate(m_pos) *
		glm::toMat4(m_rotation) *
		glm::scale(m_scale);

	if (m_parent != nullptr)
		m_global = m_parent->RecomputeGlobal() * local;
	else
		m_global = local;

	return m_global;
}

const glm::mat4& Transform::GetGlobal() const
{
	return m_global;
}

glm::mat3 Transform::GetNormal() const
{
	
	if (m_scale.x == m_scale.y && m_scale.x == m_scale.z)
		return glm::mat3(m_global);

	
	return glm::inverse(glm::transpose(glm::mat3(m_global)));
}

void Transform::SetParent(Transform* parent)
{
	
	if (m_parent != nullptr)
		m_parent->RemoveChild(this);

	m_parent = parent;

	
	if (m_parent != nullptr)
		m_parent->AddChild(this);
}

void Transform::AddChild(Transform* child)
{
	m_children.push_back(child);
}

void Transform::RemoveChild(Transform* child)
{
	for (auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		if (*it == child)
		{
			m_children.erase(it);
			break;
		}

	}
}