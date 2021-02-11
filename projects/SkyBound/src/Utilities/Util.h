#pragma once
#include <GLM/glm.hpp>
#include <time.h>
#include <vector>

namespace Util
{
	bool Init();

	//Find templated type in vector
	template <typename T>
	static int FindInVector(T toFind, std::vector<T> findIn)
	{
		auto iter = std::find(findIn.begin(), findIn.end(), toFind);

		if (iter != findIn.end())
		{
			return std::distance(findIn.begin(), iter);
		}
		else
		{
			return -1;
		}
	}

	//Check if the your num is within a specific range
	bool CheckNumBetween(int num, int min, int max);
	bool CheckNumBetween(float num, float min, float max);
	bool CheckNumBetween(glm::vec2 num, glm::vec2 min, glm::vec2 max);
	bool CheckNumBetween(glm::vec3 num, glm::vec3 min, glm::vec3 max);
	bool CheckNumBetween(glm::vec4 num, glm::vec4 min, glm::vec4 max);

	//Get random number between two values, while avoiding multiple specific ranges of numbers (or none)
	int GetRandomNumberBetween(int from, int to, std::vector<int> avoidFrom = std::vector<int>(), std::vector<int> avoidTo = std::vector<int>());
	float GetRandomNumberBetween(float from, float to, std::vector<float> avoidFrom = std::vector<float>(), std::vector<float> avoidTo = std::vector<float>());
	glm::vec2 GetRandomNumberBetween(glm::vec2 from, glm::vec2 to, std::vector <glm::vec2> avoidFrom = std::vector <glm::vec2>(), std::vector <glm::vec2> avoidTo = std::vector <glm::vec2>());
	glm::vec3 GetRandomNumberBetween(glm::vec3 from, glm::vec3 to, std::vector <glm::vec3> avoidFrom = std::vector <glm::vec3>(), std::vector <glm::vec3> avoidTo = std::vector <glm::vec3>());
	glm::vec3 GetRandomNumberBetween(glm::vec4 from, glm::vec4 to, std::vector <glm::vec4> avoidFrom = std::vector <glm::vec4>(), std::vector <glm::vec4> avoidTo = std::vector <glm::vec4>());
}