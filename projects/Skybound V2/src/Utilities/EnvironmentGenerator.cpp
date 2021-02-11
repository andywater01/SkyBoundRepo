#include "EnvironmentGenerator.h"

//The gameobject references to the spawned objects
std::vector<std::vector<GameObject>> EnvironmentGenerator::_objectsSpawned;

//Object information for being spawned
std::vector<VertexArrayObject::sptr> EnvironmentGenerator::_vaosToSpawn;
std::vector<bool> EnvironmentGenerator::_loadedIn;
std::vector<ShaderMaterial::sptr> EnvironmentGenerator::_materialsForSpawning;
std::vector<int> EnvironmentGenerator::_numToSpawn;
std::vector<glm::vec2> EnvironmentGenerator::_spawnFromAll;
std::vector<glm::vec2> EnvironmentGenerator::_spawnToAll;
std::vector<std::vector<glm::vec2>> EnvironmentGenerator::_avoidFromAll;
std::vector<std::vector<glm::vec2>> EnvironmentGenerator::_avoidToAll;

//The filenames of the objects to spawn
std::vector<std::string> EnvironmentGenerator::_objectsToSpawn;

////Not implemented//
//std::vector<char> EnvironmentGenerator::_letterRepresentation;
//std::vector<std::vector<char>> EnvironmentGenerator::_generatedMapPlacements;
//std::vector<std::vector<float>> EnvironmentGenerator::_generatedMapHeight;

void EnvironmentGenerator::RegenerateEnvironment()
{
	CleanEnvironment();

	GenerateEnvironment();
}

void EnvironmentGenerator::GenerateEnvironment()
{
	for (int i = 0; i < _objectsToSpawn.size(); i++)
	{
		std::vector<GameObject> temp;
		{
			//Load in this object vao
			if (!_loadedIn[i])
			{
				VertexArrayObject::sptr vao = ObjLoader::LoadFromFile(_objectsToSpawn[i]);
				_vaosToSpawn.push_back(vao);
				_loadedIn[i] = true;
			}

			for (int j = 0; j < _numToSpawn[i]; j++)
			{
				temp.push_back(Application::Instance().ActiveScene->CreateEntity(_objectsToSpawn[i] + (std::to_string(j + 1))));
				temp[j].emplace<RendererComponent>().SetMesh(_vaosToSpawn[i]).SetMaterial(_materialsForSpawning[i]);
				//Randomly places
				temp[j].get<Transform>().SetLocalPosition(glm::vec3(Util::GetRandomNumberBetween(_spawnFromAll[i],
					_spawnToAll[i], _avoidFromAll[i], _avoidToAll[i]), 0.0f));
				temp[j].get<Transform>().SetLocalRotation(Util::GetRandomNumberBetween(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 360.0f)));
			}
		}

		//Add object to the spawned list
		_objectsSpawned.push_back(temp);
	}
}

void EnvironmentGenerator::CleanEnvironment()
{
	//Remove all the entities
	for (int i = 0; i < _objectsSpawned.size(); i++)
	{
		for (int j = 0; j < _objectsSpawned[i].size(); j++)
		{
			Application::Instance().ActiveScene->RemoveEntity(_objectsSpawned[i][j]);
		}
	}

	//Clear out objects spawned
	_objectsSpawned.clear();
}

void EnvironmentGenerator::CleanUpPointers()
{
	//Clear up vao references so the smart pointers can clear
	_vaosToSpawn.clear();
	//Clear up material references so the smart pointers can clear
	_materialsForSpawning.clear();
}

void EnvironmentGenerator::AddObjectToGeneration(std::string fileName, ShaderMaterial::sptr objMat, int numToSpawn, glm::vec2 spawnFrom, 
													glm::vec2 spawnTo, std::vector<glm::vec2> avoidFrom, std::vector<glm::vec2> avoidTo)
{
	//Find the filename in the list
	int index = Util::FindInVector(fileName, _objectsToSpawn);
	//If the filename was found in the list we ain't adding it again
	if (index != -1)
	{
		printf("Object already found in list\n");
		return;
	}

	//Loads in the mesh and adds to list
	VertexArrayObject::sptr vao = ObjLoader::LoadFromFile(fileName);
	_vaosToSpawn.push_back(vao);
	//Adds material to list
	_materialsForSpawning.push_back(objMat);
	//Adds number to spawn for this object
	_numToSpawn.push_back(numToSpawn);

	//Adds areas to spawn and not spawn
	_spawnFromAll.push_back(spawnFrom);
	_spawnToAll.push_back(spawnTo);
	_avoidFromAll.push_back(avoidFrom);
	_avoidToAll.push_back(avoidTo);

	//Adds the filename to the list
	_objectsToSpawn.push_back(fileName);
	//Sets it as not loaded
	_loadedIn.push_back(false);
}

void EnvironmentGenerator::RemoveObjectFromGeneration(std::string fileName)
{
	int index = Util::FindInVector(fileName, _objectsToSpawn);
	if (index == -1)
	{
		printf("Object not found in list\n");
		return;
	}

	//Erase from the vaosToSpawn, Materials, numbers, etc
	_vaosToSpawn.erase(_vaosToSpawn.begin() + index);
	_loadedIn.erase(_loadedIn.begin() + index);
	_materialsForSpawning.erase(_materialsForSpawning.begin() + index);
	_numToSpawn.erase(_numToSpawn.begin() + index);
	_avoidFromAll.erase(_avoidFromAll.begin() + index);
	_avoidToAll.erase(_avoidToAll.begin() + index);
	
	//erase the filename from the list
	_objectsToSpawn.erase(_objectsToSpawn.begin() + index);
}

std::vector<std::string> EnvironmentGenerator::GetObjectsOnList()
{
	return _objectsToSpawn;
}
