#include "SceneManager.h"

SceneManager::SceneManager(std::string name)
{
	m_name = name;
}

void SceneManager::Unload()
{
	if (m_sceneReg != nullptr)
	{
		delete m_sceneReg;
		m_sceneReg = nullptr;
	}
}


entt::registry* SceneManager::GetScene() const
{
	return m_sceneReg;
}

void SceneManager::SetScene(entt::registry& scene)
{
	m_sceneReg = &scene;
}

std::string SceneManager::GetName() const
{
	return m_name;
}

void SceneManager::SetName(std::string name)
{
	m_name = name;
}
