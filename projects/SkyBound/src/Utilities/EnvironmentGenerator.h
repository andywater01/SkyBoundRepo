#pragma once
#include <Scene.h>
#include <Application.h>
#include <ObjLoader.h>
#include <RendererComponent.h>
#include <Transform.h>
#include <vector>

#include "Utilities/Util.h"

class EnvironmentGenerator abstract
{
public:
	
	//Regenerates environment with your settings
	static void RegenerateEnvironment();
	//Generates an environment with your settings
	static void GenerateEnvironment();
	//Cleans up the environment using your settings
	static void CleanEnvironment();
	
	static void CleanUpPointers();

	//Adds object to generation
	static void AddObjectToGeneration(std::string fileName, ShaderMaterial::sptr objMat, int numToSpawn, 
										glm::vec2 spawnFrom, glm::vec2 spawnTo, std::vector<glm::vec2> avoidFrom, 
											std::vector<glm::vec2> avoidTo);
	//Removes object from generation
	static void RemoveObjectFromGeneration(std::string fileName);

	static std::vector<std::string> GetObjectsOnList();
private:
	//The gameobjects spawned here
	static std::vector<std::vector<GameObject>> _objectsSpawned;

	//The vaos to spawn in
	static std::vector<VertexArrayObject::sptr> _vaosToSpawn;
	static std::vector<bool> _loadedIn;
	static std::vector<ShaderMaterial::sptr> _materialsForSpawning;
	static std::vector<int> _numToSpawn;
	static std::vector<glm::vec2> _spawnFromAll;
	static std::vector<glm::vec2> _spawnToAll;
	static std::vector<std::vector<glm::vec2>> _avoidFromAll;
	static std::vector<std::vector<glm::vec2>> _avoidToAll;

	//Allows us to go through and remove from list
	static std::vector<std::string> _objectsToSpawn;

	////////Not Implemented/////
	//static std::vector<char> _letterRepresentation;
	//static std::vector<std::vector<char>> _generatedMapPlacements;
	//static std::vector<std::vector<float>> _generatedMapHeight;
};