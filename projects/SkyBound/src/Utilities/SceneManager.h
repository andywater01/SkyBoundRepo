#pragma once
#include "entt.hpp"

class SceneManager
{
public:
	SceneManager() { m_sceneReg = new entt::registry(); }
	SceneManager(std::string name);
	~SceneManager() { }

	void Unload();

	//Each scene will need to have a different
	//init, as each scene's contents will be different
	virtual void InitScene(float windowWidth, float windowHeight) { printf("windowwidth: %f, windowHeight: %f", windowWidth, windowHeight); };

	//Saves the scene
	void SaveScene();

	//Get the scene registry
	entt::registry* GetScene() const;
	//Set the scene registry
	void SetScene(entt::registry& scene);
	std::string GetName() const;
	void SetName(std::string name);

	//Set window size (makes sure the camera aspect is proper)
	void SetWindowSize(float windowWidth, float windowHeight);

	//entt::registry* m_sceneReg = nullptr;
protected:
	entt::registry* m_sceneReg = nullptr;
	std::string m_name = "Default Name";
};