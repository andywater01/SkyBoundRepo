//Just a simple handler for simple initialization stuffs
#include "Utilities/BackendHandler.h"

#include <filesystem>
#include <json.hpp>
#include <fstream>

#include <Texture2D.h>
#include <Texture2DData.h>
#include <MeshBuilder.h>
#include <MeshFactory.h>
#include <NotObjLoader.h>
#include <ObjLoader.h>
#include <MorphLoader.h>
#include <MorphRenderer.h>
#include <VertexTypes.h>
#include <ShaderMaterial.h>
#include <RendererComponent.h>
#include <TextureCubeMap.h>
#include <TextureCubeMapData.h>
#include <Sprite.h>

#include <Timing.h>
#include <GameObjectTag.h>
#include <InputHelpers.h>

#include <IBehaviour.h>
#include <CameraControlBehaviour.h>
#include <FollowPathBehaviour.h>
#include <SimpleMoveBehaviour.h>

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

#include <DirectionalLight.h>
#include <PointLight.h>
#include <UniformBuffer.h>

#include "Sound/AudioEngine.h"
#include "Utilities/PhysicsDrawer.h"



#pragma region Global Variables

int numManifolds;

bool isRotate = true;
bool isLeft = true;
int CoinCount = 0;
bool gotCoin = false;

bool isHit = false;

//Scene number
int RenderGroupBool = 0;
int PlayerHealth = 3;
bool playerJump = false;

bool moveWizard1 = false;
bool moveWizard2 = false;
float wizardMoveTimer = 0.0f;

bool canMoveForward = true;
bool canMoveLeft = true;
bool canMoveBack = true;
bool canMoveRight = true;
bool playerControlLock = false;

bool sortByQuantity = false;
bool sortByLetter = false;
bool sortByColor = false;


int firstFrame = 0;
int lastFrame = 4;
bool pauseGame = false;
bool drawPhysics = false;
bool moveWizard = false;
bool playerAirborne = false;

int menuSelect = 1;

//Interactions with Snowmen
float redSnowmanDistance = 0;
float greenSnowmanDistance = 0;
float blueSnowmanDistance = 0;
float brownSnowmanDistance = 0;
float purpleSnowmanDistance = 0;
float orangeSnowmanDistance = 0;
float yellowSnowmanDistance = 0;
float aquaSnowmanDistance = 0;
float pinkSnowmanDistance = 0;

//Interactions with Snowmen
float bananaDistance = 0;
float greenGrapeDistance = 0;
float purpleGrapeDistance = 0;
float carrotDistance = 0;
float blueDonutDistance = 0;
float appleDistance = 0;

//Sorting Objects IDS
int carrotValue = 2;
int greenGrapeValue = 6;
int appleValue = 3;
int donutValue = 1;
int purpleGrapeValue = 4;
int bananaValue = 5;

bool appleHeld = false;
bool greenGrapeHeld = false;
bool purpleGrapeHeld = false;
bool carrotHeld = false;
bool blueDonutHeld = false;
bool bananaHeld = false;

bool sortingItemHeld = false;
glm::vec3 sortingItemHeldPos = glm::vec3(0);

int carrotUserValue = 2;
int greenGrapeUserValue = 6;
int appleUserValue = 3;
int donutUserValue = 1;
int purpleGrapeUserValue = 4;
int bananaUserValue = 5;

//int sortingValueArray[6] = { carrotValue, greenGrapeValue, appleValue, donutValue, purpleGrapeValue, bananaValue };

//Compare X-values of Objects and then see if they match with real world objects!

int sortingUserValueArray[6] = { carrotUserValue, greenGrapeUserValue, appleUserValue, donutUserValue, purpleGrapeUserValue, bananaUserValue };

bool coinDescend1 = false;
bool coinDescend2 = false;
bool startSortingPuzzle = false;
bool startAftermath = false;
bool startQuiz = false;
float quizTracker = 0;
float interactDistance = 0;
float interactButtonDistance = 0;
float interactSketchyGuy1Distance = 0;
float interactSketchyGuy2Distance = 0;
bool showInteraction = false;
int dialogueSelect = 0;

bool panToObjects = false;

bool puzzleSelect = false;
int puzzleTracker = 0;

float coin2DescendTimer = 0.0f;
float coin2DescendLimit = 5.0f;

bool initScene1 = true;
bool initScene4 = true;

bool destroyedScene1Objects = false;
bool destroyedScene2Objects = false;
bool scene1ActiveBodies = false;
bool scene2ActiveBodies = false;
bool scene3ActiveBodies = false;

bool scene2ShaderInit = false;

int width, height;
int shadowWidth = 4096;
int shadowHeight = 4096;

bool ToggleTextures = true;

// Collision groups & masks
#define BIT(x) (1<<(x))
enum collisiontypes
{
	COL_NOTHING = 0,        //< Collide with nothing
	COL_PLAYER = BIT(1),   //< Collide with player
	COL_PLATFORM = BIT(2),   //< Collide with players
	COL_TRIANGLE = BIT(3),   //< Collide with triangles
	COL_QUAD = BIT(4)    //< Collide with quads
};

int playerCollidesWith = COL_PLAYER | COL_PLATFORM | COL_TRIANGLE | COL_QUAD;
int platformCollidesWith = COL_PLAYER | COL_PLATFORM | COL_TRIANGLE;
int triangleCollidesWith = COL_PLAYER | COL_TRIANGLE | COL_QUAD;
int quadCollidesWith = COL_PLAYER | COL_QUAD | COL_PLATFORM;

#pragma endregion 


#pragma region Player Controls

//void PlayerInput(GameObject& transform, float dt, float speed, btRigidBody* body) {
//	if (glfwGetKey(BackendHandler::window, GLFW_KEY_A) == GLFW_PRESS && canMoveLeft == true && RenderGroupBool != 0) {
//		//transform->MoveLocal(0.0f, 0.0f, -1.0f * dt * speed);
//		//transform.MoveLocalFixed(0.0f, -1.0f * dt * speed, 0.0f);
//		//transform.get<Transform>().SetLocalPosition(transform.get<Transform>().GetLocalPosition() + glm::vec3(0.0f, -1.0f * dt * speed, 0.0f));
//		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 180.0f);
//		body->activate(true);
//		//body->applyForce(btVector3(0.0f, -8000.0f, 0.0f) * dt * speed, btVector3(0.0f, 0.0f, 0.0f));
//		body->applyCentralImpulse(btVector3(0.0f, -80.0f, 0.0f) * dt * speed);
//		//body->setLinearVelocity(btVector3(1.0f, -200.0f, 1.0f) * dt * speed);
//		firstFrame = 0;
//		lastFrame = 4;
//	}
//	if (glfwGetKey(BackendHandler::window, GLFW_KEY_D) == GLFW_PRESS && canMoveRight == true && RenderGroupBool != 0) {
//		//transform.get<Transform>().SetLocalPosition(transform.get<Transform>().GetLocalPosition() + glm::vec3(0.0f, 1.0f * dt * speed, 0.0f));
//		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
//		body->activate(true);
//		//body->applyForce(btVector3(0.0f, 8000.0f, 0.0f) * dt * speed, btVector3(0.0f, 0.0f, 0.0f));
//		body->applyCentralImpulse(btVector3(0.0f, 80.0f, 0.0f) * dt * speed);
//		//body->setLinearVelocity(btVector3(1.0f, 200.0f, 1.0f) * dt * speed);
//		firstFrame = 0;
//		lastFrame = 4;
//		//body->activate(true);
//		//body->setLinearVelocity(btVector3(0, 4, 0));
//		//body->applyForce(btVector3(0, 1000, 0), btVector3(0, 1000, 0));
//		//transform.MoveLocalFixed(0.0f, 1.0f * dt * speed, 0.0f);
//		//transform.SetLocalRotation(90.0f, 0.0f, 102.0f);
//	}
//	if (glfwGetKey(BackendHandler::window, GLFW_KEY_W) == GLFW_PRESS && canMoveForward == true && RenderGroupBool != 0) {
//		//transform->MoveLocal(1.0f * dt * speed, 0.0f, 0.0f);
//		//transform.MoveLocalFixed(-1.0f * dt * speed, 0.0f, 0.0f);
//		//transform.get<Transform>().SetLocalPosition(transform.get<Transform>().GetLocalPosition() + glm::vec3(-1.0f * dt * speed, 0.0f, 0.0f));
//		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
//		body->activate(true);
//		//body->applyForce(btVector3(-8000.0f, 0.0f, 0.0f) * dt * speed, btVector3(0.0f, 0.0f, 0.0f));
//		body->applyCentralImpulse(btVector3(-80.0f, 0.0f, 0.0f) * dt * speed);
//		//body->setLinearVelocity(btVector3(-200.0f, 1.0f, 1.0f) * dt * speed);
//		firstFrame = 0;
//		lastFrame = 4;
//		//transform.SetLocalRotation(90.0f, 0.0f, 192.0f);
//		//camera->SetPosition(camera->GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f) * dt);
//	}
//	if (glfwGetKey(BackendHandler::window, GLFW_KEY_S) == GLFW_PRESS && canMoveBack == true && RenderGroupBool != 0) {
//		//transform->MoveLocal(-1.0f * dt * speed, 0.0f, 0.0f);
//		//transform.MoveLocalFixed(1.0f * dt * speed, 0.0f, 0.0f);
//		//transform.get<Transform>().SetLocalPosition(transform.get<Transform>().GetLocalPosition() + glm::vec3(1.0f * dt * speed, 0.0f, 0.0f));
//		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 270.0f);
//		body->activate(true);
//		//body->applyForce(btVector3(8000.0f, 0.0f, 0.0f) * dt * speed, btVector3(0.0f, 0.0f, 0.0f));
//		body->applyCentralImpulse(btVector3(80.0f, 0.0f, 0.0f) * dt * speed);
//		//body->setLinearVelocity(btVector3(200.0f, 1.0f, 1.0f) * dt * speed);
//		firstFrame = 0;
//		lastFrame = 4;
//		//transform.SetLocalRotation(90.0f, 0.0f, 12.0f);
//		//camera->SetPosition(camera->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f) * dt);
//	}
//	if (glfwGetKey(BackendHandler::window, GLFW_KEY_SPACE) == GLFW_PRESS && RenderGroupBool != 0) {
//		//transform.MoveLocal(0.0f, 0.0f, 1.0f * dt);
//	}
//	if (glfwGetKey(BackendHandler::window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && RenderGroupBool != 0) {
//		//transform.MoveLocal(0.0f, 0.0f, -1.0f * dt);
//	}
//	else
//	{
//		firstFrame = 5;
//		lastFrame = 7;
//	}
//
//}


void PlayerInput(GameObject& transform, float dt, float speed, btRigidBody* body) {
	if (glfwGetKey(BackendHandler::window, GLFW_KEY_A) == GLFW_PRESS && canMoveLeft == true && RenderGroupBool != 0) {
		//transform->MoveLocal(0.0f, 0.0f, -1.0f * dt * speed);
		//transform.MoveLocalFixed(0.0f, -1.0f * dt * speed, 0.0f);
		//transform.get<Transform>().SetLocalPosition(transform.get<Transform>().GetLocalPosition() + glm::vec3(0.0f, -1.0f * dt * speed, 0.0f));
		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 180.0f);
		body->activate(true);
		//body->applyForce(btVector3(0.0f, -8000.0f, 0.0f) * dt * speed, btVector3(0.0f, 0.0f, 0.0f));
		body->applyCentralImpulse(btVector3(0.0f, -80.0f, 0.0f) * dt * speed);
		//body->setLinearVelocity(btVector3(1.0f, -200.0f, 1.0f) * dt * speed);
		firstFrame = 0;
		lastFrame = 4;
	}
	else if (glfwGetKey(BackendHandler::window, GLFW_KEY_D) == GLFW_PRESS && canMoveRight == true && RenderGroupBool != 0) {
		//transform.get<Transform>().SetLocalPosition(transform.get<Transform>().GetLocalPosition() + glm::vec3(0.0f, 1.0f * dt * speed, 0.0f));
		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
		body->activate(true);
		//body->applyForce(btVector3(0.0f, 8000.0f, 0.0f) * dt * speed, btVector3(0.0f, 0.0f, 0.0f));
		body->applyCentralImpulse(btVector3(0.0f, 80.0f, 0.0f) * dt * speed);
		//body->setLinearVelocity(btVector3(1.0f, 200.0f, 1.0f) * dt * speed);
		firstFrame = 0;
		lastFrame = 4;
		//body->activate(true);
		//body->setLinearVelocity(btVector3(0, 4, 0));
		//body->applyForce(btVector3(0, 1000, 0), btVector3(0, 1000, 0));
		//transform.MoveLocalFixed(0.0f, 1.0f * dt * speed, 0.0f);
		//transform.SetLocalRotation(90.0f, 0.0f, 102.0f);
	}

	else if (glfwGetKey(BackendHandler::window, GLFW_KEY_W) == GLFW_PRESS && canMoveForward == true && RenderGroupBool != 0) {
		//transform->MoveLocal(1.0f * dt * speed, 0.0f, 0.0f);
		//transform.MoveLocalFixed(-1.0f * dt * speed, 0.0f, 0.0f);
		//transform.get<Transform>().SetLocalPosition(transform.get<Transform>().GetLocalPosition() + glm::vec3(-1.0f * dt * speed, 0.0f, 0.0f));
		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
		body->activate(true);
		//body->applyForce(btVector3(-8000.0f, 0.0f, 0.0f) * dt * speed, btVector3(0.0f, 0.0f, 0.0f));
		body->applyCentralImpulse(btVector3(-80.0f, 0.0f, 0.0f) * dt * speed);
		//body->setLinearVelocity(btVector3(-200.0f, 1.0f, 1.0f) * dt * speed);
		firstFrame = 0;
		lastFrame = 4;
		//transform.SetLocalRotation(90.0f, 0.0f, 192.0f);
		//camera->SetPosition(camera->GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f) * dt);
	}
	else if (glfwGetKey(BackendHandler::window, GLFW_KEY_S) == GLFW_PRESS && canMoveBack == true && RenderGroupBool != 0) {
		//transform->MoveLocal(-1.0f * dt * speed, 0.0f, 0.0f);
		//transform.MoveLocalFixed(1.0f * dt * speed, 0.0f, 0.0f);
		//transform.get<Transform>().SetLocalPosition(transform.get<Transform>().GetLocalPosition() + glm::vec3(1.0f * dt * speed, 0.0f, 0.0f));
		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 270.0f);
		body->activate(true);
		//body->applyForce(btVector3(8000.0f, 0.0f, 0.0f) * dt * speed, btVector3(0.0f, 0.0f, 0.0f));
		body->applyCentralImpulse(btVector3(80.0f, 0.0f, 0.0f) * dt * speed);
		//body->setLinearVelocity(btVector3(200.0f, 1.0f, 1.0f) * dt * speed);
		firstFrame = 0;
		lastFrame = 4;
		//transform.SetLocalRotation(90.0f, 0.0f, 12.0f);
		//camera->SetPosition(camera->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f) * dt);
	}
	if (glfwGetKey(BackendHandler::window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(BackendHandler::window, GLFW_KEY_A) == GLFW_PRESS)
	{
		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 135.0f);
		body->activate(true);
		//body->applyForce(btVector3(8000.0f, 0.0f, 0.0f) * dt * speed, btVector3(0.0f, 0.0f, 0.0f));
		body->applyCentralImpulse(btVector3(-20.0f, -20.0f, 0.0f) * dt * speed);

		firstFrame = 0;
		lastFrame = 4;
	}

	else if (glfwGetKey(BackendHandler::window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(BackendHandler::window, GLFW_KEY_D) == GLFW_PRESS)
	{
		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 45.0f);
		body->activate(true);
		//body->applyForce(btVector3(8000.0f, 0.0f, 0.0f) * dt * speed, btVector3(0.0f, 0.0f, 0.0f));
		body->applyCentralImpulse(btVector3(-20.0f, 20.0f, 0.0f) * dt * speed);
		//body->setLinearVelocity(btVector3(200.0f, 1.0f, 1.0f) * dt * speed);
		firstFrame = 0;
		lastFrame = 4;
	}

	else if (glfwGetKey(BackendHandler::window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(BackendHandler::window, GLFW_KEY_A) == GLFW_PRESS)
	{
		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 215.0f);
		body->activate(true);
		//body->applyForce(btVector3(8000.0f, 0.0f, 0.0f) * dt * speed, btVector3(0.0f, 0.0f, 0.0f));
		body->applyCentralImpulse(btVector3(20.0f, -20.0f, 0.0f) * dt * speed);
		//body->setLinearVelocity(btVector3(200.0f, 1.0f, 1.0f) * dt * speed);
		firstFrame = 0;
		lastFrame = 4;
	}

	else if (glfwGetKey(BackendHandler::window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(BackendHandler::window, GLFW_KEY_D) == GLFW_PRESS)
	{
		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 305.0f);
		body->activate(true);
		//body->applyForce(btVector3(8000.0f, 0.0f, 0.0f) * dt * speed, btVector3(0.0f, 0.0f, 0.0f));
		body->applyCentralImpulse(btVector3(20.0f, 20.0f, 0.0f) * dt * speed);
		//body->setLinearVelocity(btVector3(200.0f, 1.0f, 1.0f) * dt * speed);
		firstFrame = 0;
		lastFrame = 4;
	}


	if (glfwGetKey(BackendHandler::window, GLFW_KEY_SPACE) == GLFW_PRESS && RenderGroupBool != 0) {
		//transform.MoveLocal(0.0f, 0.0f, 1.0f * dt);
	}
	if (glfwGetKey(BackendHandler::window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && RenderGroupBool != 0) {
		//transform.MoveLocal(0.0f, 0.0f, -1.0f * dt);
	}
	else
	{
		firstFrame = 5;
		lastFrame = 7;
	}

}

#pragma endregion


#pragma region Print Position

void printObjectPosition(GameObject& transform)
{
	float x, y, z;
	x = transform.get<Transform>().GetLocalPosition().x;
	y = transform.get<Transform>().GetLocalPosition().y;
	z = transform.get<Transform>().GetLocalPosition().z;

	std::cout << "\nObject's X: " << x << std::endl;
	std::cout << "Object's Y: " << y << std::endl;
	std::cout << "Object's Z: " << z << std::endl;
}

#pragma endregion


#pragma region LERP and Catmull

template<typename T>
T LERP(const T& p0, const T& p1, float t)
{
	return (1.0f - t) * p0 + t * p1;
}


//Templated Catmull-Rom function.
//(This will work for any type that supports addition and scalar multiplication.)
template<typename T>
T Catmull(const T& p0, const T& p1, const T& p2, const T& p3, float t)
{
	//TODO: Implement Catmull-Rom interpolation.
	return 0.5f * (2.f * p1 + t * (-p0 + p2)
		+ t * t * (2.f * p0 - 5.f * p1 + 4.f * p2 - p3)
		+ t * t * t * (-p0 + 3.f * p1 - 3.f * p2 + p3));
}

#pragma endregion


#pragma region Bubble Sorting and Swap Function

template<class T>
void swap(T* firstValue, T* secondValue)
{
	T temp = *firstValue;

	*firstValue = *secondValue;

	*secondValue = temp;
}


void swapPositions(GameObject& obj1, GameObject& obj2)
{
	glm::vec3 temp = obj1.get<Transform>().GetLocalPosition();

	obj1.get<Transform>().SetLocalPosition(obj2.get<Transform>().GetLocalPosition());

	obj2.get<Transform>().SetLocalPosition(temp);
}

void swapObjects(GameObject& obj1, GameObject& obj2)
{
	GameObject temp = obj1;

	obj1 = obj2;

	obj2 = temp;
}

void swapPos(Transform& obj1, Transform& obj2)
{
	glm::vec3 temp = obj1.GetLocalPosition();

	obj1.SetLocalPosition(obj2.GetLocalPosition());

	obj2.SetLocalPosition(temp);
}


//
//template <class T>
//void bubbleSort(T data[], int n)
//{
//	for (int i = 0; i < n - 1; i++)
//	{
//		//std::cout << "\n I: " << i << std::endl;
//
//		for (int j = n - 1; j > i; --j)
//		{
//			//std::cout << "\n J: " << j << std::endl;
//			if (data[j] < data[j - 1])
//				swapPositions(&data[j], &data[j - 1]);
//		}
//	}
//}



void bubbleSortQuantity(GameObject data[], int n, float dt)
{
	for (int i = 0; i < n - 1; i++)
	{
		//std::cout << "\n I: " << i << std::endl;

		for (int j = n - 1; j > i; --j)
		{
			//std::cout << "\n J: " << j << std::endl;
			if (data[j].get<SortingInfo>().GetQuantity() < data[j - 1].get<SortingInfo>().GetQuantity())
			{
				swapPositions(data[j], data[j - 1]);
				swapObjects(data[j], data[j - 1]);
			}

			
			std::cout << "\nSorted Array Step " << i + 1 << ": { ";

			for (int k = 0; k < n; k++)
			{
				std::cout << data[k].get<SortingInfo>().GetQuantity() << " ";
			}
			std::cout << "}" << std::endl;
		}
	}
}

void bubbleSortLetter(GameObject data[], int n, float dt)
{
	for (int i = 0; i < n - 1; i++)
	{
		//std::cout << "\n I: " << i << std::endl;

		for (int j = n - 1; j > i; --j)
		{
			//std::cout << "\n J: " << j << std::endl;
			if (data[j].get<SortingInfo>().GetAlphabet() < data[j - 1].get<SortingInfo>().GetAlphabet())
			{
				swapPositions(data[j], data[j - 1]);
				swapObjects(data[j], data[j - 1]);
			}


			std::cout << "\nSorted Array Step " << i + 1 << ": { ";

			for (int k = 0; k < n; k++)
			{
				std::cout << data[k].get<SortingInfo>().GetQuantity() << " ";
			}
			std::cout << "}" << std::endl;
		}
	}
}

void bubbleSortColor(GameObject data[], int n, float dt)
{
	for (int i = 0; i < n - 1; i++)
	{
		//std::cout << "\n I: " << i << std::endl;

		for (int j = n - 1; j > i; --j)
		{
			//std::cout << "\n J: " << j << std::endl;
			if (data[j].get<SortingInfo>().GetColor() < data[j - 1].get<SortingInfo>().GetColor())
			{
				swapPositions(data[j], data[j - 1]);
				swapObjects(data[j], data[j - 1]);
			}


			std::cout << "\nSorted Array Step " << i + 1 << ": { ";

			for (int k = 0; k < n; k++)
			{
				std::cout << data[k].get<SortingInfo>().GetQuantity() << " ";
			}
			std::cout << "}" << std::endl;
		}
	}
}


#pragma endregion


#pragma region Phantom Catmull Function and Variables

//Catmull-Rom Variables
float m_segmentTravelTime = 1.0f;
float m_segmentTimer;
size_t m_segmentIndex;


std::vector<glm::vec3> phantomWaypoints{ glm::vec3(-36.0f, 12.0f, -1.0f),
										glm::vec3(-40.0f, 0.1f, -1.0f),
										glm::vec3(-36.0f, -12.0f, -1.0f),
										glm::vec3(-33.0f, -6.8f, -1.0f),
										glm::vec3(-32.7f, 0.38f, -1.0f),
										glm::vec3(-33.8f, 7.01f, -1.0f) };


void UpdateCatmull(std::vector<glm::vec3> points, GameObject object, float deltaTime)
{
	if (points.size() == 0 || m_segmentTravelTime == 0)
		return;

	m_segmentTimer += deltaTime;


	if (m_segmentIndex == 2)
	{
		object.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
	}

	if (m_segmentIndex == 5)
	{
		object.get<Transform>().SetLocalRotation(90.0f, 0.0f, 180.0f);
	}



	while (m_segmentTimer > m_segmentTravelTime)
	{
		m_segmentTimer -= m_segmentTravelTime;

		m_segmentIndex += 1;

		if (m_segmentIndex >= points.size())
			m_segmentIndex = 0;
	}

	float t = m_segmentTimer / m_segmentTravelTime;

	if (points.size() < 4)
	{
		object.get<Transform>().SetLocalPosition(points[0]);
		return;
	}

	size_t p0_ind, p1_ind, p2_ind, p3_ind;
	glm::vec3 p0, p1, p2, p3;

	//For Catmull, the path segment between p1 and p2
	//Our segment index is gonna be p1
	p1_ind = m_segmentIndex;

	p0_ind = (p1_ind == 0) ? points.size() - 1 : p1_ind - 1;

	p2_ind = (p1_ind + 1) % points.size();

	p3_ind = (p2_ind + 1) % points.size();

	//Setting the vec3s
	p0 = points[p0_ind];
	p1 = points[p1_ind];
	p2 = points[p2_ind];
	p3 = points[p3_ind];

	object.get<Transform>().SetLocalPosition(Catmull(p0, p1, p2, p3, t));
}

#pragma endregion


#pragma region Distance and Distance Checker Functions

float Distance(GameObject obj1, GameObject obj2)
{
	float product = glm::pow((obj1.get<Transform>().GetLocalPosition().x - obj2.get<Transform>().GetLocalPosition().x), 2) +
		glm::pow((obj1.get<Transform>().GetLocalPosition().y - obj2.get<Transform>().GetLocalPosition().y), 2) +
		glm::pow((obj1.get<Transform>().GetLocalPosition().z - obj2.get<Transform>().GetLocalPosition().z), 2);

	float distance = glm::sqrt(product);

	return distance;
}



bool DistanceCheck(GameObject obj1, GameObject obj2)
{
	bool check;

	float distance = Distance(obj1, obj2);

	if (distance <= 18)
	{
		check = false;
	}
	else
	{
		check = true;
	}

	return check;

}

#pragma endregion


#pragma region GetDistance(Coin Function) and MoveWizard Function

void GetDistance(GameObject player, GameObject object, glm::vec3 distance)
{
	distance = player.get<Transform>().GetLocalPosition() - object.get<Transform>().GetLocalPosition();

	if (distance.x <= 2.0f && distance.y <= 2.0f && glfwGetKey(BackendHandler::window, GLFW_KEY_E) == GLFW_PRESS && gotCoin == false)
	{
		CoinCount = 1;
		object.get<Transform>().SetLocalPosition(100.0f, 100.0f, 100.0f);
		gotCoin = true;
	}
}

void MoveWizard(GameObject player, GameObject wizard, btRigidBody* wizardBody, btTransform wizardTransform, glm::vec3 distance2)
{
	distance2 = player.get<Transform>().GetLocalPosition() - wizard.get<Transform>().GetLocalPosition();

	if (distance2.x <= 4.0f && distance2.y <= 4.0f && CoinCount >= 1 && glfwGetKey(BackendHandler::window, GLFW_KEY_E) == GLFW_PRESS)
	{
		moveWizard = true;
		CoinCount = 0;
		//wizard.get<Transform>().SetLocalPosition(wizard.get<Transform>().GetLocalPosition() + glm::vec3(-33.0f, 5.0f, 0.0f));
		wizardTransform.setOrigin(btVector3(-47.5f, 5.0f, -2.5f));
		wizardBody->setWorldTransform(wizardTransform);

		btCollisionShape* wizardShape = new btBoxShape(btVector3(1.1f, 1.1f, 2.4f));
		wizardBody->setCollisionShape(wizardShape);
	}
}

#pragma endregion


#pragma region CollisionResolver and InputChecker Functions

void CollisionResolver()
{
	canMoveForward = true;
	canMoveLeft = true;
	canMoveBack = true;
	canMoveRight = true;
}

void inputChecker()
{
	if (canMoveForward)
		std::cout << "Can move forwards" << std::endl;

	if (canMoveLeft)
		std::cout << "Can move left" << std::endl;

	if (canMoveBack)
		std::cout << "Can move back" << std::endl;

	if (canMoveRight)
		std::cout << "Can move right" << std::endl;
}

#pragma endregion


#pragma region CheckCollision and CheckPhantomCollision Functions

void CheckCollision(GameObject player, GameObject otherObject, float xRangePos, float xRangeNeg, float yRangePos, float yRangeNeg)
{
	//Forward
	if (player.get<Transform>().GetLocalPosition().x - xRangePos <= otherObject.get<Transform>().GetLocalPosition().x + xRangeNeg &&
		player.get<Transform>().GetLocalPosition().x >= otherObject.get<Transform>().GetLocalPosition().x - xRangeNeg &&
		player.get<Transform>().GetLocalPosition().y <= otherObject.get<Transform>().GetLocalPosition().y + yRangePos &&
		player.get<Transform>().GetLocalPosition().y >= otherObject.get<Transform>().GetLocalPosition().y - yRangeNeg)
	{
		std::cout << "Can't move forwards" << std::endl;
		canMoveForward = false;
	}
	else
	{
		//std::cout << "Exited forward block" << std::endl;
		canMoveForward = true;

	}

	//Backward
	if (player.get<Transform>().GetLocalPosition().x + xRangePos >= otherObject.get<Transform>().GetLocalPosition().x - xRangePos &&
		player.get<Transform>().GetLocalPosition().x <= otherObject.get<Transform>().GetLocalPosition().x + xRangeNeg &&
		player.get<Transform>().GetLocalPosition().y <= otherObject.get<Transform>().GetLocalPosition().y + yRangePos &&
		player.get<Transform>().GetLocalPosition().y >= otherObject.get<Transform>().GetLocalPosition().y - yRangeNeg)
	{
		canMoveBack = false;
		std::cout << "Can't move backwards" << std::endl;
	}
	else
	{
		//std::cout << "Exited backwards block" << std::endl;
		canMoveBack = true;

	}

	//Left
	if (player.get<Transform>().GetLocalPosition().y - yRangeNeg <= otherObject.get<Transform>().GetLocalPosition().y + yRangePos &&
		player.get<Transform>().GetLocalPosition().y >= otherObject.get<Transform>().GetLocalPosition().y - yRangePos &&
		player.get<Transform>().GetLocalPosition().x <= otherObject.get<Transform>().GetLocalPosition().x + xRangeNeg &&
		player.get<Transform>().GetLocalPosition().x >= otherObject.get<Transform>().GetLocalPosition().x - xRangePos)
	{
		canMoveLeft = false;
		std::cout << "Can't move Left" << std::endl;
	}
	else
	{
		//std::cout << "Exited left block" << std::endl;
		canMoveLeft = true;

	}

	//Right
	if (player.get<Transform>().GetLocalPosition().y + yRangeNeg >= otherObject.get<Transform>().GetLocalPosition().y - yRangePos &&
		player.get<Transform>().GetLocalPosition().y <= otherObject.get<Transform>().GetLocalPosition().y + yRangePos &&
		player.get<Transform>().GetLocalPosition().x <= otherObject.get<Transform>().GetLocalPosition().x + xRangeNeg &&
		player.get<Transform>().GetLocalPosition().x >= otherObject.get<Transform>().GetLocalPosition().x - xRangePos)
	{
		canMoveRight = false;
		std::cout << "Can't move right" << std::endl;
	}
	else
	{
		//std::cout << "Exited right block" << std::endl;
		canMoveRight = true;

	}

}



void CheckPhantomCollision(GameObject player, btRigidBody *playerBody, btTransform playerTransform, GameObject other, float xRangePos, float xRangeNeg, float yRangePos, float yRangeNeg)
{
	//Forward
	if (player.get<Transform>().GetLocalPosition().x - xRangePos <= other.get<Transform>().GetLocalPosition().x + xRangeNeg &&
		player.get<Transform>().GetLocalPosition().x >= other.get<Transform>().GetLocalPosition().x - xRangeNeg &&
		player.get<Transform>().GetLocalPosition().y <= other.get<Transform>().GetLocalPosition().y + yRangePos &&
		player.get<Transform>().GetLocalPosition().y >= other.get<Transform>().GetLocalPosition().y - yRangeNeg)
	{
		isHit = true;
		PlayerHealth--;
		playerTransform.setOrigin(btVector3(0.0f, 0.0f, 5.0f));
		playerBody->setWorldTransform(playerTransform);
	}

	//Backward
	if (player.get<Transform>().GetLocalPosition().x + xRangePos >= other.get<Transform>().GetLocalPosition().x - xRangePos &&
		player.get<Transform>().GetLocalPosition().x <= other.get<Transform>().GetLocalPosition().x + xRangeNeg &&
		player.get<Transform>().GetLocalPosition().y <= other.get<Transform>().GetLocalPosition().y + yRangePos &&
		player.get<Transform>().GetLocalPosition().y >= other.get<Transform>().GetLocalPosition().y - yRangeNeg)
	{
		isHit = true;
		PlayerHealth--;
		playerTransform.setOrigin(btVector3(0.0f, 0.0f, 5.0f));
		playerBody->setWorldTransform(playerTransform);
	}


	//Left
	if (player.get<Transform>().GetLocalPosition().y - yRangeNeg <= other.get<Transform>().GetLocalPosition().y + yRangePos &&
		player.get<Transform>().GetLocalPosition().y >= other.get<Transform>().GetLocalPosition().y - yRangePos &&
		player.get<Transform>().GetLocalPosition().x <= other.get<Transform>().GetLocalPosition().x + xRangeNeg &&
		player.get<Transform>().GetLocalPosition().x >= other.get<Transform>().GetLocalPosition().x - xRangePos)
	{
		isHit = true;
		PlayerHealth--;
		playerTransform.setOrigin(btVector3(0.0f, 0.0f, 5.0f));
		playerBody->setWorldTransform(playerTransform);
	}


	if (player.get<Transform>().GetLocalPosition().y + yRangeNeg >= other.get<Transform>().GetLocalPosition().y - yRangePos &&
		player.get<Transform>().GetLocalPosition().y <= other.get<Transform>().GetLocalPosition().y + yRangePos &&
		player.get<Transform>().GetLocalPosition().x <= other.get<Transform>().GetLocalPosition().x + xRangeNeg &&
		player.get<Transform>().GetLocalPosition().x >= other.get<Transform>().GetLocalPosition().x - xRangePos)
	{
		isHit = true;
		PlayerHealth--;
		playerTransform.setOrigin(btVector3(0.0f, 0.0f, 5.0f));
		playerBody->setWorldTransform(playerTransform);;
	}


}


void CheckCoinCollision(GameObject player, GameObject other, float xRangePos, float xRangeNeg, float yRangePos, float yRangeNeg)
{
	//Forward
	if (player.get<Transform>().GetLocalPosition().x - xRangePos <= other.get<Transform>().GetLocalPosition().x + xRangeNeg &&
		player.get<Transform>().GetLocalPosition().x >= other.get<Transform>().GetLocalPosition().x - xRangeNeg &&
		player.get<Transform>().GetLocalPosition().y <= other.get<Transform>().GetLocalPosition().y + yRangePos &&
		player.get<Transform>().GetLocalPosition().y >= other.get<Transform>().GetLocalPosition().y - yRangeNeg)
	{
		other.get<Transform>().SetLocalPosition(other.get<Transform>().GetLocalPosition().x,
												other.get<Transform>().GetLocalPosition().y,
												100.0f);
		CoinCount++;
	}

	//Backward
	if (player.get<Transform>().GetLocalPosition().x + xRangePos >= other.get<Transform>().GetLocalPosition().x - xRangePos &&
		player.get<Transform>().GetLocalPosition().x <= other.get<Transform>().GetLocalPosition().x + xRangeNeg &&
		player.get<Transform>().GetLocalPosition().y <= other.get<Transform>().GetLocalPosition().y + yRangePos &&
		player.get<Transform>().GetLocalPosition().y >= other.get<Transform>().GetLocalPosition().y - yRangeNeg)
	{
		other.get<Transform>().SetLocalPosition(other.get<Transform>().GetLocalPosition().x,
												other.get<Transform>().GetLocalPosition().y,
												100.0f);
		CoinCount++;
	}


	//Left
	if (player.get<Transform>().GetLocalPosition().y - yRangeNeg <= other.get<Transform>().GetLocalPosition().y + yRangePos &&
		player.get<Transform>().GetLocalPosition().y >= other.get<Transform>().GetLocalPosition().y - yRangePos &&
		player.get<Transform>().GetLocalPosition().x <= other.get<Transform>().GetLocalPosition().x + xRangeNeg &&
		player.get<Transform>().GetLocalPosition().x >= other.get<Transform>().GetLocalPosition().x - xRangePos)
	{
		other.get<Transform>().SetLocalPosition(other.get<Transform>().GetLocalPosition().x,
												other.get<Transform>().GetLocalPosition().y,
												100.0f);
		CoinCount++;
	}


	if (player.get<Transform>().GetLocalPosition().y + yRangeNeg >= other.get<Transform>().GetLocalPosition().y - yRangePos &&
		player.get<Transform>().GetLocalPosition().y <= other.get<Transform>().GetLocalPosition().y + yRangePos &&
		player.get<Transform>().GetLocalPosition().x <= other.get<Transform>().GetLocalPosition().x + xRangeNeg &&
		player.get<Transform>().GetLocalPosition().x >= other.get<Transform>().GetLocalPosition().x - xRangePos)
	{
		other.get<Transform>().SetLocalPosition(other.get<Transform>().GetLocalPosition().x,
												other.get<Transform>().GetLocalPosition().y,
												100.0f);
		CoinCount++;
	}


}

#pragma endregion


#pragma region Check Player Position Function

void checkPosition(GameObject object)
{
	std::cout << "Player X: " << object.get<Transform>().GetLocalPosition().x << std::endl;
	std::cout << "Player Y: " << object.get<Transform>().GetLocalPosition().y << std::endl;
	std::cout << "Player Z: " << object.get<Transform>().GetLocalPosition().z << std::endl;
}

#pragma endregion


#pragma region Link Render Transform with Physics Body Function

//Links object's render transform with the object's physics body
void LinkBody(GameObject object, btRigidBody *body)
{
	glm::vec3 linkedBody = glm::vec3(body->getCenterOfMassTransform().getOrigin().getX(),
									 body->getCenterOfMassTransform().getOrigin().getY(),
									 body->getCenterOfMassTransform().getOrigin().getZ());

	object.get<Transform>().SetLocalPosition(linkedBody);

}

void LinkBody(GameObject object, btRigidBody* body, float adjustX, float adjustY, float adjustZ)
{
	glm::vec3 linkedBody = glm::vec3(body->getCenterOfMassTransform().getOrigin().getX() + adjustX,
		body->getCenterOfMassTransform().getOrigin().getY() + adjustY,
		body->getCenterOfMassTransform().getOrigin().getZ() + adjustZ);

	object.get<Transform>().SetLocalPosition(linkedBody);

}

#pragma endregion


#pragma region Physics Callback

//Physics callback function
void myTickCallback(btDynamicsWorld* world, btScalar timeStep) {
	numManifolds = world->getDispatcher()->getNumManifolds();
	//printf("numManifolds = %d\n", numManifolds);

	if (numManifolds == 0)
	{
		//world->removeRigidBody(rigidBody);
		playerAirborne = true;
	}
	else
	{
		playerAirborne = false;
	}
}

#pragma endregion


#pragma region Platform Collisions (Level 3)

bool isPlatform1Collision = false;
bool isPlatform2Collision = false;

#pragma endregion


#pragma region Pan Camera

float posTimeLimit = 5.0f;
float posTimer = 0.0f;
bool startPanCamera = false;
bool resetPanCamera = false;

void panCamera(GameObject& camera, GameObject& orthoCamera, GameObject& target, float dt, float offsetX, float offsetY, float offsetZ)
{
	glm::vec3 startPoint = camera.get<Transform>().GetLocalPosition();
	glm::vec3 endPoint = target.get<Transform>().GetLocalPosition() + glm::vec3(offsetX, offsetY, offsetZ);

	//Increment timers.
	posTimer += dt;

	//Resetting timers and adjusting back/forth booleans.
	if (posTimer >= posTimeLimit)
	{
		posTimer = 0.0f;
		playerControlLock = true;
	}

	//Calculate t.
	float tPos = posTimer / posTimeLimit;

	//LERPing the camera to the new position
	camera.get<Transform>().SetLocalPosition(LERP(startPoint, endPoint, tPos));
	orthoCamera.get<Transform>().SetLocalPosition(LERP(startPoint, endPoint, tPos));

	

}

void panCamera(GameObject& camera, GameObject& orthoCamera, glm::vec3 target, float dt, float offsetX, float offsetY, float offsetZ)
{
	glm::vec3 startPoint = camera.get<Transform>().GetLocalPosition();
	glm::vec3 endPoint = target + glm::vec3(offsetX, offsetY, offsetZ);

	//Increment timers.
	posTimer += dt;

	//Resetting timers and adjusting back/forth booleans.
	if (posTimer >= posTimeLimit)
	{
		posTimer = 0.0f;
		playerControlLock = true;
	}

	//Calculate t.
	float tPos = posTimer / posTimeLimit;

	//LERPing the camera to the new position
	camera.get<Transform>().SetLocalPosition(LERP(startPoint, endPoint, tPos));
	orthoCamera.get<Transform>().SetLocalPosition(LERP(startPoint, endPoint, tPos));



}

#pragma endregion

void clearSortingItems()
{
	appleHeld = false;
	bananaHeld = false;
	greenGrapeHeld = false;
	purpleGrapeHeld = false;
	carrotHeld = false;
	blueDonutHeld = false;
}


//Callback for specific object ids
struct CustomFilterCallback : public btOverlapFilterCallback
{
	// return true when pairs need collision
	virtual bool	needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
	{
		bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
		collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);

		//std::cout << proxy0->getUid() << "  " << proxy1->getUid() << std::endl;

		if (proxy0->getUid() == 13)
		{
			isPlatform1Collision = true;
		}

		if (proxy0->getUid() == 14)
		{
			isPlatform2Collision = true;
		}

		//add some additional logic here that modified 'collides'
		return collides;
	}
};


#pragma region Check Airborne

void checkAirborne(GameObject player)
{
	/*if (player.get<Transform>().GetLocalPosition().z >= 0.14f)
	{
		playerAirborne = true;
	}
	else
	{
		playerAirborne = false;
	}*/
}

#pragma endregion


#pragma region Sound Initialization

//---------------------------------------------------------------------------------
// Inplemented in Game.cpp
//---------------------------------------------------------------------------------
extern void Init();
extern void Update(float deltaTime);
extern void Render();
extern void Shutdown();

#pragma endregion


int main() {
	int frameIx = 0;
	float fpsBuffer[128];
	float minFps, maxFps, avgFps;
	int selectedVao = 0; // select cube by default
	std::vector<GameObject> controllables;



	#pragma region Sound Definitions

	//Game.cpp init//
	Init();

	AudioEngine& engine = AudioEngine::Instance();
	AudioEvent& BG = engine.GetEvent("music");
	AudioEvent& popSound = engine.GetEvent("popSound");
	AudioEvent& loon = engine.GetEvent("loonSound");
	AudioEvent& footsteps = engine.GetEvent("footstepsSound");

	AudioEvent& menu = engine.GetEvent("menuMusic");
	AudioEvent& Islandmusic2 = engine.GetEvent("music2");
	AudioEvent& Islandmusic3 = engine.GetEvent("music3");


	AudioEvent& Prelude1 = engine.GetEvent("Narration1");
	AudioEvent& Prelude2 = engine.GetEvent("Narration2");
	AudioEvent& Prelude3 = engine.GetEvent("Narration3");

	#pragma endregion



	BackendHandler::InitAll();

	// Let OpenGL know that we want debug output, and route it to our handler function
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(BackendHandler::GlDebugMessage, nullptr);

	// Enable texturing
	glEnable(GL_TEXTURE_2D);

	// Push another scope so most memory should be freed *before* we exit the app
	{
		#pragma region Physics World Variables

		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

		btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

		dynamicsWorld->setGravity(btVector3(0.f, 0.f, -15.f));
		

		//keep track of the shapes, we release memory at exit.
		//make sure to re-use collision shapes among rigid bodies whenever possible!
		btAlignedObjectArray<btCollisionShape*> collisionShapes;

		#pragma endregion

		
		#pragma region Physics Plane

		//Plane
		float planeHeight = -15.0f;
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(0, 0, planeHeight));
		btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 0, 1), 0);
		btMotionState* motion = new btDefaultMotionState(t);
		btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
		btRigidBody* planeBody = new btRigidBody(info);
		dynamicsWorld->addRigidBody(planeBody);

		#pragma endregion

		
		#pragma region Camera Physics Variables

		//Camera Physics
		btCollisionShape* cameraShape = new btBoxShape(btVector3(1.f, 1.f, 1.f));

		btTransform cameraTransform;

		btScalar cameraMass(2.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool iscameraDynamic = (cameraMass != 0.f);

		btVector3 localcameraInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* cameraMotionState;
		btRigidBody* cameraBody;

		#pragma endregion


		#pragma region LERPING Variables

		//Phantom Position Lerp
		glm::vec3 endPos = glm::vec3(-35.0f, -9.5f, -1.0f);
		glm::vec3 startPos = glm::vec3(-35.0f, 9.5f, -1.0f);

		glm::vec3 endPos2 = glm::vec3(-30.0f, 9.5f, -1.0f);
		glm::vec3 startPos2 = glm::vec3(-30.0f, -9.5f, -1.0f);

		btVector3 firePlatform1Pos1 = btVector3(-25.0f, -5.5f, 0.5f);
		btVector3 firePlatform1Pos2 = btVector3(-25.0f, 5.5f, 0.5f);

		//btVector3 firePlatform1Pos1 = btVector3(-25.0f, 0.0f, -2.0f);
		//btVector3 firePlatform1Pos2 = btVector3(-25.0f, 0.0f, 8.5f);

		//btVector3 firePlatform2Pos1 = btVector3(-30.0f, -5.5f, 3.0f);
		//btVector3 firePlatform2Pos2 = btVector3(-30.0f, 5.5f, 3.0f);

		btVector3 firePlatform2Pos1 = btVector3(-30.0f, -5.5f, 3.0f);
		btVector3 firePlatform2Pos2 = btVector3(-30.0f, 5.5f, 3.0f);

		float PhantomTimer = 0.0f;
		float PhantomTimer2 = 0.0f;
		float PhantomLevel2Timer = 0.0f;
		float JumpTimer = 0.0f;
		float firePlatform1Timer = 0.0f;
		float firePlatform2Timer = 0.0f;

		float PhantomTimeLimit = 1.0f;
		float PhantomTimeLimit2 = 2.0f;
		float JumpTimeLimit = 0.15f;
		float firePlatform1Limit = 5.0f;
		float firePlatform2Limit = 5.0f;

		bool PhantomMove = true;
		bool PhantomMove2 = true;

		bool firePlatform1Move = true;
		bool firePlatform2Move = true;



		glm::vec3 endPos4 = glm::vec3(-13.0f, 17.0f, -4.5f);
		glm::vec3 startPos4 = glm::vec3(-13.0f, 17.0f, -4.0f);

		bool moundMove = true;
		bool wellMove = true;

		bool Phantom2Move = false;
		bool Phantom2Move2 = false;

		glm::vec3 endPos3 = glm::vec3(-13.0f, 17.0f, -1.35f);
		glm::vec3 startPos3 = glm::vec3(-13.0f, 17.0f, -0.85f);

		glm::vec3 endPos5 = glm::vec3(-27.5f, -55.0f, -2.9f);
		glm::vec3 startPos5 = glm::vec3(-9.5f, -55.0f, -2.9f);

		glm::vec3 endPos6 = glm::vec3(-9.5f, -45.0f, -2.9f);
		glm::vec3 startPos6 = glm::vec3(-27.5f, -45.0f, -2.9f);
		glm::vec3 currentPosition;
		glm::vec3 currentPosition2;

		float ObjectTimer2 = 0.0f;
		float Object2Timer2 = 0.0f;
		float ObjectTimeLimit = 2.0f;

		//Phantom Rotation Lerp
		glm::quat startPhantomRot = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::quat endPhantomRot = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		glm::quat startPhantomRot2 = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::quat endPhantomRot2 = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 1.0f, 1.0f));



		float PhantomRotTimer = 0.0f;
		float PhantomRotTimeLimit = 4.0f;

		bool PhantomRot = true;
		bool PhantomRot2 = true;

		bool flipPhantom = false;
		bool flipPhantom2 = false;

		#pragma endregion
		
		
		#pragma region Shader and ImGui

		Shader::sptr passthroughShader = Shader::Create();
		passthroughShader->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
		passthroughShader->LoadShaderPartFromFile("shaders/passthrough_frag.glsl", GL_FRAGMENT_SHADER);
		passthroughShader->Link();

		Shader::sptr simpleDepthShader = Shader::Create();
		simpleDepthShader->LoadShaderPartFromFile("shaders/simple_depth_vert.glsl", GL_VERTEX_SHADER);
		simpleDepthShader->LoadShaderPartFromFile("shaders/simple_depth_frag.glsl", GL_FRAGMENT_SHADER);
		simpleDepthShader->Link();

		Shader::sptr physicsShader = Shader::Create();
		physicsShader->LoadShaderPartFromFile("shaders/Physics/physics_vertex_shader.glsl", GL_VERTEX_SHADER);
		physicsShader->LoadShaderPartFromFile("shaders/Physics/physics_frag_shader.glsl", GL_FRAGMENT_SHADER);
		physicsShader->Link();

		Shader::sptr gltfShader = Shader::Create();
		gltfShader->LoadShaderPartFromFile("shaders/skinned_vertex_shader.glsl", GL_VERTEX_SHADER);
		gltfShader->LoadShaderPartFromFile("shaders/directional_blinn_phong_frag.glsl", GL_FRAGMENT_SHADER);
		gltfShader->Link();

		// Load our shaders
		Shader::sptr shader = Shader::Create();
		shader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
		shader->LoadShaderPartFromFile("shaders/directional_blinn_phong_frag.glsl", GL_FRAGMENT_SHADER);
		shader->Link();
		
		//Creates our directional Light
		DirectionalLight theSun;
		UniformBuffer directionalLightBuffer;

		//Allocates enough memory for one directional light (we can change this easily, but we only need 1 directional light)
		directionalLightBuffer.AllocateMemory(sizeof(DirectionalLight));
		//Casts our sun as "data" and sends it to the shader
		directionalLightBuffer.SendData(reinterpret_cast<void*>(&theSun), sizeof(DirectionalLight));

		directionalLightBuffer.Bind(0);

		glm::vec3 lightPos = glm::vec3(-3.0f, 0.0f, 3.0f);
		glm::vec3 lightCol = glm::vec3(0.9f, 0.85f, 0.5f);
		float     lightAmbientPow = 0.639f;
		float     lightSpecularPow = 3.0f;
		glm::vec3 ambientCol = glm::vec3(1.0f);
		float     ambientPow = 0.6f;
		float     textureMix = 0.2f;
		float     shininess = 16.0f;
		float     lightLinearFalloff = 0.0f; //0.09f;
		float     lightQuadraticFalloff = 0.0f; //0.032f;
		float     outlineThickness = 0.15;
		
		// These are our application / scene level uniforms that don't necessarily update
		// every frame
		/*shader->SetUniform("u_LightPos", lightPos);
		shader->SetUniform("u_LightCol", lightCol);
		shader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
		shader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
		shader->SetUniform("u_AmbientCol", ambientCol);
		shader->SetUniform("u_AmbientStrength", ambientPow);
		shader->SetUniform("u_TextureMix", textureMix);
		shader->SetUniform("u_Shininess", shininess);
		shader->SetUniform("u_LightAttenuationConstant", 1.0f);
		shader->SetUniform("u_LightAttenuationLinear", lightLinearFalloff);
		shader->SetUniform("u_LightAttenuationQuadratic", lightQuadraticFalloff);
		shader->SetUniform("u_OutlineThickness", outlineThickness);*/

		
		//Morph Shader (Animated Shader)
		Shader::sptr morphShader = Shader::Create();
		morphShader->LoadShaderPartFromFile("shaders/morphvertex_shader.glsl", GL_VERTEX_SHADER);
		morphShader->LoadShaderPartFromFile("shaders/directional_blinn_phong_frag.glsl", GL_FRAGMENT_SHADER);
		morphShader->Link();
		
		
		/*morphShader->SetUniform("u_LightPos", glm::vec3(0.0f, 0.0f, 35.0f));
		morphShader->SetUniform("u_LightCol", lightCol);
		morphShader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
		morphShader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
		morphShader->SetUniform("u_AmbientCol", ambientCol);
		morphShader->SetUniform("u_AmbientStrength", ambientPow);
		morphShader->SetUniform("u_TextureMix", textureMix);
		morphShader->SetUniform("u_Shininess", shininess);
		morphShader->SetUniform("u_LightAttenuationConstant", 1.0f);
		morphShader->SetUniform("u_LightAttenuationLinear", lightLinearFalloff);
		morphShader->SetUniform("u_LightAttenuationQuadratic", lightQuadraticFalloff);
		morphShader->SetUniform("u_OutlineThickness", outlineThickness);*/


		//Sprite Shader
		Shader::sptr spriteShader = Shader::Create();
		spriteShader->LoadShaderPartFromFile("shaders/spritevertex_shader.glsl", GL_VERTEX_SHADER);
		spriteShader->LoadShaderPartFromFile("shaders/spritefrag_shader.glsl", GL_FRAGMENT_SHADER);
		spriteShader->Link();
		
		//Shader Toggles
		bool lightingToggle = false;
		bool ambientToggle = false;
		bool specularToggle = false;
		bool ambientSpecularToggle = false;
		bool customToggle = true;
		bool diffuseRampToggle = false;
		bool specularRampToggle = false;

		shader->SetUniform("u_LightToggle", (int)lightingToggle);
		shader->SetUniform("u_AmbientToggle", (int)ambientToggle);
		shader->SetUniform("u_SpecularToggle", (int)specularToggle);
		shader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
		shader->SetUniform("u_CustomToggle", (int)customToggle);
		shader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
		shader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);

		morphShader->SetUniform("u_LightToggle", (int)lightingToggle);
		morphShader->SetUniform("u_AmbientToggle", (int)ambientToggle);
		morphShader->SetUniform("u_SpecularToggle", (int)specularToggle);
		morphShader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
		morphShader->SetUniform("u_CustomToggle", (int)customToggle);
		morphShader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
		morphShader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);

		
		//Basic Effect(does nothing)
		PostEffect* basicEffect0;
		PostEffect* basicEffect;
		PostEffect* basicEffect2;
		PostEffect* basicEffect3;
		Framebuffer* shadowBuffer;
		Framebuffer* shadowBuffer2;
		Framebuffer* shadowBuffer3;

		int activeEffect = 4;
		std::vector<PostEffect*> effects;

		//Effects
		SepiaEffect* sepiaEffect;

		GreyscaleEffect* greyscaleEffect;

		ColorCorrectionEffect* colorCorrectionEffect;

		WarmEffect* warmEffect;

		CoolEffect* coolEffect;

		BloomEffect* bloomEffect;

		VignetteEffect* vignetteEffect;

		PixelateEffect* pixelateEffect;

		//Scene 2 Effects
		std::vector<PostEffect*> scene2Effects;

		SepiaEffect* sepiaEffect2;

		GreyscaleEffect* greyscaleEffect2;

		ColorCorrectionEffect* colorCorrectionEffect2;

		WarmEffect* warmEffect2;

		CoolEffect* coolEffect2;

		BloomEffect* bloomEffect2;

		VignetteEffect* vignetteEffect2;

		PixelateEffect* pixelateEffect2;

		//Scene 3 Effects
		std::vector<PostEffect*> scene3Effects;

		SepiaEffect* sepiaEffect3;

		GreyscaleEffect* greyscaleEffect3;

		ColorCorrectionEffect* colorCorrectionEffect3;

		WarmEffect* warmEffect3;

		CoolEffect* coolEffect3;

		BloomEffect* bloomEffect3;

		VignetteEffect* vignetteEffect3;

		PixelateEffect* pixelateEffect3;

		//Physics Body Control
		glm::vec3 angleRotation = glm::vec3(0, 0, 0);
		glm::vec3 bodyTranslation = glm::vec3(0, 0, 0);

		float scaleVariable = 0.223f;

		//Shadow Control
		glm::vec2 leftRight = glm::vec2(-50.0f, 50.0f);
		glm::vec2 bottomTop = glm::vec2(-50.0f, 50.0f);
		glm::vec2 nearFar = glm::vec2(-30.0f, 30.0f);

		//Shifting Camera
		glm::vec3 shiftCamera = glm::vec3(0, 0, 0);

		// We'll add some ImGui controls to control our shader
		BackendHandler::imGuiCallbacks.push_back([&]() {
			if (ImGui::CollapsingHeader("Effect Controls"))
			{
				ImGui::SliderInt("Chosen Effect", &activeEffect, 0, effects.size() - 1);
				
				if (activeEffect == 0)
				{
					ImGui::Text("Active Effect: Greyscale Effect");

					GreyscaleEffect* temp = (GreyscaleEffect*)effects[activeEffect];
					float intensity = temp->GetIntensity();

					if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1.0f))
					{
						temp->SetIntensity(intensity);
					}
				}
				if (activeEffect == 1)
				{
					ImGui::Text("Active Effect: Warm Color Correction Effect");

					WarmEffect* temp = (WarmEffect*)effects[activeEffect];
				}
				if (activeEffect == 2)
				{
					ImGui::Text("Active Effect: Cool Color Correction Effect");

					CoolEffect* temp = (CoolEffect*)effects[activeEffect];
				}
				if (activeEffect == 3)
				{
					ImGui::Text("Active Effect: Custom Color Correction Effect");

					ColorCorrectionEffect* temp = (ColorCorrectionEffect*)effects[activeEffect];
				}
				if (activeEffect == 4)
				{
					ImGui::Text("Active Effect: Bloom Effect");

					BloomEffect* temp = (BloomEffect*)effects[activeEffect];
					float passes = temp->GetPasses();
					float threshold = temp->GetThreshold();

					if (ImGui::SliderFloat("Blur Value", &passes, 0.0f, 40.0f)) {
						temp->SetPasses(passes);
					}
					if (ImGui::SliderFloat("Brightness Threshold", &threshold, 0.0f, 2.0f)) {
						temp->SetThreshold(threshold);
					}
				}
				if (activeEffect == 5)
				{
					ImGui::Text("Active Effect: Vignette Effect");

					VignetteEffect* temp = (VignetteEffect*)effects[activeEffect];
					float innerRad = temp->GetInnerRadius();
					float outerRad = temp->GetOuterRadius();
					float opacity = temp->GetOpacity();

					if (ImGui::SliderFloat("Inner Radius", &innerRad, 0.0f, 1.0f)) {
						temp->SetInnerRadius(innerRad);
					}
					if (ImGui::SliderFloat("Outer Radius", &outerRad, 0.0f, 1.0f)) {
						temp->SetOuterRadius(outerRad);
					}
					if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f)) {
						temp->SetOpacity(opacity);
					}
					
				}
				if (activeEffect == 6)
				{
					ImGui::Text("Active Effect: Pixelate Effect");

					PixelateEffect* temp = (PixelateEffect*)effects[activeEffect];

					float intensity = temp->GetIntensity();

					if (ImGui::SliderFloat("Intensity", &intensity, 400.0f, 8000.0f))
					{
						temp->SetIntensity(intensity);
					}
				}
				if (ImGui::Button("Toggle Textures"))
				{
					ToggleTextures = !ToggleTextures;
				}
			}
			if (ImGui::CollapsingHeader("Effect Controls for Scene 2"))
			{
				ImGui::SliderInt("Chosen Effect", &activeEffect, 0, effects.size() - 1);

				if (activeEffect == 0)
				{
					ImGui::Text("Active Effect: Greyscale Effect");

					GreyscaleEffect* temp = (GreyscaleEffect*)scene2Effects[activeEffect];
					float intensity = temp->GetIntensity();

					if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1.0f))
					{
						temp->SetIntensity(intensity);
					}
				}
				if (activeEffect == 1)
				{
					ImGui::Text("Active Effect: Warm Color Correction Effect");

					WarmEffect* temp = (WarmEffect*)scene2Effects[activeEffect];
				}
				if (activeEffect == 2)
				{
					ImGui::Text("Active Effect: Cool Color Correction Effect");

					CoolEffect* temp = (CoolEffect*)scene2Effects[activeEffect];
				}
				if (activeEffect == 3)
				{
					ImGui::Text("Active Effect: Custom Color Correction Effect");

					ColorCorrectionEffect* temp = (ColorCorrectionEffect*)scene2Effects[activeEffect];
				}
				if (activeEffect == 4)
				{
					ImGui::Text("Active Effect: Bloom Effect");

					BloomEffect* temp = (BloomEffect*)scene2Effects[activeEffect];
					float passes = temp->GetPasses();
					float threshold = temp->GetThreshold();

					if (ImGui::SliderFloat("Blur Value", &passes, 0.0f, 40.0f)) {
						temp->SetPasses(passes);
					}
					if (ImGui::SliderFloat("Brightness Threshold", &threshold, 0.0f, 2.0f)) {
						temp->SetThreshold(threshold);
					}
				}
				if (activeEffect == 5)
				{
					ImGui::Text("Active Effect: Vignette Effect");

					VignetteEffect* temp = (VignetteEffect*)scene2Effects[activeEffect];
					float innerRad = temp->GetInnerRadius();
					float outerRad = temp->GetOuterRadius();
					float opacity = temp->GetOpacity();

					if (ImGui::SliderFloat("Inner Radius", &innerRad, 0.0f, 1.0f)) {
						temp->SetInnerRadius(innerRad);
					}
					if (ImGui::SliderFloat("Outer Radius", &outerRad, 0.0f, 1.0f)) {
						temp->SetOuterRadius(outerRad);
					}
					if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f)) {
						temp->SetOpacity(opacity);
					}

				}
				if (activeEffect == 6)
				{
					ImGui::Text("Active Effect: Pixelate Effect");

					PixelateEffect* temp = (PixelateEffect*)scene2Effects[activeEffect];

					float intensity = temp->GetIntensity();

					if (ImGui::SliderFloat("Intensity", &intensity, 400.0f, 3000.0f))
					{
						temp->SetIntensity(intensity);
					}
				}
			}
			if (ImGui::CollapsingHeader("Effect Controls for Scene 3"))
			{
				ImGui::SliderInt("Chosen Effect", &activeEffect, 0, effects.size() - 1);

				if (activeEffect == 0)
				{
					ImGui::Text("Active Effect: Greyscale Effect");

					GreyscaleEffect* temp = (GreyscaleEffect*)scene3Effects[activeEffect];
					float intensity = temp->GetIntensity();

					if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1.0f))
					{
						temp->SetIntensity(intensity);
					}
				}
				if (activeEffect == 1)
				{
					ImGui::Text("Active Effect: Warm Color Correction Effect");

					WarmEffect* temp = (WarmEffect*)scene3Effects[activeEffect];
				}
				if (activeEffect == 2)
				{
					ImGui::Text("Active Effect: Cool Color Correction Effect");

					CoolEffect* temp = (CoolEffect*)scene3Effects[activeEffect];
				}
				if (activeEffect == 3)
				{
					ImGui::Text("Active Effect: Custom Color Correction Effect");

					ColorCorrectionEffect* temp = (ColorCorrectionEffect*)scene3Effects[activeEffect];
				}
				if (activeEffect == 4)
				{
					ImGui::Text("Active Effect: Bloom Effect");

					BloomEffect* temp = (BloomEffect*)scene3Effects[activeEffect];
					float passes = temp->GetPasses();
					float threshold = temp->GetThreshold();

					if (ImGui::SliderFloat("Blur Value", &passes, 0.0f, 40.0f)) {
						temp->SetPasses(passes);
					}
					if (ImGui::SliderFloat("Brightness Threshold", &threshold, 0.0f, 2.0f)) {
						temp->SetThreshold(threshold);
					}
				}
				if (activeEffect == 5)
				{
					ImGui::Text("Active Effect: Vignette Effect");

					VignetteEffect* temp = (VignetteEffect*)scene3Effects[activeEffect];
					float innerRad = temp->GetInnerRadius();
					float outerRad = temp->GetOuterRadius();
					float opacity = temp->GetOpacity();

					if (ImGui::SliderFloat("Inner Radius", &innerRad, 0.0f, 1.0f)) {
						temp->SetInnerRadius(innerRad);
					}
					if (ImGui::SliderFloat("Outer Radius", &outerRad, 0.0f, 1.0f)) {
						temp->SetOuterRadius(outerRad);
					}
					if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f)) {
						temp->SetOpacity(opacity);
					}

				}
				if (activeEffect == 6)
				{
					ImGui::Text("Active Effect: Pixelate Effect");

					PixelateEffect* temp = (PixelateEffect*)scene3Effects[activeEffect];

					float intensity = temp->GetIntensity();

					if (ImGui::SliderFloat("Intensity", &intensity, 400.0f, 3000.0f))
					{
						temp->SetIntensity(intensity);
					}
				}
			}
			if (ImGui::CollapsingHeader("Scene Level Lighting Settings"))
			{
				if (ImGui::ColorPicker3("Ambient Color", glm::value_ptr(ambientCol))) {
					shader->SetUniform("u_AmbientCol", ambientCol);
					morphShader->SetUniform("u_AmbientCol", ambientCol);
				}
				if (ImGui::SliderFloat("Fixed Ambient Power", &ambientPow, 0.01f, 1.0f)) {
					shader->SetUniform("u_AmbientStrength", ambientPow);
					morphShader->SetUniform("u_AmbientStrength", ambientPow);
				}
			}
			if (ImGui::CollapsingHeader("Light Level Lighting Settings"))
			{
				if (ImGui::DragFloat3("Light Pos", glm::value_ptr(lightPos), 0.01f, -10.0f, 10.0f)) {
					shader->SetUniform("u_LightPos", lightPos);
					morphShader->SetUniform("u_LightPos", lightPos);
				}
				if (ImGui::ColorPicker3("Light Col", glm::value_ptr(lightCol))) {
					shader->SetUniform("u_LightCol", lightCol);
					morphShader->SetUniform("u_LightCol", lightCol);
				}
				if (ImGui::SliderFloat("Light Ambient Power", &lightAmbientPow, 0.0f, 1.0f)) {
					shader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
					morphShader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
				}
				if (ImGui::SliderFloat("Light Specular Power", &lightSpecularPow, 0.0f, 1.0f)) {
					shader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
					morphShader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
				}
				if (ImGui::DragFloat("Light Linear Falloff", &lightLinearFalloff, 0.01f, 0.0f, 1.0f)) {
					shader->SetUniform("u_LightAttenuationLinear", lightLinearFalloff);
					morphShader->SetUniform("u_LightAttenuationLinear", lightLinearFalloff);
				}
				if (ImGui::DragFloat("Light Quadratic Falloff", &lightQuadraticFalloff, 0.01f, 0.0f, 1.0f)) {
					shader->SetUniform("u_LightAttenuationQuadratic", lightQuadraticFalloff);
					morphShader->SetUniform("u_LightAttenuationQuadratic", lightQuadraticFalloff);
				}
			}
			if (ImGui::CollapsingHeader("Phantom Waypoints"))
			{
				if (ImGui::Button("Add Waypoint"))
				{
					glm::vec3 temp{};
					phantomWaypoints.push_back(temp);
				}

				if (ImGui::Button("Remove Waypoint"))
				{
					phantomWaypoints.pop_back();
				}

				//Interface for selecting a waypoint.
				static size_t pointSelected = 0;
				static std::string pointLabel = "";

				if (pointSelected >= phantomWaypoints.size())
					pointSelected = phantomWaypoints.size() - 1;

				for (size_t i = 0; i < phantomWaypoints.size(); ++i)
				{
					pointLabel = "Waypoint " + std::to_string(i);

					if (ImGui::Selectable(pointLabel.c_str(), i == pointSelected))
					{
						pointSelected = i;
					}
				}
				
				//Interface for moving a selected component.
				if (pointSelected < phantomWaypoints.size())
				{
					static bool transformPanel = true;

					ImGui::Begin("Point Coordinates", &transformPanel, ImVec2(300, 100));

					//TODO: How will we update our point's coordinates?
					ImGui::SliderFloat("X", &phantomWaypoints[pointSelected].x, -50.0f, 50.0f);
					ImGui::SliderFloat("Y", &phantomWaypoints[pointSelected].y, -50.0f, 50.0f);
					ImGui::SliderFloat("Z", &phantomWaypoints[pointSelected].z, -50.0f, 50.0f);

					ImGui::End();
				}
			}
			if (ImGui::CollapsingHeader("Shader Toggles"))
			{ 
				if (ImGui::Checkbox("No Lighting", &lightingToggle))
				{
					lightingToggle = true;
					ambientToggle = false;
					specularToggle = false;
					ambientSpecularToggle = false;
					customToggle = false;
					diffuseRampToggle = false;
					specularRampToggle = false;
					shader->SetUniform("u_LightToggle", (int)lightingToggle);
					shader->SetUniform("u_AmbientToggle", (int)ambientToggle);
					shader->SetUniform("u_SpecularToggle", (int)specularToggle);
					shader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
					shader->SetUniform("u_CustomToggle", (int)customToggle);
					shader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
					shader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);

					morphShader->SetUniform("u_LightToggle", (int)lightingToggle);
					morphShader->SetUniform("u_AmbientToggle", (int)ambientToggle);
					morphShader->SetUniform("u_SpecularToggle", (int)specularToggle);
					morphShader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
					morphShader->SetUniform("u_CustomToggle", (int)customToggle);
					morphShader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
					morphShader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);
				}
				if (ImGui::Checkbox("Ambient Only", &ambientToggle))
				{
					lightingToggle = false;
					ambientToggle = true;
					specularToggle = false;
					ambientSpecularToggle = false;
					customToggle = false;
					diffuseRampToggle = false;
					specularRampToggle = false;
					shader->SetUniform("u_LightToggle", (int)lightingToggle);
					shader->SetUniform("u_AmbientToggle", (int)ambientToggle);
					shader->SetUniform("u_SpecularToggle", (int)specularToggle);
					shader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
					shader->SetUniform("u_CustomToggle", (int)customToggle);
					shader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
					shader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);

					morphShader->SetUniform("u_LightToggle", (int)lightingToggle);
					morphShader->SetUniform("u_AmbientToggle", (int)ambientToggle);
					morphShader->SetUniform("u_SpecularToggle", (int)specularToggle);
					morphShader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
					morphShader->SetUniform("u_CustomToggle", (int)customToggle);
					morphShader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
					morphShader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);
				}
				if (ImGui::Checkbox("Specular Only", &specularToggle))
				{
					lightingToggle = false;
					ambientToggle = false;
					specularToggle = true;
					ambientSpecularToggle = false;
					customToggle = false;
					diffuseRampToggle = false;
					specularRampToggle = false;
					shader->SetUniform("u_LightToggle", (int)lightingToggle);
					shader->SetUniform("u_AmbientToggle", (int)ambientToggle);
					shader->SetUniform("u_SpecularToggle", (int)specularToggle);
					shader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
					shader->SetUniform("u_CustomToggle", (int)customToggle);
					shader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
					shader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);

					morphShader->SetUniform("u_LightToggle", (int)lightingToggle);
					morphShader->SetUniform("u_AmbientToggle", (int)ambientToggle);
					morphShader->SetUniform("u_SpecularToggle", (int)specularToggle);
					morphShader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
					morphShader->SetUniform("u_CustomToggle", (int)customToggle);
					morphShader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
					morphShader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);
				}
				if (ImGui::Checkbox("Ambient + Specular", &ambientSpecularToggle))
				{
					lightingToggle = false;
					ambientToggle = false;
					specularToggle = false;
					ambientSpecularToggle = true;
					customToggle = false;
					diffuseRampToggle = false;
					specularRampToggle = false;
					shader->SetUniform("u_LightToggle", (int)lightingToggle);
					shader->SetUniform("u_AmbientToggle", (int)ambientToggle);
					shader->SetUniform("u_SpecularToggle", (int)specularToggle);
					shader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
					shader->SetUniform("u_CustomToggle", (int)customToggle);
					shader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
					shader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);

					morphShader->SetUniform("u_LightToggle", (int)lightingToggle);
					morphShader->SetUniform("u_AmbientToggle", (int)ambientToggle);
					morphShader->SetUniform("u_SpecularToggle", (int)specularToggle);
					morphShader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
					morphShader->SetUniform("u_CustomToggle", (int)customToggle);
					morphShader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
					morphShader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);
				}
				if (ImGui::Checkbox("Ambient + Specular + Toon Shading", &customToggle))
				{
					lightingToggle = false;
					ambientToggle = false;
					specularToggle = false;
					ambientSpecularToggle = false;
					customToggle = true;
					diffuseRampToggle = false;
					specularRampToggle = false;
					shader->SetUniform("u_LightToggle", (int)lightingToggle);
					shader->SetUniform("u_AmbientToggle", (int)ambientToggle);
					shader->SetUniform("u_SpecularToggle", (int)specularToggle);
					shader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
					shader->SetUniform("u_CustomToggle", (int)customToggle);
					shader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
					shader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);

					morphShader->SetUniform("u_LightToggle", (int)lightingToggle);
					morphShader->SetUniform("u_AmbientToggle", (int)ambientToggle);
					morphShader->SetUniform("u_SpecularToggle", (int)specularToggle);
					morphShader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
					morphShader->SetUniform("u_CustomToggle", (int)customToggle);
					morphShader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
					morphShader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);
				}
				if (ImGui::Checkbox("Diffuse Ramp", &diffuseRampToggle))
				{
					lightingToggle = false;
					ambientToggle = false;
					specularToggle = false;
					ambientSpecularToggle = false;
					customToggle = false;
					diffuseRampToggle = true;
					specularRampToggle = false;
					shader->SetUniform("u_LightToggle", (int)lightingToggle);
					shader->SetUniform("u_AmbientToggle", (int)ambientToggle);
					shader->SetUniform("u_SpecularToggle", (int)specularToggle);
					shader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
					shader->SetUniform("u_CustomToggle", (int)customToggle);
					shader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
					shader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);

					morphShader->SetUniform("u_LightToggle", (int)lightingToggle);
					morphShader->SetUniform("u_AmbientToggle", (int)ambientToggle);
					morphShader->SetUniform("u_SpecularToggle", (int)specularToggle);
					morphShader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
					morphShader->SetUniform("u_CustomToggle", (int)customToggle);
					morphShader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
					morphShader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);
				}
				if (ImGui::Checkbox("Specular Ramp", &specularRampToggle))
				{
					lightingToggle = false;
					ambientToggle = false;
					specularToggle = false;
					ambientSpecularToggle = false;
					customToggle = false;
					diffuseRampToggle = false;
					specularRampToggle = true;
					shader->SetUniform("u_LightToggle", (int)lightingToggle);
					shader->SetUniform("u_AmbientToggle", (int)ambientToggle);
					shader->SetUniform("u_SpecularToggle", (int)specularToggle);
					shader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
					shader->SetUniform("u_CustomToggle", (int)customToggle);
					shader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
					shader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);

					morphShader->SetUniform("u_LightToggle", (int)lightingToggle);
					morphShader->SetUniform("u_AmbientToggle", (int)ambientToggle);
					morphShader->SetUniform("u_SpecularToggle", (int)specularToggle);
					morphShader->SetUniform("u_AmbientSpecularToggle", (int)ambientSpecularToggle);
					morphShader->SetUniform("u_CustomToggle", (int)customToggle);
					morphShader->SetUniform("u_DiffuseRampToggle", (int)diffuseRampToggle);
					morphShader->SetUniform("u_SpecularRampToggle", (int)specularRampToggle);
				}
			}
			if (ImGui::CollapsingHeader("Sun Light Level Lighting Settings"))
			{
				if (ImGui::DragFloat3("Light Direction/Position", glm::value_ptr(theSun._lightDirection), 0.01f, -100.0f, 100.0f))
				{
					directionalLightBuffer.SendData(reinterpret_cast<void*>(&theSun), sizeof(DirectionalLight));
				}
				if (ImGui::SliderFloat("Ambient Power", &(theSun._ambientPow), 0.0f, 1.0f)) {
					directionalLightBuffer.SendData(reinterpret_cast<void*>(&theSun), sizeof(DirectionalLight));
					//shader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
					//morphShader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
				}
				if (ImGui::ColorPicker3("Light Col", glm::value_ptr(theSun._lightCol)))
				{
					directionalLightBuffer.SendData(reinterpret_cast<void*>(&theSun), sizeof(DirectionalLight));
				}
			}
			if (ImGui::CollapsingHeader("Physics Controls"))
			{
				if (ImGui::DragFloat3("Translation", glm::value_ptr(bodyTranslation), 0.1f, -50.0f, 200.0f)) {

				}
				if (ImGui::DragFloat3("Rotation", glm::value_ptr(angleRotation), 10.0f, -360.0f, 360.0f)) {

				}
				if (ImGui::SliderFloat("Scale Sprite", &scaleVariable, 0.01f, 10.0f))
				{

				}
			}
			if (ImGui::CollapsingHeader("Shadow Controls"))
			{
				if (ImGui::DragFloat2("Left and Right", glm::value_ptr(leftRight), 0.1, -1000.0f, 1000.0f))
				{

				}
				if (ImGui::DragFloat2("Bottom and Top", glm::value_ptr(bottomTop), 0.1, -1000.0f, 1000.0f))
				{

				}
				if (ImGui::DragFloat2("Near and Far", glm::value_ptr(nearFar), 0.1, -1000.0f, 1000.0f))
				{

				}
			}
			if (ImGui::CollapsingHeader("Camera Controls"))
			{
				if (ImGui::DragFloat3("Camera Controls: ", glm::value_ptr(shiftCamera), 0.1, -100.0f, 100.0f))
				{

				}
			}
			
			
			

			auto name = controllables[selectedVao].get<GameObjectTag>().Name;
			ImGui::Text(name.c_str());
			auto behaviour = BehaviourBinding::Get<SimpleMoveBehaviour>(controllables[selectedVao]);
			ImGui::Checkbox("Relative Rotation", &behaviour->Relative);

			ImGui::Text("Q/E -> Yaw\nLeft/Right -> Roll\nUp/Down -> Pitch\nY -> Toggle Mode");
		
			minFps = FLT_MAX;
			maxFps = 0;
			avgFps = 0;
			for (int ix = 0; ix < 128; ix++) {
				if (fpsBuffer[ix] < minFps) { minFps = fpsBuffer[ix]; }
				if (fpsBuffer[ix] > maxFps) { maxFps = fpsBuffer[ix]; }
				avgFps += fpsBuffer[ix];
			}
			ImGui::PlotLines("FPS", fpsBuffer, 128);
			ImGui::Text("MIN: %f MAX: %f AVG: %f", minFps, maxFps, avgFps / 128.0f);
			});

		#pragma endregion 

		// GL states
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LEQUAL); // New 
		
		#pragma region TEXTURE LOADING

		// Load some textures from files
		Texture2D::sptr stone = Texture2D::LoadFromFile("images/Stone_001_Diffuse.png");
		Texture2D::sptr stoneSpec = Texture2D::LoadFromFile("images/Stone_001_Specular.png");
		Texture2D::sptr grass = Texture2D::LoadFromFile("images/grass.jpg");
		Texture2D::sptr noSpec = Texture2D::LoadFromFile("images/grassSpec.png");
		Texture2D::sptr box = Texture2D::LoadFromFile("images/box.bmp");
		Texture2D::sptr boxSpec = Texture2D::LoadFromFile("images/box-reflections.bmp");
		Texture2D::sptr simpleFlora = Texture2D::LoadFromFile("images/SimpleFlora.png");

		Texture2D::sptr PlayerDiffuse = Texture2D::LoadFromFile("images/NewCharacterTexture.png");
		Texture2D::sptr diffuseMp02 = Texture2D::LoadFromFile("images/plains_island_texture.png");
		Texture2D::sptr diffuseMp03 = Texture2D::LoadFromFile("images/WizardColours.png");
		Texture2D::sptr diffuseMp04 = Texture2D::LoadFromFile("images/PhantomColours.png");
		Texture2D::sptr diffuseMp05 = Texture2D::LoadFromFile("images/CoinTex.png");
		Texture2D::sptr diffuseMp06 = Texture2D::LoadFromFile("images/MoundColours.png");
		Texture2D::sptr diffuseMp07 = Texture2D::LoadFromFile("images/WishingWellColours.png");
		Texture2D::sptr diffuseMp08 = Texture2D::LoadFromFile("images/FlowerUVsColor.png");
		Texture2D::sptr diffuseMp09 = Texture2D::LoadFromFile("images/portal_texture.png");
		Texture2D::sptr diffuseMp10 = Texture2D::LoadFromFile("images/crystal_texture.png");
		Texture2D::sptr diffuseMp11 = Texture2D::LoadFromFile("images/taiga_island_texture.png");
		Texture2D::sptr diffuseMp12 = Texture2D::LoadFromFile("images/BushColour.png");
		Texture2D::sptr diffuseMp13 = Texture2D::LoadFromFile("images/BlackColour.png");
		Texture2D::sptr diffuseMp14 = Texture2D::LoadFromFile("images/brown.png");
		Texture2D::sptr diffuseMp15 = Texture2D::LoadFromFile("images/SignTexture.png");
		Texture2D::sptr diffuseMp16 = Texture2D::LoadFromFile("images/TreeTexture.png");
		Texture2D::sptr diffuseMp17 = Texture2D::LoadFromFile("images/PlatformTexture.png");
		Texture2D::sptr diffuseMp18 = Texture2D::LoadFromFile("images/StoneTexture.png");
		Texture2D::sptr diffuseMp19 = Texture2D::LoadFromFile("images/MailboxColor.png");
		Texture2D::sptr diffuseMp20 = Texture2D::LoadFromFile("images/flowerTexture.png");
		Texture2D::sptr diffuseMp21 = Texture2D::LoadFromFile("images/HouseColors.png");

		Texture2D::sptr diffuseMp22 = Texture2D::LoadFromFile("images/FireIsland_Texture.png");
		Texture2D::sptr diffuseMp23 = Texture2D::LoadFromFile("images/Volcano_Texture.png");
		Texture2D::sptr diffuseMp24 = Texture2D::LoadFromFile("images/DeadBirchTreeTexture.png");
		Texture2D::sptr diffuseMp25 = Texture2D::LoadFromFile("images/CreatureTexture.png");
		Texture2D::sptr diffuseMp26 = Texture2D::LoadFromFile("images/barrel_texture.png");
		Texture2D::sptr diffuseMp27 = Texture2D::LoadFromFile("images/FireIslandPlatform_Texture.png");

		Texture2D::sptr diffuseMp28 = Texture2D::LoadFromFile("images/snowman_texture.png");
		Texture2D::sptr diffuseMp29 = Texture2D::LoadFromFile("images/sled_texture.png");
		Texture2D::sptr diffuseMp30 = Texture2D::LoadFromFile("images/ice crystal_texture.png");

		Texture2D::sptr diffuseMp31 = Texture2D::LoadFromFile("images/PineTreeColours.png");
		Texture2D::sptr diffuseMp32 = Texture2D::LoadFromFile("images/tree1_texture.png");
		Texture2D::sptr diffuseMp33 = Texture2D::LoadFromFile("images/tree2_texture.png");

		Texture2D::sptr parkingMeterDiffuse = Texture2D::LoadFromFile("images/Parking_Meter_Texture.png");
		Texture2D::sptr parkingMeterPoleDiffuse = Texture2D::LoadFromFile("images/Parking_Meter_Pole_Texture.png");
		Texture2D::sptr buttonDiffuse = Texture2D::LoadFromFile("images/ButtonTexture.png");
		Texture2D::sptr snowBlockDiffuse = Texture2D::LoadFromFile("images/SnowBlockTexture.png");
		Texture2D::sptr sketchyGuyDiffuse = Texture2D::LoadFromFile("images/SketchyGuyTexture.png");
		Texture2D::sptr sketchyGuy2Diffuse = Texture2D::LoadFromFile("images/SketchyGuyBrotherTexture.png");

		Texture2D::sptr WhiteDiffuse = Texture2D::LoadFromFile("images/White.png");

		Texture2D::sptr emissiveBlueDiffuse = Texture2D::LoadFromFile("images/Emissive Wizard.png");
		Texture2D::sptr emissiveYellowDiffuse = Texture2D::LoadFromFile("images/Emissive Coin.png");

		//Heart Sprites
		Texture2D::sptr heartDiffuse = Texture2D::LoadFromFile("images/heart.png");

		//Interaction Sprites
		Texture2D::sptr interactDiffuse = Texture2D::LoadFromFile("images/Dialogue/Interact.png");
		Texture2D::sptr talkDiffuse = Texture2D::LoadFromFile("images/Dialogue/Talk.png");
		Texture2D::sptr continueDiffuse = Texture2D::LoadFromFile("images/Dialogue/Continue.png");

		//Dialogue Sprites
		Texture2D::sptr dialogueDiffuseL1D1 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level1-1.png");
		Texture2D::sptr dialogueDiffuseL1D2 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level1-2.png");
		Texture2D::sptr dialogueDiffuseL1D3 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level1-3.png");
		Texture2D::sptr dialogueDiffuseL1D4 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level1-4.png");
		Texture2D::sptr dialogueDiffuseL1D5 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level1-5.png");
		Texture2D::sptr dialogueDiffuseL1D6 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level1-6.png");
		Texture2D::sptr dialogueDiffuseL1D7 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level1-7.png");
		Texture2D::sptr dialogueDiffuseL1D8 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level1-8.png");
		Texture2D::sptr dialogueDiffuseL1D9 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level1-9.png");
		Texture2D::sptr dialogueDiffuseL1D10 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level1-10.png");
		Texture2D::sptr dialogueDiffuseL1D11 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level1-11.png");

		Texture2D::sptr dialogueDiffuseL2D1 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level2-1.png");
		Texture2D::sptr dialogueDiffuseL2D2 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level2-2.png");
		Texture2D::sptr dialogueDiffuseL2D3 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level2-3.png");
		Texture2D::sptr dialogueDiffuseL2D4 = Texture2D::LoadFromFile("images/Dialogue/Dialogue Level2-4.png");

		Texture2D::sptr sketchyGuyDiffuseD1 = Texture2D::LoadFromFile("images/Dialogue/Dealer1-1.png");
		Texture2D::sptr sketchyGuyDiffuseD2 = Texture2D::LoadFromFile("images/Dialogue/Dealer1-2.png");
		Texture2D::sptr sketchyGuyDiffuseD3 = Texture2D::LoadFromFile("images/Dialogue/Dealer1-3.png");
		Texture2D::sptr sketchyGuyDiffuseD4 = Texture2D::LoadFromFile("images/Dialogue/Dealer1-4.png");
		Texture2D::sptr sketchyGuyDiffuseD5 = Texture2D::LoadFromFile("images/Dialogue/Dealer1-5.png");

		Texture2D::sptr sketchyGuy2DiffuseD1 = Texture2D::LoadFromFile("images/Dialogue/Dealer2-1.png");
		Texture2D::sptr sketchyGuy2DiffuseD2 = Texture2D::LoadFromFile("images/Dialogue/Dealer2-2.png");
		Texture2D::sptr sketchyGuy2DiffuseD3 = Texture2D::LoadFromFile("images/Dialogue/Dealer2-3.png");
		Texture2D::sptr sketchyGuy2DiffuseD4 = Texture2D::LoadFromFile("images/Dialogue/Dealer2-4.png");
		Texture2D::sptr sketchyGuy2DiffuseD5 = Texture2D::LoadFromFile("images/Dialogue/Dealer2-5.png");
		Texture2D::sptr sketchyGuy2DiffuseD6 = Texture2D::LoadFromFile("images/Dialogue/Dealer2-6.png");

		Texture2D::sptr quizDiffuseD1 = Texture2D::LoadFromFile("images/Dialogue/Quiz1.png");
		Texture2D::sptr quizDiffuseD2 = Texture2D::LoadFromFile("images/Dialogue/Quiz2.png");
		Texture2D::sptr quizDiffuseD3 = Texture2D::LoadFromFile("images/Dialogue/Quiz3.png");

		Texture2D::sptr confirmDiffuse = Texture2D::LoadFromFile("images/Dialogue/Confirm.png");
		Texture2D::sptr numberDiffuse = Texture2D::LoadFromFile("images/Dialogue/Number.png");
		Texture2D::sptr letterDiffuse = Texture2D::LoadFromFile("images/Dialogue/Letter.png");
		Texture2D::sptr colorDiffuse = Texture2D::LoadFromFile("images/Dialogue/Color.png");

		Texture2D::sptr quizSelectDiffuse = Texture2D::LoadFromFile("images/Dialogue/QuizSelect.png");
		Texture2D::sptr selectObjectDiffuse = Texture2D::LoadFromFile("images/Dialogue/SelectObject.png");
		Texture2D::sptr swapObjectDiffuse = Texture2D::LoadFromFile("images/Dialogue/SwapObject.png");

		Texture2D::sptr aftermathDiffuseD1 = Texture2D::LoadFromFile("images/Dialogue/Aftermath1.png");
		Texture2D::sptr aftermathDiffuseD2 = Texture2D::LoadFromFile("images/Dialogue/Aftermath2.png");
		Texture2D::sptr aftermathDiffuseD3 = Texture2D::LoadFromFile("images/Dialogue/Aftermath3.png");
		Texture2D::sptr aftermathDiffuseD4 = Texture2D::LoadFromFile("images/Dialogue/Aftermath4.png");
		Texture2D::sptr aftermathDiffuseD5 = Texture2D::LoadFromFile("images/Dialogue/Aftermath5.png");
		Texture2D::sptr aftermathDiffuseD6 = Texture2D::LoadFromFile("images/Dialogue/Aftermath6.png");


		Texture2D::sptr aquaSnowmanTextDiffuse = Texture2D::LoadFromFile("images/Dialogue/AquaSnowmanText.png");
		Texture2D::sptr blueSnowmanTextDiffuse = Texture2D::LoadFromFile("images/Dialogue/BlueSnowmanText.png");
		Texture2D::sptr brownSnowmanTextDiffuse = Texture2D::LoadFromFile("images/Dialogue/BrownSnowmanText.png");
		Texture2D::sptr greenSnowmanTextDiffuse = Texture2D::LoadFromFile("images/Dialogue/GreenSnowmanText.png");
		Texture2D::sptr orangeSnowmanTextDiffuse = Texture2D::LoadFromFile("images/Dialogue/OrangeSnowmanText.png");
		Texture2D::sptr pinkSnowmanTextDiffuse = Texture2D::LoadFromFile("images/Dialogue/PinkSnowmanText.png");
		Texture2D::sptr purpleSnowmanTextDiffuse = Texture2D::LoadFromFile("images/Dialogue/PurpleSnowmanText.png");
		Texture2D::sptr redSnowmanTextDiffuse = Texture2D::LoadFromFile("images/Dialogue/RedSnowmanText.png");
		Texture2D::sptr yellowSnowmanTextDiffuse = Texture2D::LoadFromFile("images/Dialogue/YellowSnowmanText.png");

		//Sorting Objects Textures
		Texture2D::sptr bananasDiffuse = Texture2D::LoadFromFile("images/Banana_Texture.png");
		Texture2D::sptr appleDiffuse = Texture2D::LoadFromFile("images/Apples_Texture.png");
		Texture2D::sptr carrotDiffuse = Texture2D::LoadFromFile("images/Carrots_Texture.png");
		Texture2D::sptr greenGrapeDiffuse = Texture2D::LoadFromFile("images/GreenGrapes_Texture.png");
		Texture2D::sptr purpleGrapeDiffuse = Texture2D::LoadFromFile("images/PurpleGrapes_Texture.png");
		Texture2D::sptr blueDonutDiffuse = Texture2D::LoadFromFile("images/Doughnut_Texture.png");

		//Scene 0 Menu Sprites
		Texture2D::sptr playMenuDiffuse = Texture2D::LoadFromFile("images/SkyboundMenuPlay.png");
		Texture2D::sptr controlsMenuDiffuse = Texture2D::LoadFromFile("images/SkyboundMenuControls.png");
		Texture2D::sptr exitMenuDiffuse = Texture2D::LoadFromFile("images/SkyboundMenuExit.png");

		//Death Scene Sprites
		Texture2D::sptr deathMenuDiffuse = Texture2D::LoadFromFile("images/YouDied.png");

		Texture2D::sptr controlsDiffuse = Texture2D::LoadFromFile("images/SkyboundControls.png");

		Texture2D::sptr story1Diffuse = Texture2D::LoadFromFile("images/StoryImage1.png");
		Texture2D::sptr story2Diffuse = Texture2D::LoadFromFile("images/StoryImage2.png");
		Texture2D::sptr story3Diffuse = Texture2D::LoadFromFile("images/StoryImage3.png");

		Texture2D::sptr diffuseRamp = Texture2D::LoadFromFile("images/DiffuseRamp.png");
		Texture2D::sptr specularRamp = Texture2D::LoadFromFile("images/SpecularRamp.png");

		LUT3D testCube("cubes/test.cube");

		// Load the cube map
		//TextureCubeMap::sptr environmentMap = TextureCubeMap::LoadFromImages("images/cubemaps/skybox/sample.jpg");
		TextureCubeMap::sptr environmentMap = TextureCubeMap::LoadFromImages("images/cubemaps/skybox/ToonSky.jpg"); 

		// Creating an empty texture
		Texture2DDescription desc = Texture2DDescription();  
		desc.Width = 1;
		desc.Height = 1;
		desc.Format = InternalFormat::RGB8;
		Texture2D::sptr texture2 = Texture2D::Create(desc);
		// Clear it with a white colour
		texture2->Clear();

		// Creating an empty texture
		Texture2DDescription desc2 = Texture2DDescription();
		desc2.Width = 1;
		desc2.Height = 1;
		desc2.Format = InternalFormat::RGB8;
		Texture2D::sptr blank = Texture2D::Create(desc2);

		#pragma region Solid Coloured Textures

		// Creating an red texture
		Texture2DDescription desc3 = Texture2DDescription();
		desc3.Width = 1;
		desc3.Height = 1;
		desc3.Format = InternalFormat::RGB8;
		Texture2D::sptr redTexture = Texture2D::Create(desc3);
		redTexture->Clear(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		// Creating an green texture
		Texture2DDescription desc4 = Texture2DDescription();
		desc4.Width = 1;
		desc4.Height = 1;
		desc4.Format = InternalFormat::RGB8;
		Texture2D::sptr greenTexture = Texture2D::Create(desc4);
		greenTexture->Clear(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		// Creating an blue texture
		Texture2DDescription desc5 = Texture2DDescription();
		desc5.Width = 1;
		desc5.Height = 1;
		desc5.Format = InternalFormat::RGB8;
		Texture2D::sptr blueTexture = Texture2D::Create(desc5);
		blueTexture->Clear(glm::vec4(0.18f, 0.219f, 1.0f, 1.0f));

		// Creating an purple texture
		Texture2DDescription desc6 = Texture2DDescription();
		desc6.Width = 1;
		desc6.Height = 1;
		desc6.Format = InternalFormat::RGB8;
		Texture2D::sptr purpleTexture = Texture2D::Create(desc6);
		purpleTexture->Clear(glm::vec4(0.784f, 0.18f, 1.0f, 1.0f));

		// Creating an orange texture
		Texture2DDescription desc7 = Texture2DDescription();
		desc7.Width = 1;
		desc7.Height = 1;
		desc7.Format = InternalFormat::RGB8;
		Texture2D::sptr orangeTexture = Texture2D::Create(desc7);
		orangeTexture->Clear(glm::vec4(1.0f, 0.49f, 0.141f, 1.0f));

		// Creating an pink texture
		Texture2DDescription desc8 = Texture2DDescription();
		desc8.Width = 1;
		desc8.Height = 1;
		desc8.Format = InternalFormat::RGB8;
		Texture2D::sptr pinkTexture = Texture2D::Create(desc8);
		pinkTexture->Clear(glm::vec4(1.0f, 0.36f, 0.78f, 1.0f));

		// Creating an yellow texture
		Texture2DDescription desc9 = Texture2DDescription();
		desc9.Width = 1;
		desc9.Height = 1;
		desc9.Format = InternalFormat::RGB8;
		Texture2D::sptr yellowTexture = Texture2D::Create(desc9);
		yellowTexture->Clear(glm::vec4(0.976f, 0.964f, 0.101f, 1.0f));

		// Creating an aqua texture
		Texture2DDescription desc10 = Texture2DDescription();
		desc10.Width = 1;
		desc10.Height = 1;
		desc10.Format = InternalFormat::RGB8;
		Texture2D::sptr aquaTexture = Texture2D::Create(desc10);
		aquaTexture->Clear(glm::vec4(0.121f, 1.0f, 0.992f, 1.0f));

		// Creating an brown texture
		Texture2DDescription desc11 = Texture2DDescription();
		desc11.Width = 1;
		desc11.Height = 1;
		desc11.Format = InternalFormat::RGB8;
		Texture2D::sptr brownTexture = Texture2D::Create(desc11);
		brownTexture->Clear(glm::vec4(0.588f, 0.345f, 0.329f, 1.0f));

		#pragma endregion

		#pragma endregion

		 
		///////////////////////////////////// Scene 1 Generation //////////////////////////////////////////////////
		#pragma region Scene 1 Generation
		
		// We need to tell our scene system what extra component types we want to support
		GameScene::RegisterComponentType<RendererComponent>();
		GameScene::RegisterComponentType<BehaviourBinding>();
		GameScene::RegisterComponentType<Camera>();

		// Create a scene, and set it to be the active scene in the application
		GameScene::sptr scene = GameScene::Create("Scene 1");

		// We can create a group ahead of time to make iterating on the group faster
		entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<Transform>, RendererComponent> renderGroup =
			scene->Registry().group<RendererComponent>(entt::get_t<Transform>());

		//entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<Transform>, RendererComponent, MorphRenderer> renderGroup =
			//scene->Registry().group<RendererComponent, MorphRenderer>(entt::get_t<Transform>());

		#pragma region Long List of Materials
		
		ShaderMaterial::sptr material0 = ShaderMaterial::Create();
		material0->Shader = morphShader;
		material0->Set("s_Diffuse", PlayerDiffuse);
		//material0->Set("s_Diffuse2", Boxdiffuse2);
		//material0->Set("s_Specular", Boxspecular);
		material0->Set("u_Shininess", 8.0f);
		material0->Set("u_OutlineThickness", 0.53f);
		material0->Set("s_DiffuseRamp", diffuseRamp);
		material0->Set("s_SpecularRamp", specularRamp);
		material0->Set("s_Emissive", blank);
		//material0->Set("u_TextureMix", 0.5f);
		//material0->Set("u_Reflectivity", 0.6f);
		
		ShaderMaterial::sptr material1 = ShaderMaterial::Create();
		material1->Shader = shader;
		material1->Set("s_Diffuse", diffuseMp02);
		material1->Set("u_Shininess", 8.0f);
		material1->Set("u_OutlineThickness", 0.08f);
		material1->Set("s_DiffuseRamp", diffuseRamp);
		material1->Set("s_SpecularRamp", specularRamp);
		
		ShaderMaterial::sptr material2 = ShaderMaterial::Create();
		material2->Shader = morphShader;
		material2->Set("s_Diffuse", diffuseMp03);
		material2->Set("u_Shininess", 8.0f);
		material2->Set("u_OutlineThickness", 0.5f);
		material2->Set("s_DiffuseRamp", diffuseRamp);
		material2->Set("s_SpecularRamp", specularRamp);
		material2->Set("s_Emissive", blank);
		
		ShaderMaterial::sptr material3 = ShaderMaterial::Create();
		material3->Shader = morphShader;
		material3->Set("s_Diffuse", diffuseMp04);
		material3->Set("u_Shininess", 8.0f);
		material3->Set("u_OutlineThickness", 0.5f);
		material3->Set("s_DiffuseRamp", diffuseRamp);
		material3->Set("s_SpecularRamp", specularRamp);
		material3->Set("s_Emissive", blank);
		
		
		ShaderMaterial::sptr material4 = ShaderMaterial::Create();
		material4->Shader = morphShader;
		material4->Set("s_Diffuse", diffuseMp05);
		material4->Set("u_Shininess", 8.0f);
		material4->Set("u_OutlineThickness", 0.4f);
		material4->Set("s_DiffuseRamp", diffuseRamp);
		material4->Set("s_SpecularRamp", specularRamp);
		material4->Set("s_Emissive", emissiveYellowDiffuse);

		ShaderMaterial::sptr material5 = ShaderMaterial::Create();
		material5->Shader = shader;
		material5->Set("s_Diffuse", diffuseMp06);
		material5->Set("u_Shininess", 8.0f);
		material5->Set("u_OutlineThickness", 0.18f);
		material5->Set("s_DiffuseRamp", diffuseRamp);
		material5->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material6 = ShaderMaterial::Create();
		material6->Shader = shader;
		material6->Set("s_Diffuse", diffuseMp07);
		material6->Set("u_Shininess", 8.0f);
		material6->Set("u_OutlineThickness", 0.22f);
		material6->Set("s_DiffuseRamp", diffuseRamp);
		material6->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material7 = ShaderMaterial::Create();
		material7->Shader = shader;
		material7->Set("s_Diffuse", diffuseMp08);
		material7->Set("u_Shininess", 8.0f);
		material7->Set("u_OutlineThickness", 0.08f);
		material7->Set("s_DiffuseRamp", diffuseRamp);
		material7->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material8 = ShaderMaterial::Create();
		material8->Shader = shader;
		material8->Set("s_Diffuse", diffuseMp09);
		material8->Set("u_Shininess", 8.0f);
		material8->Set("u_OutlineThickness", 0.08f);
		material8->Set("s_DiffuseRamp", diffuseRamp);
		material8->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material9 = ShaderMaterial::Create();
		material9->Shader = shader;
		material9->Set("s_Diffuse", diffuseMp10);
		material9->Set("u_Shininess", 8.0f);
		material9->Set("u_OutlineThickness", 0.08f);
		material9->Set("s_DiffuseRamp", diffuseRamp);
		material9->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material10 = ShaderMaterial::Create();
		material10->Shader = shader;
		material10->Set("s_Diffuse", diffuseMp11);
		material10->Set("u_Shininess", 8.0f);
		material10->Set("u_OutlineThickness", 0.08f);
		material10->Set("s_DiffuseRamp", diffuseRamp);
		material10->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material11 = ShaderMaterial::Create();
		material11->Shader = shader;
		material11->Set("s_Diffuse", diffuseMp12);
		material11->Set("u_Shininess", 8.0f);
		material11->Set("u_OutlineThickness", 0.42f);
		material11->Set("s_DiffuseRamp", diffuseRamp);
		material11->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material12 = ShaderMaterial::Create();
		material12->Shader = shader;
		material12->Set("s_Diffuse", diffuseMp13);
		material12->Set("u_Shininess", 8.0f);
		material12->Set("u_OutlineThickness", 0.12f);
		material12->Set("s_DiffuseRamp", diffuseRamp);
		material12->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material13 = ShaderMaterial::Create();
		material13->Shader = shader;
		material13->Set("s_Diffuse", diffuseMp14);
		material13->Set("u_Shininess", 8.0f);
		material13->Set("u_OutlineThickness", 0.0f);
		material13->Set("s_DiffuseRamp", diffuseRamp);
		material13->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material14 = ShaderMaterial::Create();
		material14->Shader = shader;
		material14->Set("s_Diffuse", diffuseMp15);
		material14->Set("u_Shininess", 8.0f);
		material14->Set("u_OutlineThickness", 0.15f);
		material14->Set("s_DiffuseRamp", diffuseRamp);
		material14->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material15 = ShaderMaterial::Create();
		material15->Shader = shader;
		material15->Set("s_Diffuse", diffuseMp16);
		material15->Set("u_Shininess", 8.0f);
		material15->Set("u_OutlineThickness", 0.35f);
		material15->Set("s_DiffuseRamp", diffuseRamp);
		material15->Set("s_SpecularRamp", specularRamp);
		
		ShaderMaterial::sptr material16 = ShaderMaterial::Create();
		material16->Shader = shader;
		material16->Set("s_Diffuse", diffuseMp17);
		material16->Set("u_Shininess", 8.0f);
		material16->Set("u_OutlineThickness", 0.15f); //0.15f
		material16->Set("s_DiffuseRamp", diffuseRamp);
		material16->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material17 = ShaderMaterial::Create();
		material17->Shader = shader;
		material17->Set("s_Diffuse", diffuseMp18);
		material17->Set("u_Shininess", 8.0f);
		material17->Set("u_OutlineThickness", 0.3f);
		material17->Set("s_DiffuseRamp", diffuseRamp);
		material17->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material18 = ShaderMaterial::Create();
		material18->Shader = shader;
		material18->Set("s_Diffuse", diffuseMp19);
		material18->Set("u_Shininess", 8.0f);
		material18->Set("u_OutlineThickness", 0.15f);
		material18->Set("s_DiffuseRamp", diffuseRamp);
		material18->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material19 = ShaderMaterial::Create();
		material19->Shader = shader;
		material19->Set("s_Diffuse", diffuseMp20);
		material19->Set("u_Shininess", 8.0f);
		material19->Set("u_OutlineThickness", 0.15f);
		material19->Set("s_DiffuseRamp", diffuseRamp);
		material19->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material20 = ShaderMaterial::Create();
		material20->Shader = shader;
		material20->Set("s_Diffuse", diffuseMp21);
		material20->Set("u_Shininess", 8.0f);
		material20->Set("u_OutlineThickness", 0.15f);
		material20->Set("s_DiffuseRamp", diffuseRamp);
		material20->Set("s_SpecularRamp", specularRamp);


		//Fire Island Materials
		ShaderMaterial::sptr material21 = ShaderMaterial::Create();
		material21->Shader = shader;
		material21->Set("s_Diffuse", diffuseMp22);
		material21->Set("u_Shininess", 4.0f);
		material21->Set("u_OutlineThickness", 0.15f);
		material21->Set("s_DiffuseRamp", diffuseRamp);
		material21->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material22 = ShaderMaterial::Create();
		material22->Shader = shader;
		material22->Set("s_Diffuse", diffuseMp23);
		material22->Set("u_Shininess", 8.0f);
		material22->Set("u_OutlineThickness", 0.0001f);
		material22->Set("s_DiffuseRamp", diffuseRamp);
		material22->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material23 = ShaderMaterial::Create();
		material23->Shader = shader;
		material23->Set("s_Diffuse", diffuseMp24);
		material23->Set("u_Shininess", 4.0f);
		material23->Set("u_OutlineThickness", 0.0001f);
		material23->Set("s_DiffuseRamp", diffuseRamp);
		material23->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material24 = ShaderMaterial::Create();
		material24->Shader = shader;
		material24->Set("s_Diffuse", diffuseMp25);
		material24->Set("u_Shininess", 4.0f);
		material24->Set("u_OutlineThickness", 0.01f);
		material24->Set("s_DiffuseRamp", diffuseRamp);
		material24->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material25 = ShaderMaterial::Create();
		material25->Shader = shader;
		material25->Set("s_Diffuse", diffuseMp26);
		material25->Set("u_Shininess", 4.0f);
		material25->Set("u_OutlineThickness", 0.1f);
		material25->Set("s_DiffuseRamp", diffuseRamp);
		material25->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material26 = ShaderMaterial::Create();
		material26->Shader = shader;
		material26->Set("s_Diffuse", diffuseMp27);
		material26->Set("u_Shininess", 4.0f);
		material26->Set("u_OutlineThickness", 0.1f);
		material26->Set("s_DiffuseRamp", diffuseRamp);
		material26->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material27 = ShaderMaterial::Create();
		material27->Shader = shader;
		material27->Set("s_Diffuse", diffuseMp28);
		material27->Set("u_Shininess", 4.0f);
		material27->Set("u_OutlineThickness", 0.3f);
		material27->Set("s_DiffuseRamp", diffuseRamp);
		material27->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material28 = ShaderMaterial::Create();
		material28->Shader = shader;
		material28->Set("s_Diffuse", diffuseMp29);
		material28->Set("u_Shininess", 4.0f);
		material28->Set("u_OutlineThickness", 0.1f);
		material28->Set("s_DiffuseRamp", diffuseRamp);
		material28->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material29 = ShaderMaterial::Create();
		material29->Shader = shader;
		material29->Set("s_Diffuse", diffuseMp30);
		material29->Set("u_Shininess", 4.0f);
		material29->Set("u_OutlineThickness", 0.1f);
		material29->Set("s_DiffuseRamp", diffuseRamp);
		material29->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material30 = ShaderMaterial::Create();
		material30->Shader = shader;
		material30->Set("s_Diffuse", diffuseMp31);
		material30->Set("u_Shininess", 0.2f);
		material30->Set("u_OutlineThickness", 0.2f);
		material30->Set("s_DiffuseRamp", diffuseRamp);
		material30->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material31 = ShaderMaterial::Create();
		material31->Shader = shader;
		material31->Set("s_Diffuse", diffuseMp32);
		material31->Set("u_Shininess", 0.2f);
		material31->Set("u_OutlineThickness", 0.2f);
		material31->Set("s_DiffuseRamp", diffuseRamp);
		material31->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material32 = ShaderMaterial::Create();
		material32->Shader = shader;
		material32->Set("s_Diffuse", diffuseMp33);
		material32->Set("u_Shininess", 0.2f);
		material32->Set("u_OutlineThickness", 0.2f);
		material32->Set("s_DiffuseRamp", diffuseRamp);
		material32->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr material33 = ShaderMaterial::Create();
		material33->Shader = shader;
		material33->Set("s_Diffuse", WhiteDiffuse);
		material33->Set("s_DiffuseRamp", diffuseRamp);
		material32->Set("s_SpecularRamp", specularRamp);

		Texture2D::sptr FinalCutScene1Diffuse = Texture2D::LoadFromFile("images/FinalCutScene1.png");
		Texture2D::sptr FinalCutScene2Diffuse = Texture2D::LoadFromFile("images/FinalCutScene2.png");
		Texture2D::sptr FinalCutScene3Diffuse = Texture2D::LoadFromFile("images/FinalCutScene3.png");

		ShaderMaterial::sptr cutsceneMat1 = ShaderMaterial::Create();
		cutsceneMat1->Shader = spriteShader;
		cutsceneMat1->Set("s_Diffuse", FinalCutScene1Diffuse);
		cutsceneMat1->RenderLayer = 3;

		ShaderMaterial::sptr cutsceneMat2 = ShaderMaterial::Create();
		cutsceneMat2->Shader = spriteShader;
		cutsceneMat2->Set("s_Diffuse", FinalCutScene2Diffuse);
		cutsceneMat2->RenderLayer = 3;


		ShaderMaterial::sptr cutsceneMat3 = ShaderMaterial::Create();
		cutsceneMat3->Shader = spriteShader;
		cutsceneMat3->Set("s_Diffuse", FinalCutScene3Diffuse);
		cutsceneMat3->RenderLayer = 3;


		Texture2D::sptr winMenuDiffuse = Texture2D::LoadFromFile("images/YouWin.png");

		ShaderMaterial::sptr winMenuMat = ShaderMaterial::Create();
		winMenuMat->Shader = spriteShader;
		winMenuMat->Set("s_Diffuse", winMenuDiffuse);
		winMenuMat->Set("s_DiffuseRamp", diffuseRamp);
		winMenuMat->Set("s_SpecularRamp", specularRamp);

		////////

		//Heart Mat
		ShaderMaterial::sptr heartMat = ShaderMaterial::Create();
		heartMat->Shader = spriteShader;
		heartMat->Set("s_Diffuse", heartDiffuse);
		heartMat->RenderLayer = 0;



		#pragma region Interaction Mats

		//Interaction Sprites
		ShaderMaterial::sptr interactMat = ShaderMaterial::Create();
		interactMat->Shader = spriteShader;
		interactMat->Set("s_Diffuse", interactDiffuse);
		interactMat->RenderLayer = 0;

		ShaderMaterial::sptr talkMat = ShaderMaterial::Create();
		talkMat->Shader = spriteShader;
		talkMat->Set("s_Diffuse", talkDiffuse);
		talkMat->RenderLayer = 0;

		ShaderMaterial::sptr continueMat = ShaderMaterial::Create();
		continueMat->Shader = spriteShader;
		continueMat->Set("s_Diffuse", continueDiffuse);
		continueMat->RenderLayer = 0;

		#pragma endregion


		#pragma region Dialogue Level 1 Mats

		//Level 1 Dialogue
		ShaderMaterial::sptr dialogueMatL1D1 = ShaderMaterial::Create();
		dialogueMatL1D1->Shader = spriteShader;
		dialogueMatL1D1->Set("s_Diffuse", dialogueDiffuseL1D1);
		dialogueMatL1D1->RenderLayer = 0;

		ShaderMaterial::sptr dialogueMatL1D2 = ShaderMaterial::Create();
		dialogueMatL1D2->Shader = spriteShader;
		dialogueMatL1D2->Set("s_Diffuse", dialogueDiffuseL1D2);
		dialogueMatL1D2->RenderLayer = 0;

		ShaderMaterial::sptr dialogueMatL1D3 = ShaderMaterial::Create();
		dialogueMatL1D3->Shader = spriteShader;
		dialogueMatL1D3->Set("s_Diffuse", dialogueDiffuseL1D3);
		dialogueMatL1D3->RenderLayer = 0;

		ShaderMaterial::sptr dialogueMatL1D4 = ShaderMaterial::Create();
		dialogueMatL1D4->Shader = spriteShader;
		dialogueMatL1D4->Set("s_Diffuse", dialogueDiffuseL1D4);
		dialogueMatL1D4->RenderLayer = 0;

		ShaderMaterial::sptr dialogueMatL1D5 = ShaderMaterial::Create();
		dialogueMatL1D5->Shader = spriteShader;
		dialogueMatL1D5->Set("s_Diffuse", dialogueDiffuseL1D5);
		dialogueMatL1D5->RenderLayer = 0;

		ShaderMaterial::sptr dialogueMatL1D6 = ShaderMaterial::Create();
		dialogueMatL1D6->Shader = spriteShader;
		dialogueMatL1D6->Set("s_Diffuse", dialogueDiffuseL1D6);
		dialogueMatL1D6->RenderLayer = 0;

		ShaderMaterial::sptr dialogueMatL1D7 = ShaderMaterial::Create();
		dialogueMatL1D7->Shader = spriteShader;
		dialogueMatL1D7->Set("s_Diffuse", dialogueDiffuseL1D7);
		dialogueMatL1D7->RenderLayer = 0;

		ShaderMaterial::sptr dialogueMatL1D8 = ShaderMaterial::Create();
		dialogueMatL1D8->Shader = spriteShader;
		dialogueMatL1D8->Set("s_Diffuse", dialogueDiffuseL1D8);
		dialogueMatL1D8->RenderLayer = 0;

		ShaderMaterial::sptr dialogueMatL1D9 = ShaderMaterial::Create();
		dialogueMatL1D9->Shader = spriteShader;
		dialogueMatL1D9->Set("s_Diffuse", dialogueDiffuseL1D9);
		dialogueMatL1D9->RenderLayer = 0;

		ShaderMaterial::sptr dialogueMatL1D10 = ShaderMaterial::Create();
		dialogueMatL1D10->Shader = spriteShader;
		dialogueMatL1D10->Set("s_Diffuse", dialogueDiffuseL1D10);
		dialogueMatL1D10->RenderLayer = 0;

		ShaderMaterial::sptr dialogueMatL1D11 = ShaderMaterial::Create();
		dialogueMatL1D11->Shader = spriteShader;
		dialogueMatL1D11->Set("s_Diffuse", dialogueDiffuseL1D11);
		dialogueMatL1D11->RenderLayer = 0;

		#pragma endregion


		#pragma region Dialogue Level 2 Mats

		//Level 2 Dialogue
		ShaderMaterial::sptr dialogueMatL2D1 = ShaderMaterial::Create();
		dialogueMatL2D1->Shader = spriteShader;
		dialogueMatL2D1->Set("s_Diffuse", dialogueDiffuseL2D1);
		dialogueMatL2D1->RenderLayer = 0;

		ShaderMaterial::sptr dialogueMatL2D2 = ShaderMaterial::Create();
		dialogueMatL2D2->Shader = spriteShader;
		dialogueMatL2D2->Set("s_Diffuse", dialogueDiffuseL2D2);
		dialogueMatL2D2->RenderLayer = 0;

		ShaderMaterial::sptr dialogueMatL2D3 = ShaderMaterial::Create();
		dialogueMatL2D3->Shader = spriteShader;
		dialogueMatL2D3->Set("s_Diffuse", dialogueDiffuseL2D3);
		dialogueMatL2D3->RenderLayer = 0;

		ShaderMaterial::sptr dialogueMatL2D4 = ShaderMaterial::Create();
		dialogueMatL2D4->Shader = spriteShader;
		dialogueMatL2D4->Set("s_Diffuse", dialogueDiffuseL2D4);
		dialogueMatL2D4->RenderLayer = 0;

		//Sketchy Guy Dialogue 1
		ShaderMaterial::sptr sketchyGuy1Mat1 = ShaderMaterial::Create();
		sketchyGuy1Mat1->Shader = spriteShader;
		sketchyGuy1Mat1->Set("s_Diffuse", sketchyGuyDiffuseD1);
		sketchyGuy1Mat1->RenderLayer = 0;

		ShaderMaterial::sptr sketchyGuy1Mat2 = ShaderMaterial::Create();
		sketchyGuy1Mat2->Shader = spriteShader;
		sketchyGuy1Mat2->Set("s_Diffuse", sketchyGuyDiffuseD2);
		sketchyGuy1Mat2->RenderLayer = 0;

		ShaderMaterial::sptr sketchyGuy1Mat3 = ShaderMaterial::Create();
		sketchyGuy1Mat3->Shader = spriteShader;
		sketchyGuy1Mat3->Set("s_Diffuse", sketchyGuyDiffuseD3);
		sketchyGuy1Mat3->RenderLayer = 0;

		ShaderMaterial::sptr sketchyGuy1Mat4 = ShaderMaterial::Create();
		sketchyGuy1Mat4->Shader = spriteShader;
		sketchyGuy1Mat4->Set("s_Diffuse", sketchyGuyDiffuseD4);
		sketchyGuy1Mat4->RenderLayer = 0;

		ShaderMaterial::sptr sketchyGuy1Mat5 = ShaderMaterial::Create();
		sketchyGuy1Mat5->Shader = spriteShader;
		sketchyGuy1Mat5->Set("s_Diffuse", sketchyGuyDiffuseD5);
		sketchyGuy1Mat5->RenderLayer = 0;

		//Sketchy Guy 2 Dialogue
		ShaderMaterial::sptr sketchyGuy2Mat1 = ShaderMaterial::Create();
		sketchyGuy2Mat1->Shader = spriteShader;
		sketchyGuy2Mat1->Set("s_Diffuse", sketchyGuy2DiffuseD1);
		sketchyGuy2Mat1->RenderLayer = 0;

		ShaderMaterial::sptr sketchyGuy2Mat2 = ShaderMaterial::Create();
		sketchyGuy2Mat2->Shader = spriteShader;
		sketchyGuy2Mat2->Set("s_Diffuse", sketchyGuy2DiffuseD2);
		sketchyGuy2Mat2->RenderLayer = 0;

		ShaderMaterial::sptr sketchyGuy2Mat3 = ShaderMaterial::Create();
		sketchyGuy2Mat3->Shader = spriteShader;
		sketchyGuy2Mat3->Set("s_Diffuse", sketchyGuy2DiffuseD3);
		sketchyGuy2Mat3->RenderLayer = 0;

		ShaderMaterial::sptr sketchyGuy2Mat4 = ShaderMaterial::Create();
		sketchyGuy2Mat4->Shader = spriteShader;
		sketchyGuy2Mat4->Set("s_Diffuse", sketchyGuy2DiffuseD4);
		sketchyGuy2Mat4->RenderLayer = 0;

		ShaderMaterial::sptr sketchyGuy2Mat5 = ShaderMaterial::Create();
		sketchyGuy2Mat5->Shader = spriteShader;
		sketchyGuy2Mat5->Set("s_Diffuse", sketchyGuy2DiffuseD5);
		sketchyGuy2Mat5->RenderLayer = 0;

		ShaderMaterial::sptr sketchyGuy2Mat6 = ShaderMaterial::Create();
		sketchyGuy2Mat6->Shader = spriteShader;
		sketchyGuy2Mat6->Set("s_Diffuse", sketchyGuy2DiffuseD6);
		sketchyGuy2Mat6->RenderLayer = 0;

		//Quiz
		ShaderMaterial::sptr quizMat1 = ShaderMaterial::Create();
		quizMat1->Shader = spriteShader;
		quizMat1->Set("s_Diffuse", quizDiffuseD1);
		quizMat1->RenderLayer = 0;

		ShaderMaterial::sptr quizMat2 = ShaderMaterial::Create();
		quizMat2->Shader = spriteShader;
		quizMat2->Set("s_Diffuse", quizDiffuseD2);
		quizMat2->RenderLayer = 0;

		ShaderMaterial::sptr quizMat3 = ShaderMaterial::Create();
		quizMat3->Shader = spriteShader;
		quizMat3->Set("s_Diffuse", quizDiffuseD3);
		quizMat3->RenderLayer = 0;

		//Aftermath
		ShaderMaterial::sptr afterMathMat1 = ShaderMaterial::Create();
		afterMathMat1->Shader = spriteShader;
		afterMathMat1->Set("s_Diffuse", aftermathDiffuseD1);
		afterMathMat1->RenderLayer = 0;

		ShaderMaterial::sptr afterMathMat2 = ShaderMaterial::Create();
		afterMathMat2->Shader = spriteShader;
		afterMathMat2->Set("s_Diffuse", aftermathDiffuseD2);
		afterMathMat2->RenderLayer = 0;

		ShaderMaterial::sptr afterMathMat3 = ShaderMaterial::Create();
		afterMathMat3->Shader = spriteShader;
		afterMathMat3->Set("s_Diffuse", aftermathDiffuseD3);
		afterMathMat3->RenderLayer = 0;

		ShaderMaterial::sptr afterMathMat4 = ShaderMaterial::Create();
		afterMathMat4->Shader = spriteShader;
		afterMathMat4->Set("s_Diffuse", aftermathDiffuseD4);
		afterMathMat4->RenderLayer = 0;

		ShaderMaterial::sptr afterMathMat5 = ShaderMaterial::Create();
		afterMathMat5->Shader = spriteShader;
		afterMathMat5->Set("s_Diffuse", aftermathDiffuseD5);
		afterMathMat5->RenderLayer = 0;

		ShaderMaterial::sptr afterMathMat6 = ShaderMaterial::Create();
		afterMathMat6->Shader = spriteShader;
		afterMathMat6->Set("s_Diffuse", aftermathDiffuseD6);
		afterMathMat6->RenderLayer = 0;

		//Interactions - Select/Swap
		ShaderMaterial::sptr quizSelectMat = ShaderMaterial::Create();
		quizSelectMat->Shader = spriteShader;
		quizSelectMat->Set("s_Diffuse", quizSelectDiffuse);
		quizSelectMat->RenderLayer = 0;

		ShaderMaterial::sptr selectObjectMat = ShaderMaterial::Create();
		selectObjectMat->Shader = spriteShader;
		selectObjectMat->Set("s_Diffuse", selectObjectDiffuse);
		selectObjectMat->RenderLayer = 0;

		ShaderMaterial::sptr swapObjectMat = ShaderMaterial::Create();
		swapObjectMat->Shader = spriteShader;
		swapObjectMat->Set("s_Diffuse", swapObjectDiffuse);
		swapObjectMat->RenderLayer = 0;

		ShaderMaterial::sptr confirmMat = ShaderMaterial::Create();
		confirmMat->Shader = spriteShader;
		confirmMat->Set("s_Diffuse", confirmDiffuse);
		confirmMat->RenderLayer = 0;

		ShaderMaterial::sptr numberMat = ShaderMaterial::Create();
		numberMat->Shader = spriteShader;
		numberMat->Set("s_Diffuse", numberDiffuse);
		numberMat->RenderLayer = 0;

		ShaderMaterial::sptr letterMat = ShaderMaterial::Create();
		letterMat->Shader = spriteShader;
		letterMat->Set("s_Diffuse", letterDiffuse);
		letterMat->RenderLayer = 0;

		ShaderMaterial::sptr colorMat = ShaderMaterial::Create();
		colorMat->Shader = spriteShader;
		colorMat->Set("s_Diffuse", colorDiffuse);
		colorMat->RenderLayer = 0;

		#pragma endregion


		#pragma region Snowmen Text Mats

		ShaderMaterial::sptr aquaSnowmanTextMat = ShaderMaterial::Create();
		aquaSnowmanTextMat->Shader = spriteShader;
		aquaSnowmanTextMat->Set("s_Diffuse", aquaSnowmanTextDiffuse);
		aquaSnowmanTextMat->RenderLayer = 0;

		ShaderMaterial::sptr blueSnowmanTextMat = ShaderMaterial::Create();
		blueSnowmanTextMat->Shader = spriteShader;
		blueSnowmanTextMat->Set("s_Diffuse", blueSnowmanTextDiffuse);
		blueSnowmanTextMat->RenderLayer = 0;

		ShaderMaterial::sptr brownSnowmanTextMat = ShaderMaterial::Create();
		brownSnowmanTextMat->Shader = spriteShader;
		brownSnowmanTextMat->Set("s_Diffuse", brownSnowmanTextDiffuse);
		brownSnowmanTextMat->RenderLayer = 0;

		ShaderMaterial::sptr greenSnowmanTextMat = ShaderMaterial::Create();
		greenSnowmanTextMat->Shader = spriteShader;
		greenSnowmanTextMat->Set("s_Diffuse", greenSnowmanTextDiffuse);
		greenSnowmanTextMat->RenderLayer = 0;

		ShaderMaterial::sptr orangeSnowmanTextMat = ShaderMaterial::Create();
		orangeSnowmanTextMat->Shader = spriteShader;
		orangeSnowmanTextMat->Set("s_Diffuse", orangeSnowmanTextDiffuse);
		orangeSnowmanTextMat->RenderLayer = 0;

		ShaderMaterial::sptr pinkSnowmanTextMat = ShaderMaterial::Create();
		pinkSnowmanTextMat->Shader = spriteShader;
		pinkSnowmanTextMat->Set("s_Diffuse", pinkSnowmanTextDiffuse);
		pinkSnowmanTextMat->RenderLayer = 0;

		ShaderMaterial::sptr purpleSnowmanTextMat = ShaderMaterial::Create();
		purpleSnowmanTextMat->Shader = spriteShader;
		purpleSnowmanTextMat->Set("s_Diffuse", purpleSnowmanTextDiffuse);
		purpleSnowmanTextMat->RenderLayer = 0;

		ShaderMaterial::sptr redSnowmanTextMat = ShaderMaterial::Create();
		redSnowmanTextMat->Shader = spriteShader;
		redSnowmanTextMat->Set("s_Diffuse", redSnowmanTextDiffuse);
		redSnowmanTextMat->RenderLayer = 0;

		ShaderMaterial::sptr yellowSnowmanTextMat = ShaderMaterial::Create();
		yellowSnowmanTextMat->Shader = spriteShader;
		yellowSnowmanTextMat->Set("s_Diffuse", yellowSnowmanTextDiffuse);
		yellowSnowmanTextMat->RenderLayer = 0;

		#pragma endregion


		#pragma endregion


		#pragma region Short List of Materials & Snowmen Materials + New Materials

		// Create a material and set some properties for it
		ShaderMaterial::sptr stoneMat = ShaderMaterial::Create();
		stoneMat->Shader = shader;
		stoneMat->Set("s_Diffuse", stone);
		stoneMat->Set("s_Specular", stoneSpec);
		stoneMat->Set("u_Shininess", 2.0f);
		stoneMat->Set("u_TextureMix", 0.0f);

		ShaderMaterial::sptr grassMat = ShaderMaterial::Create();
		grassMat->Shader = shader;
		grassMat->Set("s_Diffuse", grass);
		grassMat->Set("s_Specular", noSpec);
		grassMat->Set("u_Shininess", 2.0f);
		grassMat->Set("u_TextureMix", 0.0f);

		ShaderMaterial::sptr boxMat = ShaderMaterial::Create();
		boxMat->Shader = shader;
		boxMat->Set("s_Diffuse", box);
		boxMat->Set("s_Specular", boxSpec);
		boxMat->Set("u_Shininess", 8.0f);
		boxMat->Set("u_TextureMix", 0.0f);

		ShaderMaterial::sptr simpleFloraMat = ShaderMaterial::Create();
		simpleFloraMat->Shader = shader;
		simpleFloraMat->Set("s_Diffuse", simpleFlora);
		simpleFloraMat->Set("s_Specular", noSpec);
		simpleFloraMat->Set("u_Shininess", 8.0f);
		simpleFloraMat->Set("u_TextureMix", 0.0f);

		ShaderMaterial::sptr parkingMeterMat = ShaderMaterial::Create();
		parkingMeterMat->Shader = morphShader;
		parkingMeterMat->Set("s_Diffuse", parkingMeterDiffuse);
		parkingMeterMat->Set("u_OutlineThickness", 0.53f);
		parkingMeterMat->Set("s_DiffuseRamp", diffuseRamp);
		parkingMeterMat->Set("s_SpecularRamp", specularRamp);
		parkingMeterMat->Set("s_Emissive", blank);

		ShaderMaterial::sptr parkingMeterPoleMat = ShaderMaterial::Create();
		parkingMeterPoleMat->Shader = morphShader;
		parkingMeterPoleMat->Set("s_Diffuse", parkingMeterPoleDiffuse);
		parkingMeterPoleMat->Set("u_OutlineThickness", 0.53f);
		parkingMeterPoleMat->Set("s_DiffuseRamp", diffuseRamp);
		parkingMeterPoleMat->Set("s_SpecularRamp", specularRamp);
		parkingMeterPoleMat->Set("s_Emissive", blank);

		ShaderMaterial::sptr snowBlockMat = ShaderMaterial::Create();
		snowBlockMat->Shader = shader;
		snowBlockMat->Set("s_Diffuse", snowBlockDiffuse);
		snowBlockMat->Set("s_Specular", noSpec);
		snowBlockMat->Set("u_Shininess", 2.0f);
		snowBlockMat->Set("u_TextureMix", 0.0f);

		ShaderMaterial::sptr sketchyGuyMat = ShaderMaterial::Create();
		sketchyGuyMat->Shader = morphShader;
		sketchyGuyMat->Set("s_Diffuse", sketchyGuyDiffuse);
		sketchyGuyMat->Set("u_OutlineThickness", 0.53f);
		sketchyGuyMat->Set("s_DiffuseRamp", diffuseRamp);
		sketchyGuyMat->Set("s_SpecularRamp", specularRamp);
		sketchyGuyMat->Set("s_Emissive", blank);

		ShaderMaterial::sptr sketchyGuy2Mat = ShaderMaterial::Create();
		sketchyGuy2Mat->Shader = morphShader;
		sketchyGuy2Mat->Set("s_Diffuse", sketchyGuy2Diffuse);
		sketchyGuy2Mat->Set("u_OutlineThickness", 0.53f);
		sketchyGuy2Mat->Set("s_DiffuseRamp", diffuseRamp);
		sketchyGuy2Mat->Set("s_SpecularRamp", specularRamp);
		sketchyGuy2Mat->Set("s_Emissive", blank);

		ShaderMaterial::sptr buttonMat = ShaderMaterial::Create();
		buttonMat->Shader = morphShader;
		buttonMat->Set("s_Diffuse", buttonDiffuse);
		buttonMat->Set("u_OutlineThickness", 0.53f);
		buttonMat->Set("s_DiffuseRamp", diffuseRamp);
		buttonMat->Set("s_SpecularRamp", specularRamp);
		buttonMat->Set("s_Emissive", blank);

		ShaderMaterial::sptr bananaMat = ShaderMaterial::Create();
		bananaMat->Shader = shader;
		bananaMat->Set("s_Diffuse", bananasDiffuse);
		bananaMat->Set("s_Specular", noSpec);
		bananaMat->Set("u_Shininess", 8.0f);
		bananaMat->Set("u_TextureMix", 0.0f);

		ShaderMaterial::sptr appleMat = ShaderMaterial::Create();
		appleMat->Shader = shader;
		appleMat->Set("s_Diffuse", appleDiffuse);
		appleMat->Set("s_Specular", noSpec);
		appleMat->Set("u_Shininess", 8.0f);
		appleMat->Set("u_TextureMix", 0.0f);

		ShaderMaterial::sptr carrotMat = ShaderMaterial::Create();
		carrotMat->Shader = shader;
		carrotMat->Set("s_Diffuse", carrotDiffuse);
		carrotMat->Set("s_Specular", noSpec);
		carrotMat->Set("u_Shininess", 8.0f);
		carrotMat->Set("u_TextureMix", 0.0f);

		ShaderMaterial::sptr greenGrapeMat = ShaderMaterial::Create();
		greenGrapeMat->Shader = shader;
		greenGrapeMat->Set("s_Diffuse", greenGrapeDiffuse);
		greenGrapeMat->Set("s_Specular", noSpec);
		greenGrapeMat->Set("u_Shininess", 8.0f);
		greenGrapeMat->Set("u_TextureMix", 0.0f);

		ShaderMaterial::sptr purpleGrapeMat = ShaderMaterial::Create();
		purpleGrapeMat->Shader = shader;
		purpleGrapeMat->Set("s_Diffuse", purpleGrapeDiffuse);
		purpleGrapeMat->Set("s_Specular", noSpec);
		purpleGrapeMat->Set("u_Shininess", 8.0f);
		purpleGrapeMat->Set("u_TextureMix", 0.0f);

		ShaderMaterial::sptr blueDonutMat = ShaderMaterial::Create();
		blueDonutMat->Shader = shader;
		blueDonutMat->Set("s_Diffuse", blueDonutDiffuse);
		blueDonutMat->Set("s_Specular", noSpec);
		blueDonutMat->Set("u_Shininess", 8.0f);
		blueDonutMat->Set("u_TextureMix", 0.0f);

		#pragma region Snowmen Materials

		//Row 1

		ShaderMaterial::sptr snowR1C1Mat = ShaderMaterial::Create();
		snowR1C1Mat->Shader = shader;
		snowR1C1Mat->Set("s_Diffuse", diffuseMp28);
		snowR1C1Mat->Set("s_Diffuse2", redTexture);
		snowR1C1Mat->Set("u_TextureMix", 0.8f);
		snowR1C1Mat->Set("u_Shininess", 4.0f);
		snowR1C1Mat->Set("u_OutlineThickness", 0.3f);
		snowR1C1Mat->Set("s_DiffuseRamp", diffuseRamp);
		snowR1C1Mat->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr snowR1C2Mat = ShaderMaterial::Create();
		snowR1C2Mat->Shader = shader;
		snowR1C2Mat->Set("s_Diffuse", diffuseMp28);
		snowR1C2Mat->Set("s_Diffuse2", greenTexture);
		snowR1C2Mat->Set("u_TextureMix", 0.8f);
		snowR1C2Mat->Set("u_Shininess", 4.0f);
		snowR1C2Mat->Set("u_OutlineThickness", 0.3f);
		snowR1C2Mat->Set("s_DiffuseRamp", diffuseRamp);
		snowR1C2Mat->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr snowR1C3Mat = ShaderMaterial::Create();
		snowR1C3Mat->Shader = shader;
		snowR1C3Mat->Set("s_Diffuse", diffuseMp28);
		snowR1C3Mat->Set("s_Diffuse2", blueTexture);
		snowR1C3Mat->Set("u_TextureMix", 0.8f);
		snowR1C3Mat->Set("u_Shininess", 4.0f);
		snowR1C3Mat->Set("u_OutlineThickness", 0.3f);
		snowR1C3Mat->Set("s_DiffuseRamp", diffuseRamp);
		snowR1C3Mat->Set("s_SpecularRamp", specularRamp);

		//Row 2

		ShaderMaterial::sptr snowR2C1Mat = ShaderMaterial::Create();
		snowR2C1Mat->Shader = shader;
		snowR2C1Mat->Set("s_Diffuse", diffuseMp28);
		snowR2C1Mat->Set("s_Diffuse2", brownTexture);
		snowR2C1Mat->Set("u_TextureMix", 0.8f);
		snowR2C1Mat->Set("u_Shininess", 4.0f);
		snowR2C1Mat->Set("u_OutlineThickness", 0.3f);
		snowR2C1Mat->Set("s_DiffuseRamp", diffuseRamp);
		snowR2C1Mat->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr snowR2C2Mat = ShaderMaterial::Create();
		snowR2C2Mat->Shader = shader;
		snowR2C2Mat->Set("s_Diffuse", diffuseMp28);
		snowR2C2Mat->Set("s_Diffuse2", purpleTexture);
		snowR2C2Mat->Set("u_TextureMix", 0.8f);
		snowR2C2Mat->Set("u_Shininess", 4.0f);
		snowR2C2Mat->Set("u_OutlineThickness", 0.3f);
		snowR2C2Mat->Set("s_DiffuseRamp", diffuseRamp);
		snowR2C2Mat->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr snowR2C3Mat = ShaderMaterial::Create();
		snowR2C3Mat->Shader = shader;
		snowR2C3Mat->Set("s_Diffuse", diffuseMp28);
		snowR2C3Mat->Set("s_Diffuse2", orangeTexture);
		snowR2C3Mat->Set("u_TextureMix", 0.8f);
		snowR2C3Mat->Set("u_Shininess", 4.0f);
		snowR2C3Mat->Set("u_OutlineThickness", 0.3f);
		snowR2C3Mat->Set("s_DiffuseRamp", diffuseRamp);
		snowR2C3Mat->Set("s_SpecularRamp", specularRamp);

		//Row 3

		ShaderMaterial::sptr snowR3C1Mat = ShaderMaterial::Create();
		snowR3C1Mat->Shader = shader;
		snowR3C1Mat->Set("s_Diffuse", diffuseMp28);
		snowR3C1Mat->Set("s_Diffuse2", yellowTexture);
		snowR3C1Mat->Set("u_TextureMix", 0.8f);
		snowR3C1Mat->Set("u_Shininess", 4.0f);
		snowR3C1Mat->Set("u_OutlineThickness", 0.3f);
		snowR3C1Mat->Set("s_DiffuseRamp", diffuseRamp);
		snowR3C1Mat->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr snowR3C2Mat = ShaderMaterial::Create();
		snowR3C2Mat->Shader = shader;
		snowR3C2Mat->Set("s_Diffuse", diffuseMp28);
		snowR3C2Mat->Set("s_Diffuse2", aquaTexture);
		snowR3C2Mat->Set("u_TextureMix", 0.8f);
		snowR3C2Mat->Set("u_Shininess", 4.0f);
		snowR3C2Mat->Set("u_OutlineThickness", 0.3f);
		snowR3C2Mat->Set("s_DiffuseRamp", diffuseRamp);
		snowR3C2Mat->Set("s_SpecularRamp", specularRamp);

		ShaderMaterial::sptr snowR3C3Mat = ShaderMaterial::Create();
		snowR3C3Mat->Shader = shader;
		snowR3C3Mat->Set("s_Diffuse", diffuseMp28);
		snowR3C3Mat->Set("s_Diffuse2", pinkTexture);
		snowR3C3Mat->Set("u_TextureMix", 0.9f);
		snowR3C3Mat->Set("u_Shininess", 4.0f);
		snowR3C3Mat->Set("u_OutlineThickness", 0.3f);
		snowR3C3Mat->Set("s_DiffuseRamp", diffuseRamp);
		snowR3C3Mat->Set("s_SpecularRamp", specularRamp);

		#pragma endregion

		#pragma endregion

		//X = In and Out
		//Y = Left and Right
		//Z = up and down

		#pragma region Environment Generation

		/*GameObject obj1 = scene->CreateEntity("Ground");
		{
			VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("models/plane.obj");
			obj1.emplace<RendererComponent>().SetMesh(vao).SetMaterial(grassMat);
		}

		GameObject obj2 = scene->CreateEntity("monkey_quads");
		{
			VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("models/monkey_quads.obj");
			obj2.emplace<RendererComponent>().SetMesh(vao).SetMaterial(stoneMat);
			obj2.get<Transform>().SetLocalPosition(0.0f, 0.0f, 2.0f);
			obj2.get<Transform>().SetLocalRotation(0.0f, 0.0f, -90.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(obj2);
		}

		std::vector<glm::vec2> allAvoidAreasFrom = { glm::vec2(-4.0f, -4.0f) };
		std::vector<glm::vec2> allAvoidAreasTo = { glm::vec2(4.0f, 4.0f) };

		std::vector<glm::vec2> rockAvoidAreasFrom = { glm::vec2(-3.0f, -3.0f), glm::vec2(-19.0f, -19.0f), glm::vec2(5.0f, -19.0f),
														glm::vec2(-19.0f, 5.0f), glm::vec2(-19.0f, -19.0f) };
		std::vector<glm::vec2> rockAvoidAreasTo = { glm::vec2(3.0f, 3.0f), glm::vec2(19.0f, -5.0f), glm::vec2(19.0f, 19.0f),
														glm::vec2(19.0f, 19.0f), glm::vec2(-5.0f, 19.0f) };
		glm::vec2 spawnFromHere = glm::vec2(-19.0f, -19.0f);
		glm::vec2 spawnToHere = glm::vec2(19.0f, 19.0f);

		EnvironmentGenerator::AddObjectToGeneration("models/simplePine.obj", simpleFloraMat, 150,
			spawnFromHere, spawnToHere, allAvoidAreasFrom, allAvoidAreasTo);
		EnvironmentGenerator::AddObjectToGeneration("models/simpleTree.obj", simpleFloraMat, 150,
			spawnFromHere, spawnToHere, allAvoidAreasFrom, allAvoidAreasTo);
		EnvironmentGenerator::AddObjectToGeneration("models/simpleRock.obj", simpleFloraMat, 40,
			spawnFromHere, spawnToHere, rockAvoidAreasFrom, rockAvoidAreasTo);
		EnvironmentGenerator::GenerateEnvironment();*/

		#pragma endregion


		#pragma region Sprite Objects

		VertexArrayObject::sptr heartVao;

		GameObject heart1Obj = scene->CreateEntity("HeartSprite");
		{
			GLfloat vertices[] = { -1, -1, 0, // bottom left corner
						   -1,  1, 0, // top left corner
							1,  1, 0, // top right corner
							1, -1, 0 }; // bottom right corner


			GLuint elements[] = {
					0, 1, 2,
					2, 3, 0
			};


			heartVao = NotObjLoader::LoadFromFile("sprite.notobj"); 
			heart1Obj.emplace<Sprite>().SetMesh(heartVao).SetMaterial(heartMat);
			//heart1Obj.emplace<RendererComponent>().SetMesh(heart1Vao).SetMaterial(heart1Mat);
			heart1Obj.get<Transform>().SetLocalPosition(0.0f, 0.0f, 5.0f);
			heart1Obj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			//heart1Obj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 180.0f);
			heart1Obj.get<Transform>().SetLocalScale(0.07f, 0.07f, 0.07f);
			//heart1Obj.get<Transform>().LookAt(glm::vec3(0));

		}

		GameObject heart2Obj = scene->CreateEntity("HeartSprite");
		{
			GLfloat vertices[] = { -1, -1, 0, // bottom left corner
						   -1,  1, 0, // top left corner
							1,  1, 0, // top right corner
							1, -1, 0 }; // bottom right corner


			GLuint elements[] = {
					0, 1, 2,
					2, 3, 0
			};


			heartVao = NotObjLoader::LoadFromFile("sprite.notobj");
			heart2Obj.emplace<Sprite>().SetMesh(heartVao).SetMaterial(heartMat);
			//heart1Obj.emplace<RendererComponent>().SetMesh(heart1Vao).SetMaterial(heart1Mat);
			heart2Obj.get<Transform>().SetLocalPosition(0.0f, 0.0f, 5.0f);
			heart2Obj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			//heart1Obj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 180.0f);
			heart2Obj.get<Transform>().SetLocalScale(0.07f, 0.07f, 0.07f);
			//heart1Obj.get<Transform>().LookAt(glm::vec3(0));

		}

		GameObject heart3Obj = scene->CreateEntity("HeartSprite");
		{
			GLfloat vertices[] = { -1, -1, 0, // bottom left corner
						   -1,  1, 0, // top left corner
							1,  1, 0, // top right corner
							1, -1, 0 }; // bottom right corner


			GLuint elements[] = {
					0, 1, 2,
					2, 3, 0
			};


			heartVao = NotObjLoader::LoadFromFile("sprite.notobj");
			heart3Obj.emplace<Sprite>().SetMesh(heartVao).SetMaterial(heartMat);
			//heart1Obj.emplace<RendererComponent>().SetMesh(heart1Vao).SetMaterial(heart1Mat);
			heart3Obj.get<Transform>().SetLocalPosition(0.0f, 0.0f, 5.0f);
			heart3Obj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			//heart1Obj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 180.0f);
			heart3Obj.get<Transform>().SetLocalScale(0.07f, 0.07f, 0.07f);
			//heart1Obj.get<Transform>().LookAt(glm::vec3(0));

		}

		#pragma endregion


		#pragma region Dialogue Sprites

		VertexArrayObject::sptr dialogueLevel1VAO;

		GameObject dialogueL1D1 = scene->CreateEntity("Dialogue Level 1-1");
		{
			dialogueLevel1VAO = NotObjLoader::LoadFromFile("sprite.notobj");
			dialogueL1D1.emplace<Sprite>().SetMesh(dialogueLevel1VAO).SetMaterial(dialogueMatL1D1);
			//dialogueL1D1.emplace<RendererComponent>().SetMesh(heart1Vao).SetMaterial(heart1Mat);
			dialogueL1D1.get<Transform>().SetLocalPosition(0.0f, 0.0f, 5.0f);
			dialogueL1D1.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			//dialogueL1D1.get<Transform>().SetLocalRotation(0.0f, 180.0f, 180.0f);
			dialogueL1D1.get<Transform>().SetLocalScale(0.223f, 0.223f, 0.223f);
			//dialogueL1D1.get<Transform>().LookAt(glm::vec3(0));

		}

		#pragma endregion


		#pragma region Interaction Sprites

		VertexArrayObject::sptr interactionVAO;

		GameObject interactionObj = scene->CreateEntity("Interaction Sprite");
		{
			interactionVAO = NotObjLoader::LoadFromFile("sprite.notobj");
			interactionObj.emplace<Sprite>().SetMesh(interactionVAO).SetMaterial(talkMat);
			interactionObj.get<Transform>().SetLocalPosition(0.0f, 0.0f, 5.0f);
			interactionObj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			interactionObj.get<Transform>().SetLocalScale(0.223f, 0.223f, 0.223f);
		}

		#pragma endregion


		#pragma region Player Object

		//Player Physics
		btCollisionShape* playerShape = new btBoxShape(btVector3(1.1f, 1.1f, 2.4f));

		btCollisionObject* playerObj = new btCollisionObject();
		playerObj->setCollisionShape(playerShape);

		btTransform playerTransform;

		btScalar playerMass(2.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isPlayerDynamic = (playerMass != 0.f);

		btVector3 localPlayerInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* playerMotionState;
		btRigidBody* playerBody;

		GameObject player = scene->CreateEntity("Player");
		{
			player.emplace<MorphRenderer>();

			//Idle Animation
			std::string idlePrefix = "models/Player/Idle/SkyBoundCharacter0";
			std::string idleFileName;

			for (int i = 0; i < 2; i++)
			{
				idleFileName = idlePrefix + std::to_string(i) + ".obj";

				player.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(idleFileName), 0);

			}

			player.get<MorphRenderer>().SetFrameTime(0.45, 0);


			////////////////////////////////////////////////////////


			//Walk Animation
			std::string walkPrefix = "models/Player/Walk/SkyBoundCharacter0";
			std::string walkFileName;

			for (int i = 0; i < 4; i++)
			{
				walkFileName = walkPrefix + std::to_string(i) + ".obj";

				player.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(walkFileName), 1);

			}

			player.get<MorphRenderer>().SetFrameTime(0.25, 1);

			player.get<MorphRenderer>().SetMesh(player.get<MorphRenderer>().vao).SetMaterial(material0);


			///////////////////////////////////////////////////////

			//Jump Animation
			std::string jumpPrefix = "models/Player/Jump/SkyBoundCharacter0";
			std::string jumpFileName;

			for (int i = 0; i < 5; i++)
			{
				jumpFileName = jumpPrefix + std::to_string(i) + ".obj";

				player.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(jumpFileName), 2);

			}

			player.get<MorphRenderer>().SetFrameTime(0.85, 2);


			//Shiver Animation
			std::string shiverPrefix = "models/Player/Shiver/SkyBoundCharacterShiver0";
			std::string shiverFileName;

			for (int i = 0; i < 8; i++)
			{
				shiverFileName = shiverPrefix + std::to_string(i) + ".obj";

				player.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(shiverFileName), 3);

			}

			player.get<MorphRenderer>().SetFrameTime(0.85, 3);


			/*
			player.emplace<MorphRenderer>();

			std::string walkPrefix = "models/Player/Walk/SkyBoundCharacter0";
			std::string walkFileName;

			for (int i = firstFrame; i < lastFrame; i++)
			{
				walkFileName = walkPrefix + std::to_string(i) + ".obj";

				player.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(walkFileName));

			}

			player.get<MorphRenderer>().SetFrameTime(0.2f);


			player.get<MorphRenderer>().SetMesh(player.get<MorphRenderer>().vao).SetMaterial(material0);
			*/


			player.get<Transform>().SetLocalPosition(0.5f, 0.0f, 5.0f);
			player.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			player.get<Transform>().SetLocalScale(0.5f, 0.5f, 0.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(player);


			//Collision Stuff
			collisionShapes.push_back(playerShape);


			playerTransform.setIdentity();
			playerTransform.setOrigin(btVector3(0.0f, 0.0f, 10.0f));
			//playerTransform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(90.0f)));
			//playerTransform.setRotation(btQuaternion(btVector3(0, 0, 1), btScalar(180.0f)));
			//playerTransform.setOrigin(glm2bt(player.get<Transform>().GetLocalPosition()));
			//playerTransform.setIdentity();

			//Disabling the calculation of inertia makes the physics shape not rotate

			if (isPlayerDynamic)
				playerShape->calculateLocalInertia(playerMass, localPlayerInertia);


			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			playerMotionState = new btDefaultMotionState(playerTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(playerMass, playerMotionState, playerShape, localPlayerInertia);
			playerBody = new btRigidBody(rbInfo);

			playerBody->setDamping(0.95f, 0.95f);

			//add the body to the dynamics world
			dynamicsWorld->addRigidBody(playerBody, 1, 1);

			//dynamicsWorld->addCollisionObject(playerObj, 1, 1);


		}

		#pragma endregion


		#pragma region First Level Island (Plain Islands) Objects

		//Physics - Island 1
		
		btCollisionShape* island1Shape = new btCylinderShapeZ(btVector3(15.3f, 15.3f, 2.0f));

		btTransform island1Transform;

		btScalar island1Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isisland1Dynamic = (island1Mass != 0.f);

		btVector3 localisland1Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* island1MotionState;
		btRigidBody* island1Body;

		GameObject island1 = scene->CreateEntity("Island1");
		{
			VertexArrayObject::sptr Island1VAO = ObjLoader::LoadFromFile("models/plains_island.obj");
			island1.emplace<RendererComponent>().SetMesh(Island1VAO).SetMaterial(material1).SetCastShadow(false);
			island1.get<Transform>().SetLocalPosition(0.0f, 0.0f, -4.5f);
			island1.get<Transform>().SetLocalRotation(-90.0f, 180.0f, 0.0f);
			island1.get<Transform>().SetLocalScale(1.5f, 1.5f, 1.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(island1);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);


			//Collision Stuff
			collisionShapes.push_back(island1Shape);
			island1Transform.setIdentity();
			island1Transform.setOrigin(btVector3(0.0f, 0.0f, -4.3f));
			btQuaternion rotation;
			//rotation.setEuler(0.0f, 0.0f, 0.0f);
			//island1Transform.setRotation(rotation);
			//island1Transform.setRotation(btQuaternion(btVector3(0, 0, 1), btScalar(3.)));
			//island1Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island1Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island1Transform.setOrigin(glm2bt(island1.get<Transform>().GetLocalPosition()));
			//island1Transform.setIdentity();

			if (isisland1Dynamic)
				island1Shape->calculateLocalInertia(island1Mass, localisland1Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			island1MotionState = new btDefaultMotionState(island1Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(island1Mass, island1MotionState, island1Shape, localisland1Inertia);
			island1Body = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			dynamicsWorld->addRigidBody(island1Body, 1, 1);
			
		}


		//Physics - Island 2

		btCollisionShape* island2Shape = new btCylinderShapeZ(btVector3(15.3f, 15.3f, 2.0f));

		btTransform island2Transform;

		btScalar island2Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isisland2Dynamic = (island2Mass != 0.f);

		btVector3 localisland2Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* island2MotionState;
		btRigidBody* island2Body;


		GameObject island2 = scene->CreateEntity("Island2");
		{

			VertexArrayObject::sptr Island2VAO = ObjLoader::LoadFromFile("models/plains_island.obj");
			island2.emplace<RendererComponent>().SetMesh(Island2VAO).SetMaterial(material1).SetCastShadow(false);
			island2.get<Transform>().SetLocalPosition(-35.0f, 0.0f, -4.5f);
			island2.get<Transform>().SetLocalRotation(-90.0f, 180.0f, 0.0f);
			island2.get<Transform>().SetLocalScale(1.5f, 1.5f, 1.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(island2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(island2Shape);
			island2Transform.setIdentity();
			island2Transform.setOrigin(btVector3(-35.0f, 0.0f, -4.5f));
			btQuaternion rotation;

			if (isisland2Dynamic)
				island2Shape->calculateLocalInertia(island2Mass, localisland2Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			island2MotionState = new btDefaultMotionState(island2Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(island2Mass, island2MotionState, island2Shape, localisland2Inertia);
			island2Body = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			dynamicsWorld->addRigidBody(island2Body, 1, 1);
		}

		#pragma endregion


		#pragma region Wizard Object

		//Wizard Physics
		btCollisionShape* wizardShape = new btBoxShape(btVector3(1.f, 4.f, 5.0f));

		btTransform wizardTransform;

		btScalar wizardMass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isWizardDynamic = (wizardMass != 0.f);

		btVector3 localWizardInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* wizardMotionState;
		btRigidBody* wizardBody;

		
		GameObject Wizard = scene->CreateEntity("Wizard");
		{
			Wizard.emplace<MorphRenderer>();

			//Idle Animation
			std::string IdlePrefix = "models/BridgeKeeper/Wizard0";
			std::string IdleFileName;

			for (int i = 0; i < 2; i++)
			{
				IdleFileName = IdlePrefix + std::to_string(i) + ".obj";

				Wizard.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(IdleFileName), 0);

			}

			Wizard.get<MorphRenderer>().SetFrameTime(0.7f, 0);
			Wizard.get<MorphRenderer>().SetLooping(true, 0);


			//Teleport Animation
			std::string teleportPrefix = "models/BridgeKeeper/Teleport/WizardTeleport";
			std::string teleportFileName;

			for (int i = 0; i < 12; i++)
			{
				teleportFileName = teleportPrefix + std::to_string(i) + ".obj";

				Wizard.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(teleportFileName), 1);

			}

			Wizard.get<MorphRenderer>().SetFrameTime(0.85, 1);
			Wizard.get<MorphRenderer>().SetLooping(false, 1);


			Wizard.get<MorphRenderer>().SetMesh(Wizard.get<MorphRenderer>().vao).SetMaterial(material2);
			
			Wizard.get<Transform>().SetLocalPosition(-14.5f, 0.0f, -2.5f);
			Wizard.get<Transform>().SetLocalRotation(90.0f, 0.0f, 270.0f);
			Wizard.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Wizard);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(wizardShape);

			wizardTransform.setIdentity();
			wizardTransform.setOrigin(btVector3(-14.5f, 0.0f, -2.5f));

			if (isWizardDynamic)
				wizardShape->calculateLocalInertia(wizardMass, localWizardInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			wizardMotionState = new btDefaultMotionState(wizardTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(wizardMass, wizardMotionState, wizardShape, localWizardInertia);
			wizardBody = new btRigidBody(rbInfo);


			//add the body to the dynamics world
			dynamicsWorld->addRigidBody(wizardBody, 1, 1);
		}
		
		#pragma endregion


		#pragma region Phantom Objects

		GameObject Phantom = scene->CreateEntity("Phantom");
		{
			Phantom.emplace<MorphRenderer>();

			std::string PhantomPrefix = "models/Phantom/Phantom0";
			std::string PhantomFileName;

			for (int i = 0; i < 4; i++)
			{
				PhantomFileName = PhantomPrefix + std::to_string(i) + ".obj";

				Phantom.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(PhantomFileName), 0);

			}

			Phantom.get<MorphRenderer>().SetFrameTime(0.3f, 0);


			Phantom.get<MorphRenderer>().SetMesh(Phantom.get<MorphRenderer>().vao).SetMaterial(material3);
			Phantom.get<Transform>().SetLocalPosition(-35.0f, 9.5f, -1.0f);
			Phantom.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			Phantom.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Phantom);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject Phantom2 = scene->CreateEntity("Phantom2");
		{
			Phantom2.emplace<MorphRenderer>();

			std::string Phantom2Prefix = "models/Phantom/Phantom0";
			std::string Phantom2FileName;

			for (int i = 0; i < 4; i++)
			{
				Phantom2FileName = Phantom2Prefix + std::to_string(i) + ".obj";

				Phantom2.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(Phantom2FileName), 0);

			}

			Phantom2.get<MorphRenderer>().SetFrameTime(0.3f, 0);


			Phantom2.get<MorphRenderer>().SetMesh(Phantom2.get<MorphRenderer>().vao).SetMaterial(material3);
			Phantom2.get<Transform>().SetLocalPosition(-30.0f, -9.5f, -1.0f);
			Phantom2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			Phantom2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Phantom2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Coin Object

		//Coin Physics
		btCollisionShape* coinShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));

		btCollisionObject* coinObj = new btCollisionObject();
		coinObj->setCollisionShape(coinShape);

		btTransform coinTransform;

		btScalar coinMass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool iscoinDynamic = (coinMass != 0.f);

		btVector3 localcoinInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* coinMotionState;
		btRigidBody* coinBody;

		GameObject Coin = scene->CreateEntity("Coin");
		{
			Coin.emplace<MorphRenderer>();

			std::string coinPrefix = "models/Coin/Coin0";
			std::string coinFileName;

			for (int i = 0; i < 4; i++)
			{
				coinFileName = coinPrefix + std::to_string(i) + ".obj";

				Coin.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(coinFileName), 0);

			}

			Coin.get<MorphRenderer>().SetFrameTime(0.5f, 0);


			Coin.get<MorphRenderer>().SetMesh(Coin.get<MorphRenderer>().vao).SetMaterial(material4);

			Coin.get<Transform>().SetLocalPosition(6.0f, -7.0f, 0.0f);
			Coin.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Coin.get<Transform>().SetLocalScale(0.5f, 0.5f, 0.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Coin);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(coinShape);

			coinTransform.setIdentity();
			coinTransform.setOrigin(btVector3(6.0f, -7.0f, 0.0f));

			if (iscoinDynamic)
				coinShape->calculateLocalInertia(coinMass, localcoinInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			coinMotionState = new btDefaultMotionState(coinTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(coinMass, coinMotionState, coinShape, localcoinInertia);
			coinBody = new btRigidBody(rbInfo);


			//add the body to the dynamics world
			//dynamicsWorld->addRigidBody(coinBody, 1, 1);
		}

		#pragma endregion


		#pragma region Mound Objects

		GameObject Mound = scene->CreateEntity("Mound");
		{
			VertexArrayObject::sptr MoundVAO = ObjLoader::LoadFromFile("models/Mound.obj");
			Mound.emplace<RendererComponent>().SetMesh(MoundVAO).SetMaterial(material5).SetCastShadow(false);
			Mound.get<Transform>().SetLocalPosition(-13.0f, 17.0f, -4.0f);
			Mound.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Mound.get<Transform>().SetLocalScale(1.5f, 1.5f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Mound);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			GameObject Mound2 = scene->CreateEntity("Mound2");
			Mound2.emplace<RendererComponent>().SetMesh(MoundVAO).SetMaterial(material5).SetCastShadow(false);
			Mound2.get<Transform>().SetLocalPosition(-46.5f, -17.5f, -4.0f);
			Mound2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Mound2.get<Transform>().SetLocalScale(1.5f, 1.5f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Mound2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			GameObject Mound3 = scene->CreateEntity("Mound3");
			Mound3.emplace<RendererComponent>().SetMesh(MoundVAO).SetMaterial(material5).SetCastShadow(false);
			Mound3.get<Transform>().SetLocalPosition(-17.5f, -9.5f, -4.0f);
			Mound3.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Mound3.get<Transform>().SetLocalScale(1.5f, 1.5f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Mound3);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			GameObject Mound4 = scene->CreateEntity("Mound4");
			Mound4.emplace<RendererComponent>().SetMesh(MoundVAO).SetMaterial(material5).SetCastShadow(false);
			Mound4.get<Transform>().SetLocalPosition(-13.0f, -23.5f, -4.0f);
			Mound4.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Mound4.get<Transform>().SetLocalScale(2.5f, 1.5f, 2.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Mound4);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Wishing Well Object

		GameObject WishingWell = scene->CreateEntity("Wishing Well");
		{
			VertexArrayObject::sptr WishingWellVAO = ObjLoader::LoadFromFile("models/Wishing Well.obj");
			WishingWell.emplace<RendererComponent>().SetMesh(WishingWellVAO).SetMaterial(material6).SetCastShadow(false);
			WishingWell.get<Transform>().SetLocalPosition(-13.0f, 17.0f, -0.85f);
			WishingWell.get<Transform>().SetLocalRotation(90.0f, 0.0f, 125.0f);
			WishingWell.get<Transform>().SetLocalScale(0.4f, 0.4f, 0.4f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(WishingWell);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Flower Objects

		GameObject Flower = scene->CreateEntity("Flower");
		{
			VertexArrayObject::sptr FlowerVAO = ObjLoader::LoadFromFile("models/flower.obj");
			Flower.emplace<RendererComponent>().SetMesh(FlowerVAO).SetMaterial(material7);
			Flower.get<Transform>().SetLocalPosition(-5.8f, -4.2f, -2.3f);
			Flower.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Flower.get<Transform>().SetLocalScale(1.6f, 1.6f, 1.6f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Flower);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			GameObject Flower2 = scene->CreateEntity("flower2");

			Flower2.emplace<RendererComponent>().SetMesh(FlowerVAO).SetMaterial(material7);
			Flower2.get<Transform>().SetLocalPosition(-6.3f, -4.4f, -2.3f);
			Flower2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Flower2.get<Transform>().SetLocalScale(1.6f, 1.6f, 1.6f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Flower2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			GameObject Flower3 = scene->CreateEntity("flower3");

			Flower3.emplace<RendererComponent>().SetMesh(FlowerVAO).SetMaterial(material7);
			Flower3.get<Transform>().SetLocalPosition(-5.6f, -3.6f, -2.3f);
			Flower3.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Flower3.get<Transform>().SetLocalScale(1.6f, 1.6f, 1.6f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Flower3);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			GameObject Flower4 = scene->CreateEntity("flower4");

			VertexArrayObject::sptr FlowerVAO2 = ObjLoader::LoadFromFile("models/hossain/flower.obj");
			Flower4.emplace<RendererComponent>().SetMesh(FlowerVAO2).SetMaterial(material7);
			Flower4.get<Transform>().SetLocalPosition(-45.0f, -16.5f, -1.0f);
			Flower4.get<Transform>().SetLocalRotation(90.0f, 0.0f, -20.0f);
			Flower4.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Flower4);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

		}

		#pragma endregion


		#pragma region Portal Object

		GameObject Portal = scene->CreateEntity("Portal");
		{

			Portal.emplace<MorphRenderer>();

			//Idle Animation
			std::string IdlePrefix = "models/Portal/portal_updated0";
			std::string IdleFileName;

			for (int i = 0; i <= 4; i++)
			{
				IdleFileName = IdlePrefix + std::to_string(i) + ".obj";

				Portal.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(IdleFileName), 0);

			}

			Portal.get<MorphRenderer>().SetFrameTime(0.7f, 0);
			Portal.get<MorphRenderer>().SetLooping(true, 0);


			VertexArrayObject::sptr PortalVAO = ObjLoader::LoadFromFile("models/portal.obj");
			Portal.emplace<RendererComponent>().SetMesh(PortalVAO).SetMaterial(material8).SetCastShadow(false);
			Portal.get<Transform>().SetLocalPosition(-48.0f, -0.0f, 1.0f);
			Portal.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Portal.get<Transform>().SetLocalScale(3.0f, 3.0f, 3.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Portal);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Crystal Object

		GameObject Crystal = scene->CreateEntity("Crystal");
		{
			VertexArrayObject::sptr CrystalVAO = ObjLoader::LoadFromFile("models/crystal.obj");
			Crystal.emplace<RendererComponent>().SetMesh(CrystalVAO).SetMaterial(material9);
			Crystal.get<Transform>().SetLocalPosition(-46.0f, -0.0f, -0.8f);
			Crystal.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Crystal.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Crystal);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Bridge Object

		//Physics

		btCollisionShape* bridgeShape = new btBoxShape(btVector3(2.0f, 2.0f, 0.8f));

		btTransform bridgeTransform;

		btScalar bridgeMass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isbridgeDynamic = (bridgeMass != 0.f);

		btVector3 localbridgeInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* bridgeMotionState;
		btRigidBody* bridgeBody;

		GameObject Bridge = scene->CreateEntity("Bridge");
		{
			VertexArrayObject::sptr BridgeVAO = ObjLoader::LoadFromFile("models/hossain/NewBridge.obj");
			Bridge.emplace<RendererComponent>().SetMesh(BridgeVAO).SetMaterial(material13).SetCastShadow(false);
			Bridge.get<Transform>().SetLocalPosition(-18.0f, -0.0f, -3.0f);
			Bridge.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Bridge.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Bridge);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(bridgeShape);
			bridgeTransform.setIdentity();
			bridgeTransform.setOrigin(btVector3(-18.0f, 0.0f, -3.2f));
			btQuaternion rotation;

			if (isbridgeDynamic)
				bridgeShape->calculateLocalInertia(bridgeMass, localbridgeInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			bridgeMotionState = new btDefaultMotionState(bridgeTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(bridgeMass, bridgeMotionState, bridgeShape, localbridgeInertia);
			bridgeBody = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			dynamicsWorld->addRigidBody(bridgeBody, 1, 1);
		}

		#pragma endregion


		#pragma region Cactus Object

		GameObject Cacti = scene->CreateEntity("Cactus");
		{

			VertexArrayObject::sptr CactiVAO = ObjLoader::LoadFromFile("models/hossain/cacti.obj");
			Cacti.emplace<RendererComponent>().SetMesh(CactiVAO).SetMaterial(material11);
			Cacti.get<Transform>().SetLocalPosition(-46.5f, -17.5f, -1.0f);
			Cacti.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Cacti.get<Transform>().SetLocalScale(0.5f, 0.5f, 0.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Cacti);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Bush Object

		GameObject Bush = scene->CreateEntity("Bush");
		{
			VertexArrayObject::sptr BushVAO = ObjLoader::LoadFromFile("models/Bush.obj");
			Bush.emplace<RendererComponent>().SetMesh(BushVAO).SetMaterial(material11);
			Bush.get<Transform>().SetLocalPosition(6.0f, 11.0f, -2.7f);
			Bush.get<Transform>().SetLocalRotation(90.0f, 0.0f, 40.0f);
			Bush.get<Transform>().SetLocalScale(2.0f, 2.0f, 2.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Bush);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Sign Object

		GameObject Sign = scene->CreateEntity("Sign");
		{
			VertexArrayObject::sptr SignVAO = ObjLoader::LoadFromFile("models/Fardeen/Sign.obj");
			Sign.emplace<RendererComponent>().SetMesh(SignVAO).SetMaterial(material14);
			Sign.get<Transform>().SetLocalPosition(-6.0f, -5.0f, -2.7f);
			Sign.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Sign.get<Transform>().SetLocalScale(0.8f, 0.8f, 0.8f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Sign);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Tree Object

		GameObject Tree = scene->CreateEntity("Tree");
		{
			VertexArrayObject::sptr TreeVAO = ObjLoader::LoadFromFile("models/Fardeen/Tree.obj");
			Tree.emplace<RendererComponent>().SetMesh(TreeVAO).SetMaterial(material15);
			Tree.get<Transform>().SetLocalPosition(-17.5f, -9.5f, -2.7f);
			Tree.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Tree.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Tree);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Platform Object

		GameObject Platform = scene->CreateEntity("Platform");
		{
			VertexArrayObject::sptr PlatformVAO = ObjLoader::LoadFromFile("models/Fardeen/Platform.obj");
			Platform.emplace<RendererComponent>().SetMesh(PlatformVAO).SetMaterial(material16);
			Platform.get<Transform>().SetLocalPosition(9.5f, 9.5f, -2.7f);
			Platform.get<Transform>().SetLocalRotation(90.0f, 0.0f, -60.0f);
			Platform.get<Transform>().SetLocalScale(2.0f, 2.0f, 2.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Platform);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Mailbox Object

		GameObject Mailbox = scene->CreateEntity("mailbox");
		{
			VertexArrayObject::sptr MailboxVAO = ObjLoader::LoadFromFile("models/hossain/mailbox.obj");
			Mailbox.emplace<RendererComponent>().SetMesh(MailboxVAO).SetMaterial(material18);
			Mailbox.get<Transform>().SetLocalPosition(11.0f, 7.5f, -2.7f);
			Mailbox.get<Transform>().SetLocalRotation(90.0f, 0.0f, -60.0f);
			Mailbox.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Mailbox);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region House Object

		GameObject House = scene->CreateEntity("House");
		{
			VertexArrayObject::sptr HouseVAO = ObjLoader::LoadFromFile("models/CompletedHouse.obj");
			House.emplace<RendererComponent>().SetMesh(HouseVAO).SetMaterial(material20).SetCastShadow(false);
			House.get<Transform>().SetLocalPosition(-13.0f, -23.5f, -1.1f);
			House.get<Transform>().SetLocalRotation(90.0f, 0.0f, 225.0f);
			House.get<Transform>().SetLocalScale(0.45f, 0.8f, 0.8f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(House);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Reflectivity Shader

		// Load a second material for our reflective material!
		Shader::sptr reflectiveShader = Shader::Create();
		reflectiveShader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
		reflectiveShader->LoadShaderPartFromFile("shaders/frag_reflection.frag.glsl", GL_FRAGMENT_SHADER);
		reflectiveShader->Link();

		ShaderMaterial::sptr reflectiveMat = ShaderMaterial::Create();
		reflectiveMat->Shader = reflectiveShader;
		reflectiveMat->Set("s_Environment", environmentMap);
		// TODO: send the rotation to apply to the skybox
		reflectiveMat->Set("u_EnvironmentRotation", glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f),
			glm::vec3(1, 0, 0))));

		#pragma endregion


		#pragma region Camera Objects

		// Create an object to be our camera
		GameObject cameraObject = scene->CreateEntity("Camera");
		{
			//cameraObject.get<Transform>().SetLocalPosition(6.0f, 3.0f, 8.0f).LookAt(glm::vec3(-5.0f, 0.0f, 8.0f));
			cameraObject.get<Transform>().SetLocalPosition(8.0f, 0.0f, 4.0f).LookAt(glm::vec3(0));
			//cameraObject.get<Transform>().SetParent(player);

			// We'll make our camera a component of the camera object
			Camera& camera = cameraObject.emplace<Camera>();// Camera::Create();
			camera.SetPosition(glm::vec3(6.0f, 3.0f, 8.0f));
			camera.SetUp(glm::vec3(0, 0, 1));
			//camera.LookAt(glm::vec3(-5.0f, 0.0f, 8.0f));
			camera.LookAt(glm::vec3(0));
			camera.SetFovDegrees(90.0f); // Set an initial FOV
			camera.SetOrthoHeight(3.0f);
			//BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(player);
			//BehaviourBinding::Bind<CameraControlBehaviour>(cameraObject);
		}


		// Create an object to be our orthographic camera
		GameObject orthoCameraObject = scene->CreateEntity("OrthoCamera");
		{
			//cameraObject.get<Transform>().SetLocalPosition(0, 3, 3).LookAt(glm::vec3(0, 0, 0));
			//Looking at water and terrain positions
			//cameraObject.get<Transform>().SetLocalPosition(-43.89, 25.74, 3.89).LookAt(glm::vec3(-40.69, -0.53, -7.83));
			orthoCameraObject.get<Transform>().SetLocalPosition(8.0f, 0.0f, 4.0f).LookAt(glm::vec3(0));

			// We'll make our camera a component of the camera object
			Camera& orthoCamera = orthoCameraObject.emplace<Camera>();// Camera::Create();
			orthoCamera.SetPosition(glm::vec3(0, 3, 3));
			orthoCamera.SetUp(glm::vec3(0, 0, 1));
			orthoCamera.LookAt(glm::vec3(0));
			orthoCamera.SetFovDegrees(90.0f); // Set an initial FOV
			orthoCamera.SetOrthoHeight(3.0f);
			orthoCameraObject.get<Camera>().ToggleOrtho();
			//BehaviourBinding::Bind<CameraControlBehaviour>(orthoCameraObject);
		}

		#pragma endregion


		#pragma region Post-Processing Effect Objects

		glfwGetWindowSize(BackendHandler::window, &width, &height);

		GameObject shadowBufferObject = scene->CreateEntity("Shadow Buffer");
		{
			shadowBuffer = &shadowBufferObject.emplace<Framebuffer>();
			shadowBuffer->AddDepthTarget();
			shadowBuffer->Init(shadowWidth, shadowHeight);
		}
		
		GameObject framebufferObject = scene->CreateEntity("Basic Effect");
		{
			basicEffect = &framebufferObject.emplace<PostEffect>();
			basicEffect->Init(width, height);
		}

		GameObject greyscaleEffectObject = scene->CreateEntity("Greyscale Effect");
		{
			greyscaleEffect = &greyscaleEffectObject.emplace<GreyscaleEffect>();
			greyscaleEffect->Init(width, height);
		}
		effects.push_back(greyscaleEffect);

		GameObject warmEffectObject = scene->CreateEntity("Warm Color Correction Effect");
		{
			warmEffect = &warmEffectObject.emplace<WarmEffect>();
			warmEffect->Init(width, height);
		}
		effects.push_back(warmEffect);
		
		GameObject coolEffectObject = scene->CreateEntity("Cool Color Correction Effect");
		{
			coolEffect = &coolEffectObject.emplace<CoolEffect>();
			coolEffect->Init(width, height);
		}
		effects.push_back(coolEffect);
		
		GameObject colorCorrectionEffectObject = scene->CreateEntity("Color Correction Effect");
		{
			colorCorrectionEffect = &colorCorrectionEffectObject.emplace<ColorCorrectionEffect>();
			colorCorrectionEffect->Init(width, height);
		}
		effects.push_back(colorCorrectionEffect);

		GameObject bloomEffectObject = scene->CreateEntity("Color Correction Effect");
		{
			bloomEffect = &bloomEffectObject.emplace<BloomEffect>();
			bloomEffect->Init(width, height);
		}
		effects.push_back(bloomEffect);

		GameObject vignetteEffectObject = scene->CreateEntity("Vignette Effect");
		{
			vignetteEffect = &vignetteEffectObject.emplace<VignetteEffect>();
			vignetteEffect->Init(width, height);
		}
		effects.push_back(vignetteEffect);

		GameObject pixelateEffectObject = scene->CreateEntity("Pixelate Effect");
		{
			pixelateEffect = &pixelateEffectObject.emplace<PixelateEffect>();
			pixelateEffect->Init(width, height);
		}
		effects.push_back(pixelateEffect);

		#pragma endregion


		#pragma endregion 
		///////////////////////////////////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////// Scene 2 Generation //////////////////////////////////////////////////
		#pragma region Scene 2 Generation

		// Create a scene, and set it to be the active scene in the application
		GameScene::sptr scene2 = GameScene::Create("Scene 2");

		// We can create a group ahead of time to make iterating on the group faster
		entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<Transform>, RendererComponent> renderGroup2 =
			scene2->Registry().group<RendererComponent>(entt::get_t<Transform>());



		#pragma region Second Level Island (Taiga Island) Objects


		btCollisionShape* taigaIsland1Shape = new btCylinderShapeZ(btVector3(20.5f, 20.5f, 2.0f));

		btTransform taigaIsland1Transform;

		btScalar taigaIsland1Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isTaigaIsland1Dynamic = (taigaIsland1Mass != 0.f);

		btVector3 localtaigaIsland1Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* taigaIsland1MotionState;
		btRigidBody* taigaIsland1Body;

		GameObject TaigaGround = scene2->CreateEntity("Taiga");
		{

			VertexArrayObject::sptr TaigaVAO = ObjLoader::LoadFromFile("models/taiga_island.obj");
			TaigaGround.emplace<RendererComponent>().SetMesh(TaigaVAO).SetMaterial(material10);
			TaigaGround.get<Transform>().SetLocalPosition(4.0f, 0.0f, -11.5f);
			TaigaGround.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			TaigaGround.get<Transform>().SetLocalScale(3.0f, 3.0f, 3.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(TaigaGround);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(taigaIsland1Shape);
			taigaIsland1Transform.setIdentity();
			taigaIsland1Transform.setOrigin(btVector3(2.0f, 50.0f, -6.4f));
			btQuaternion rotation;
			//rotation.setEuler(0.0f, 0.0f, 0.0f);
			//island1Transform.setRotation(rotation);
			taigaIsland1Transform.setRotation(btQuaternion(btVector3(0, 0, 1), btScalar(3.)));
			//island1Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island1Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island1Transform.setOrigin(glm2bt(island1.get<Transform>().GetLocalPosition()));
			//island1Transform.setIdentity();

			if (isTaigaIsland1Dynamic)
				taigaIsland1Shape->calculateLocalInertia(taigaIsland1Mass, localtaigaIsland1Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			taigaIsland1MotionState = new btDefaultMotionState(taigaIsland1Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(taigaIsland1Mass, taigaIsland1MotionState, taigaIsland1Shape, localtaigaIsland1Inertia);
			taigaIsland1Body = new btRigidBody(rbInfo);



			//add the body to the dynamics world
			//dynamicsWorld->addRigidBody(taigaIsland1Body, 1, 1);
		}


		btCollisionShape* taigaIsland2Shape = new btCylinderShapeZ(btVector3(20.5f, 20.5f, 2.0f));

		btTransform taigaIsland2Transform;

		btScalar taigaIsland2Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isTaigaIsland2Dynamic = (taigaIsland2Mass != 0.f);

		btVector3 localtaigaIsland2Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* taigaIsland2MotionState;
		btRigidBody* taigaIsland2Body;

		GameObject TaigaGround2 = scene2->CreateEntity("Taiga2");
		{

			VertexArrayObject::sptr Taiga2VAO = ObjLoader::LoadFromFile("models/taiga_island.obj");
			TaigaGround2.emplace<RendererComponent>().SetMesh(Taiga2VAO).SetMaterial(material10);
			TaigaGround2.get<Transform>().SetLocalPosition(-35.0f, 0.0f, -20.5f); //-11.5
			TaigaGround2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			TaigaGround2.get<Transform>().SetLocalScale(3.0f, 3.0f, 3.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(TaigaGround2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(taigaIsland2Shape);
			taigaIsland2Transform.setIdentity();
			taigaIsland2Transform.setOrigin(btVector3(-40.7f, 50.4f, -6.3f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			taigaIsland2Transform.setRotation(rotation);
			//island2Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island2Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island2Transform.setOrigin(glm2bt(island2.get<Transform>().GetLocalPosition()));
			//island2Transform.setIdentity();

			if (isTaigaIsland2Dynamic)
				taigaIsland2Shape->calculateLocalInertia(taigaIsland2Mass, localtaigaIsland2Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			taigaIsland2MotionState = new btDefaultMotionState(taigaIsland2Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(taigaIsland2Mass, taigaIsland2MotionState, taigaIsland2Shape, localtaigaIsland2Inertia);
			taigaIsland2Body = new btRigidBody(rbInfo);

		}

		btCollisionShape* taigaIsland3Shape = new btCylinderShapeZ(btVector3(20.5f, 20.5f, 2.0f));

		btTransform taigaIsland3Transform;

		btScalar taigaIsland3Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isTaigaIsland3Dynamic = (taigaIsland3Mass != 0.f);

		btVector3 localtaigaIsland3Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* taigaIsland3MotionState;
		btRigidBody* taigaIsland3Body;

		GameObject TaigaGround3 = scene2->CreateEntity("Taiga3");
		{

			VertexArrayObject::sptr Taiga3VAO = ObjLoader::LoadFromFile("models/taiga_island.obj");
			TaigaGround3.emplace<RendererComponent>().SetMesh(Taiga3VAO).SetMaterial(material10);
			TaigaGround3.get<Transform>().SetLocalPosition(-20.0f, 50.0f, -13.0f);
			TaigaGround3.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			TaigaGround3.get<Transform>().SetLocalScale(3.0f, 3.0f, 3.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(TaigaGround3);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(taigaIsland3Shape);
			taigaIsland3Transform.setIdentity();
			taigaIsland3Transform.setOrigin(btVector3(-22.0f, 0.9f, -6.4f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			taigaIsland3Transform.setRotation(rotation);
			//island3Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island3Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island3Transform.setOrigin(glm2bt(island3.get<Transform>().GetLocalPosition()));
			//island3Transform.setIdentity();

			if (isTaigaIsland3Dynamic)
				taigaIsland3Shape->calculateLocalInertia(taigaIsland3Mass, localtaigaIsland3Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			taigaIsland3MotionState = new btDefaultMotionState(taigaIsland3Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(taigaIsland3Mass, taigaIsland3MotionState, taigaIsland3Shape, localtaigaIsland3Inertia);
			taigaIsland3Body = new btRigidBody(rbInfo);
		}


		btCollisionShape* taigaIsland4Shape = new btCylinderShapeZ(btVector3(20.5f, 20.5f, 2.0f));

		btTransform taigaIsland4Transform;

		btScalar taigaIsland4Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isTaigaIsland4Dynamic = (taigaIsland4Mass != 0.f);

		btVector3 localtaigaIsland4Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* taigaIsland4MotionState;
		btRigidBody* taigaIsland4Body;

		GameObject TaigaGround4 = scene2->CreateEntity("Taiga4");
		{

			VertexArrayObject::sptr Taiga4VAO = ObjLoader::LoadFromFile("models/taiga_island.obj");
			TaigaGround4.emplace<RendererComponent>().SetMesh(Taiga4VAO).SetMaterial(material10);
			TaigaGround4.get<Transform>().SetLocalPosition(-20.0f, -50.0f, -13.0f);
			TaigaGround4.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			TaigaGround4.get<Transform>().SetLocalScale(3.0f, 3.0f, 3.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(TaigaGround4);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(taigaIsland4Shape);
			taigaIsland4Transform.setIdentity();
			taigaIsland4Transform.setOrigin(btVector3(-22.0f, 98.9f, -6.4f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			taigaIsland4Transform.setRotation(rotation);
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setOrigin(glm2bt(island4.get<Transform>().GetLocalPosition()));
			//island4Transform.setIdentity();

			if (isTaigaIsland4Dynamic)
				taigaIsland4Shape->calculateLocalInertia(taigaIsland4Mass, localtaigaIsland4Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			taigaIsland4MotionState = new btDefaultMotionState(taigaIsland4Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(taigaIsland4Mass, taigaIsland4MotionState, taigaIsland4Shape, localtaigaIsland4Inertia);
			taigaIsland4Body = new btRigidBody(rbInfo);
		}

		#pragma endregion


		#pragma region Sketchy Guy Object

		//sketchyGuy Physics
		btCollisionShape* sketchyGuyShape = new btBoxShape(btVector3(1.1f, 1.1f, 2.4f));

		btTransform sketchyGuyTransform;

		btScalar sketchyGuyMass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool issketchyGuyDynamic = (sketchyGuyMass != 0.f);

		btVector3 localsketchyGuyInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* sketchyGuyMotionState;
		btRigidBody* sketchyGuyBody;


		GameObject sketchyGuy = scene2->CreateEntity("sketchyGuy1");
		{
			sketchyGuy.emplace<MorphRenderer>();

			std::string IdlePrefix = "models/SketchyGuy/Idle/SketchyGuyIdle0";
			std::string IdleFileName;

			for (int i = 0; i < 2; i++)
			{
				IdleFileName = IdlePrefix + std::to_string(i) + ".obj";

				sketchyGuy.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(IdleFileName), 0);

			}

			sketchyGuy.get<MorphRenderer>().SetFrameTime(0.7f, 0);
			sketchyGuy.get<MorphRenderer>().SetLooping(true, 0);


			sketchyGuy.get<MorphRenderer>().SetMesh(sketchyGuy.get<MorphRenderer>().vao).SetMaterial(sketchyGuyMat);

			sketchyGuy.get<Transform>().SetLocalPosition(-14.5f, 0.0f, -2.5f);
			sketchyGuy.get<Transform>().SetLocalRotation(90.0f, 0.0f, 180.0f);
			sketchyGuy.get<Transform>().SetLocalScale(0.7f, 0.7f, 0.7f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(sketchyGuy);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(sketchyGuyShape);

			sketchyGuyTransform.setIdentity();
			sketchyGuyTransform.setOrigin(btVector3(-16.7f, 18.6f, -1.8f));

			if (issketchyGuyDynamic)
				sketchyGuyShape->calculateLocalInertia(sketchyGuyMass, localsketchyGuyInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			sketchyGuyMotionState = new btDefaultMotionState(sketchyGuyTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(sketchyGuyMass, sketchyGuyMotionState, sketchyGuyShape, localsketchyGuyInertia);
			sketchyGuyBody = new btRigidBody(rbInfo);
		}

		#pragma endregion


		#pragma region Sketchy Guy Object 2

		//sketchyGuy2 Physics
		btCollisionShape* sketchyGuy2Shape = new btBoxShape(btVector3(1.1f, 1.1f, 2.4f));

		btTransform sketchyGuy2Transform;

		btScalar sketchyGuy2Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool issketchyGuy2Dynamic = (sketchyGuy2Mass != 0.f);

		btVector3 localsketchyGuy2Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* sketchyGuy2MotionState;
		btRigidBody* sketchyGuy2Body;


		GameObject sketchyGuy2 = scene2->CreateEntity("sketchyGuy2");
		{
			sketchyGuy2.emplace<MorphRenderer>();

			std::string IdlePrefix = "models/SketchyGuy/Idle/SketchyGuyIdle0";
			std::string IdleFileName;

			for (int i = 0; i < 2; i++)
			{
				IdleFileName = IdlePrefix + std::to_string(i) + ".obj";

				sketchyGuy2.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(IdleFileName), 0);

			}

			sketchyGuy2.get<MorphRenderer>().SetFrameTime(0.7f, 0);
			sketchyGuy2.get<MorphRenderer>().SetLooping(true, 0);


			sketchyGuy2.get<MorphRenderer>().SetMesh(sketchyGuy2.get<MorphRenderer>().vao).SetMaterial(sketchyGuy2Mat);

			sketchyGuy2.get<Transform>().SetLocalPosition(-5.8f, 94.3f, -1.8f);
			sketchyGuy2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 180.0f);
			sketchyGuy2.get<Transform>().SetLocalScale(0.7f, 0.7f, 0.7f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(sketchyGuy2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(sketchyGuy2Shape);

			sketchyGuy2Transform.setIdentity();
			sketchyGuy2Transform.setOrigin(btVector3(-14.1f, 97.3f, -1.9f));

			if (issketchyGuy2Dynamic)
				sketchyGuy2Shape->calculateLocalInertia(sketchyGuy2Mass, localsketchyGuy2Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			sketchyGuy2MotionState = new btDefaultMotionState(sketchyGuy2Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(sketchyGuy2Mass, sketchyGuy2MotionState, sketchyGuy2Shape, localsketchyGuy2Inertia);
			sketchyGuy2Body = new btRigidBody(rbInfo);
		}

		#pragma endregion


		#pragma region Coin Object

		//Coin2 Physics
		btCollisionShape* Coin2Shape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));

		btCollisionObject* Coin2Obj = new btCollisionObject();
		Coin2Obj->setCollisionShape(Coin2Shape);

		btTransform Coin2Transform;

		btScalar Coin2Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isCoin2Dynamic = (Coin2Mass != 0.f);

		btVector3 localCoin2Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* Coin2MotionState;
		btRigidBody* Coin2Body;

		GameObject Coin2 = scene2->CreateEntity("Coin");
		{
			Coin2.emplace<MorphRenderer>();

			std::string coinPrefix = "models/Coin/Coin0";
			std::string coinFileName;

			for (int i = 0; i < 4; i++)
			{
				coinFileName = coinPrefix + std::to_string(i) + ".obj";

				Coin2.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(coinFileName), 0);

			}

			Coin2.get<MorphRenderer>().SetFrameTime(0.5f, 0);


			Coin2.get<MorphRenderer>().SetMesh(Coin2.get<MorphRenderer>().vao).SetMaterial(material4);

			Coin2.get<Transform>().SetLocalPosition(-19.6f, 6.6f, 50.5f); //-1.5f
			Coin2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Coin2.get<Transform>().SetLocalScale(0.5f, 0.5f, 0.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Coin2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(Coin2Shape);

			Coin2Transform.setIdentity();
			Coin2Transform.setOrigin(btVector3(6.0f, -7.0f, 0.0f));

			if (isCoin2Dynamic)
				Coin2Shape->calculateLocalInertia(Coin2Mass, localCoin2Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			Coin2MotionState = new btDefaultMotionState(Coin2Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(Coin2Mass, Coin2MotionState, Coin2Shape, localCoin2Inertia);
			Coin2Body = new btRigidBody(rbInfo);


			//add the body to the dynamics world
			//dynamicsWorld->addRigidBody(coinBody, 1, 1);
		}

		#pragma endregion


		#pragma region Sorting Objects

		GameObject bananasObj = scene2->CreateEntity("bananas");
		{
			bananasObj.emplace<SortingInfo>().SetQuantity(5);
			bananasObj.get<SortingInfo>().SetAlphabet('B');
			bananasObj.get<SortingInfo>().SetColor(3);

			VertexArrayObject::sptr bananasVAO = ObjLoader::LoadFromFile("models/SortingObjects/Bananas.obj");
			bananasObj.emplace<RendererComponent>().SetMesh(bananasVAO).SetMaterial(bananaMat);
			bananasObj.get<Transform>().SetLocalPosition(-24.4f, 89.5f, -3.0f);
			bananasObj.get<Transform>().SetLocalRotation(90.0f, 0.0f, 30.0f);
			bananasObj.get<Transform>().SetLocalScale(0.3f, 0.3f, 0.3f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(bananasObj);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject appleObj = scene2->CreateEntity("apple");
		{
			appleObj.emplace<SortingInfo>().SetQuantity(3);
			appleObj.get<SortingInfo>().SetAlphabet('A');
			appleObj.get<SortingInfo>().SetColor(1);

			VertexArrayObject::sptr appleVAO = ObjLoader::LoadFromFile("models/SortingObjects/RedApples.obj");
			appleObj.emplace<RendererComponent>().SetMesh(appleVAO).SetMaterial(appleMat);
			appleObj.get<Transform>().SetLocalPosition(-24.4f, 95.0f, -3.0f);
			appleObj.get<Transform>().SetLocalRotation(90.0f, 0.0f, 70.0f);
			appleObj.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(appleObj);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject carrotObj = scene2->CreateEntity("carrot");
		{
			carrotObj.emplace<SortingInfo>().SetQuantity(2);
			carrotObj.get<SortingInfo>().SetAlphabet('C');
			carrotObj.get<SortingInfo>().SetColor(2);

			VertexArrayObject::sptr carrotVAO = ObjLoader::LoadFromFile("models/SortingObjects/Carrots.obj");
			carrotObj.emplace<RendererComponent>().SetMesh(carrotVAO).SetMaterial(carrotMat);
			carrotObj.get<Transform>().SetLocalPosition(-24.4f, 100.0f, -3.0f);
			carrotObj.get<Transform>().SetLocalRotation(90.0f, 0.0f, 70.0f);
			carrotObj.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(carrotObj);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject greenGrapeObj = scene2->CreateEntity("greenGrape");
		{
			greenGrapeObj.emplace<SortingInfo>().SetQuantity(6);
			greenGrapeObj.get<SortingInfo>().SetAlphabet('G');
			greenGrapeObj.get<SortingInfo>().SetColor(4);

			VertexArrayObject::sptr greenGrapeVAO = ObjLoader::LoadFromFile("models/SortingObjects/GreenGrapes.obj");
			greenGrapeObj.emplace<RendererComponent>().SetMesh(greenGrapeVAO).SetMaterial(greenGrapeMat);
			greenGrapeObj.get<Transform>().SetLocalPosition(-24.4f, 105.0f, -3.0f);
			greenGrapeObj.get<Transform>().SetLocalRotation(90.0f, 0.0f, 70.0f);
			greenGrapeObj.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(greenGrapeObj);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject purpleGrapeObj = scene2->CreateEntity("purpleGrape");
		{
			purpleGrapeObj.emplace<SortingInfo>().SetQuantity(4);
			purpleGrapeObj.get<SortingInfo>().SetAlphabet('P');
			purpleGrapeObj.get<SortingInfo>().SetColor(7);

			VertexArrayObject::sptr purpleGrapeVAO = ObjLoader::LoadFromFile("models/SortingObjects/PurpleGrapes.obj");
			purpleGrapeObj.emplace<RendererComponent>().SetMesh(purpleGrapeVAO).SetMaterial(purpleGrapeMat);
			purpleGrapeObj.get<Transform>().SetLocalPosition(-24.4f, 110.0f, -3.0f);
			purpleGrapeObj.get<Transform>().SetLocalRotation(90.0f, 0.0f, 70.0f);
			purpleGrapeObj.get<Transform>().SetLocalScale(1.5f, 1.5f, 1.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(purpleGrapeObj);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject blueDonutObj = scene2->CreateEntity("blueDonut");
		{
			blueDonutObj.emplace<SortingInfo>().SetQuantity(1);
			blueDonutObj.get<SortingInfo>().SetAlphabet('B');
			blueDonutObj.get<SortingInfo>().SetColor(5);

			VertexArrayObject::sptr blueDonutVAO = ObjLoader::LoadFromFile("models/SortingObjects/BlueDonut.obj");
			blueDonutObj.emplace<RendererComponent>().SetMesh(blueDonutVAO).SetMaterial(blueDonutMat);
			blueDonutObj.get<Transform>().SetLocalPosition(-24.4f, 115.0f, -3.0f);
			blueDonutObj.get<Transform>().SetLocalRotation(90.0f, 0.0f, 70.0f);
			blueDonutObj.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(blueDonutObj);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}




		GameObject bananasObj2 = scene2->CreateEntity("bananas");
		{
			bananasObj2.emplace<SortingInfo>().SetQuantity(5);
			bananasObj2.get<SortingInfo>().SetAlphabet('B');
			bananasObj2.get<SortingInfo>().SetColor(3);

			VertexArrayObject::sptr bananasVAO = ObjLoader::LoadFromFile("models/SortingObjects/Bananas.obj");
			bananasObj2.emplace<RendererComponent>().SetMesh(bananasVAO).SetMaterial(bananaMat);
			bananasObj2.get<Transform>().SetLocalPosition(-10.4f, 89.5f, -3.0f);
			bananasObj2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 30.0f);
			bananasObj2.get<Transform>().SetLocalScale(0.3f, 0.3f, 0.3f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(bananasObj);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject appleObj2 = scene2->CreateEntity("apple");
		{
			appleObj2.emplace<SortingInfo>().SetQuantity(3);
			appleObj2.get<SortingInfo>().SetAlphabet('A');
			appleObj2.get<SortingInfo>().SetColor(1);

			VertexArrayObject::sptr appleVAO = ObjLoader::LoadFromFile("models/SortingObjects/RedApples.obj");
			appleObj2.emplace<RendererComponent>().SetMesh(appleVAO).SetMaterial(appleMat);
			appleObj2.get<Transform>().SetLocalPosition(-10.4f, 95.0f, -3.0f);
			appleObj2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 70.0f);
			appleObj2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(appleObj);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject carrotObj2 = scene2->CreateEntity("carrot");
		{
			carrotObj2.emplace<SortingInfo>().SetQuantity(2);
			carrotObj2.get<SortingInfo>().SetAlphabet('C');
			carrotObj2.get<SortingInfo>().SetColor(2);

			VertexArrayObject::sptr carrotVAO = ObjLoader::LoadFromFile("models/SortingObjects/Carrots.obj");
			carrotObj2.emplace<RendererComponent>().SetMesh(carrotVAO).SetMaterial(carrotMat);
			carrotObj2.get<Transform>().SetLocalPosition(-10.4f, 100.0f, -3.0f);
			carrotObj2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 70.0f);
			carrotObj2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(carrotObj);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject greenGrapeObj2 = scene2->CreateEntity("greenGrape");
		{
			greenGrapeObj2.emplace<SortingInfo>().SetQuantity(6);
			greenGrapeObj2.get<SortingInfo>().SetAlphabet('G');
			greenGrapeObj2.get<SortingInfo>().SetColor(4);

			VertexArrayObject::sptr greenGrapeVAO = ObjLoader::LoadFromFile("models/SortingObjects/GreenGrapes.obj");
			greenGrapeObj2.emplace<RendererComponent>().SetMesh(greenGrapeVAO).SetMaterial(greenGrapeMat);
			greenGrapeObj2.get<Transform>().SetLocalPosition(-10.4f, 105.0f, -3.0f);
			greenGrapeObj2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 70.0f);
			greenGrapeObj2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(greenGrapeObj);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject purpleGrapeObj2 = scene2->CreateEntity("purpleGrape");
		{
			purpleGrapeObj2.emplace<SortingInfo>().SetQuantity(4);
			purpleGrapeObj2.get<SortingInfo>().SetAlphabet('P');
			purpleGrapeObj2.get<SortingInfo>().SetColor(7);

			VertexArrayObject::sptr purpleGrapeVAO = ObjLoader::LoadFromFile("models/SortingObjects/PurpleGrapes.obj");
			purpleGrapeObj2.emplace<RendererComponent>().SetMesh(purpleGrapeVAO).SetMaterial(purpleGrapeMat);
			purpleGrapeObj2.get<Transform>().SetLocalPosition(-10.4f, 110.0f, -3.0f);
			purpleGrapeObj2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 70.0f);
			purpleGrapeObj2.get<Transform>().SetLocalScale(1.5f, 1.5f, 1.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(purpleGrapeObj);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject blueDonutObj2 = scene2->CreateEntity("blueDonut");
		{
			blueDonutObj2.emplace<SortingInfo>().SetQuantity(1);
			blueDonutObj2.get<SortingInfo>().SetAlphabet('B');
			blueDonutObj2.get<SortingInfo>().SetColor(5);

			VertexArrayObject::sptr blueDonutVAO = ObjLoader::LoadFromFile("models/SortingObjects/BlueDonut.obj");
			blueDonutObj2.emplace<RendererComponent>().SetMesh(blueDonutVAO).SetMaterial(blueDonutMat);
			blueDonutObj2.get<Transform>().SetLocalPosition(-10.4f, 115.0f, -3.0f);
			blueDonutObj2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 70.0f);
			blueDonutObj2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(blueDonutObj);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Phantom Objects Level 2

		GameObject PhantomLevel2 = scene2->CreateEntity("Phantom");
		{
			PhantomLevel2.emplace<MorphRenderer>();

			std::string PhantomLevel2Prefix = "models/Phantom/Phantom0";
			std::string PhantomLevel2FileName;

			for (int i = 0; i < 4; i++)
			{
				PhantomLevel2FileName = PhantomLevel2Prefix + std::to_string(i) + ".obj";

				PhantomLevel2.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(PhantomLevel2FileName), 0);

			}

			PhantomLevel2.get<MorphRenderer>().SetFrameTime(0.3f, 0);

			//VertexArrayObject::sptr PhantomLevel2VAO = ObjLoader::LoadFromFile("models/Phantom/Phantom0.obj");
			PhantomLevel2.get<MorphRenderer>().SetMesh(PhantomLevel2.get<MorphRenderer>().vao).SetMaterial(material3);
			PhantomLevel2.get<Transform>().SetLocalPosition(-9.5f, -5.0f, -2.9f);
			PhantomLevel2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 270.0f);
			PhantomLevel2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(PhantomLevel2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject Phantom2Level2 = scene2->CreateEntity("Phantom2");
		{
			Phantom2Level2.emplace<MorphRenderer>();

			std::string Phantom2Level2Prefix = "models/Phantom/Phantom0";
			std::string Phantom2Level2FileName;

			for (int i = 0; i < 4; i++)
			{
				Phantom2Level2FileName = Phantom2Level2Prefix + std::to_string(i) + ".obj";

				Phantom2Level2.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(Phantom2Level2FileName), 0);

			}

			Phantom2Level2.get<MorphRenderer>().SetFrameTime(0.3f, 0);

			//VertexArrayObject::sptr Phantom2Level2VAO = ObjLoader::LoadFromFile("models/Phantom/Phantom0.obj");
			Phantom2Level2.get<MorphRenderer>().SetMesh(Phantom2Level2.get<MorphRenderer>().vao).SetMaterial(material3);
			Phantom2Level2.get<Transform>().SetLocalPosition(-27.5f, 5.0f, -2.9f);
			Phantom2Level2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Phantom2Level2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Phantom2Level2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Parking Meter

		//parkingMeter Physics
		btCollisionShape* parkingMeterShape = new btBoxShape(btVector3(1.1f, 1.1f, 2.4f));

		btTransform parkingMeterTransform;

		btScalar parkingMeterMass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isparkingMeterDynamic = (parkingMeterMass != 0.f);

		btVector3 localparkingMeterInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* parkingMeterMotionState;
		btRigidBody* parkingMeterBody;


		GameObject parkingMeter = scene->CreateEntity("parkingMeter");
		{
			parkingMeter.emplace<MorphRenderer>();

			std::string IdlePrefix = "models/ParkingMeter/Parking_Meter_0000";
			std::string IdleFileName;

			for (int i = 1; i < 8; i++)
			{
				IdleFileName = IdlePrefix + std::to_string(i) + ".obj";

				parkingMeter.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(IdleFileName), 0);

			}

			parkingMeter.get<MorphRenderer>().SetFrameTime(0.7f, 0);
			parkingMeter.get<MorphRenderer>().SetLooping(true, 0);


			parkingMeter.get<MorphRenderer>().SetMesh(parkingMeter.get<MorphRenderer>().vao).SetMaterial(parkingMeterMat);

			parkingMeter.get<Transform>().SetLocalPosition(-14.5f, 0.0f, -2.5f);
			parkingMeter.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			parkingMeter.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(parkingMeter);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(parkingMeterShape);

			parkingMeterTransform.setIdentity();
			parkingMeterTransform.setOrigin(btVector3(-13.4f, 55.0f, -2.5f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			parkingMeterTransform.setRotation(rotation);

			if (isparkingMeterDynamic)
				parkingMeterShape->calculateLocalInertia(parkingMeterMass, localparkingMeterInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			parkingMeterMotionState = new btDefaultMotionState(parkingMeterTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(parkingMeterMass, parkingMeterMotionState, parkingMeterShape, localparkingMeterInertia);
			parkingMeterBody = new btRigidBody(rbInfo);


			
		}

		#pragma endregion


		#pragma region Parking Meter Pole

		//parkingMeterPole Physics
		btCollisionShape* parkingMeterPoleShape = new btBoxShape(btVector3(0.6f, 4.7f, 8.6f));

		btTransform parkingMeterPoleTransform;

		btScalar parkingMeterPoleMass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isparkingMeterPoleDynamic = (parkingMeterPoleMass != 0.f);

		btVector3 localparkingMeterPoleInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* parkingMeterPoleMotionState;
		btRigidBody* parkingMeterPoleBody;


		GameObject parkingMeterPole = scene->CreateEntity("parkingMeterPole");
		{
			parkingMeterPole.emplace<MorphRenderer>();

			std::string IdlePrefix = "models/ParkingMeterPole/Parking_Meter_Pole_0000";
			std::string IdleFileName;

			for (int i = 1; i < 8; i++)
			{
				IdleFileName = IdlePrefix + std::to_string(i) + ".obj";

				parkingMeterPole.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(IdleFileName), 0);

			}

			parkingMeterPole.get<MorphRenderer>().SetFrameTime(0.7f, 0);
			parkingMeterPole.get<MorphRenderer>().SetLooping(true, 0);


			parkingMeterPole.get<MorphRenderer>().SetMesh(parkingMeterPole.get<MorphRenderer>().vao).SetMaterial(parkingMeterPoleMat);

			parkingMeterPole.get<Transform>().SetLocalPosition(-14.5f, 0.0f, -2.5f);
			parkingMeterPole.get<Transform>().SetLocalRotation(90.0f, 0.0f, 180.0f);
			parkingMeterPole.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(parkingMeterPole);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(parkingMeterPoleShape);

			parkingMeterPoleTransform.setIdentity();
			parkingMeterPoleTransform.setOrigin(btVector3(-19.6f, 50.0f, -2.2f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			parkingMeterPoleTransform.setRotation(rotation);

			if (isparkingMeterPoleDynamic)
				parkingMeterPoleShape->calculateLocalInertia(parkingMeterPoleMass, localparkingMeterPoleInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			parkingMeterPoleMotionState = new btDefaultMotionState(parkingMeterPoleTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(parkingMeterPoleMass, parkingMeterPoleMotionState, parkingMeterPoleShape, localparkingMeterPoleInertia);
			parkingMeterPoleBody = new btRigidBody(rbInfo);



		}

		#pragma endregion


		#pragma region Wall Button

		//wallButton Physics
		btCollisionShape* wallButtonShape = new btBoxShape(btVector3(1.1f, 4.6f, 2.4f));

		btTransform wallButtonTransform;

		btScalar wallButtonMass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool iswallButtonDynamic = (wallButtonMass != 0.f);

		btVector3 localwallButtonInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* wallButtonMotionState;
		btRigidBody* wallButtonBody;


		GameObject wallButton = scene2->CreateEntity("wallButton");
		{
			wallButton.emplace<MorphRenderer>();

			std::string IdlePrefix = "models/Button/Button_0";
			std::string IdleFileName;

			for (int i = 0; i < 4; i++)
			{
				IdleFileName = IdlePrefix + std::to_string(i) + ".obj";

				wallButton.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(IdleFileName), 0);

			}

			wallButton.get<MorphRenderer>().SetFrameTime(0.7f, 0);
			wallButton.get<MorphRenderer>().SetLooping(true, 0);


			wallButton.get<MorphRenderer>().SetMesh(wallButton.get<MorphRenderer>().vao).SetMaterial(buttonMat);

			wallButton.get<Transform>().SetLocalPosition(-14.5f, 0.0f, -2.5f);
			wallButton.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			wallButton.get<Transform>().SetLocalScale(4.0f, 4.0f, 4.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(wallButton);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(wallButtonShape);

			wallButtonTransform.setIdentity();
			wallButtonTransform.setOrigin(btVector3(-31.5f, -0.2f, -1.9f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			wallButtonTransform.setRotation(rotation);

			if (iswallButtonDynamic)
				wallButtonShape->calculateLocalInertia(wallButtonMass, localwallButtonInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			wallButtonMotionState = new btDefaultMotionState(wallButtonTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(wallButtonMass, wallButtonMotionState, wallButtonShape, localwallButtonInertia);
			wallButtonBody = new btRigidBody(rbInfo);
		}

		#pragma endregion


		#pragma region Fence Object

		GameObject FenceEnd = scene2->CreateEntity("Fence End");
		{

			VertexArrayObject::sptr FenceEndVAO = ObjLoader::LoadFromFile("models/hossain/Fence end.obj");
			FenceEnd.emplace<RendererComponent>().SetMesh(FenceEndVAO).SetMaterial(material13);
			FenceEnd.get<Transform>().SetLocalPosition(-12.0f, 41.3f, -4.5f);
			FenceEnd.get<Transform>().SetLocalRotation(90.0f, 0.0f, 32.0f);
			FenceEnd.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(FenceEnd);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject FenceEnd2 = scene2->CreateEntity("Fence End2");
		{

			VertexArrayObject::sptr FenceEnd2VAO = ObjLoader::LoadFromFile("models/hossain/Fence end.obj");
			FenceEnd2.emplace<RendererComponent>().SetMesh(FenceEnd2VAO).SetMaterial(material13);
			FenceEnd2.get<Transform>().SetLocalPosition(-45.0f, 62.7f, -4.5f);
			FenceEnd2.get<Transform>().SetLocalRotation(90.0f, 0.0f, -32.0f);
			FenceEnd2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(FenceEnd2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Rock Object

		GameObject Rock = scene2->CreateEntity("Rock");
		{

			VertexArrayObject::sptr RockVAO = ObjLoader::LoadFromFile("models/Fardeen/Rock.obj");
			Rock.emplace<RendererComponent>().SetMesh(RockVAO).SetMaterial(material17);
			Rock.get<Transform>().SetLocalPosition(-6.0f, 59.7f, -4.5f);
			Rock.get<Transform>().SetLocalRotation(90.0f, 0.0f, 70.0f);
			Rock.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Rock);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject Rock2 = scene2->CreateEntity("Rock");
		{

			VertexArrayObject::sptr Rock2VAO = ObjLoader::LoadFromFile("models/Fardeen/Rock.obj");
			Rock2.emplace<RendererComponent>().SetMesh(Rock2VAO).SetMaterial(material17);
			Rock2.get<Transform>().SetLocalPosition(-5.4f, 60.5f, -4.5f);
			Rock2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 70.0f);
			Rock2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Rock2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Snowman Object

		GameObject Snowman = scene2->CreateEntity("snowman");
		{

			VertexArrayObject::sptr SnowmanVAO = ObjLoader::LoadFromFile("models/Ethan/snowman.obj");
			Snowman.emplace<RendererComponent>().SetMesh(SnowmanVAO).SetMaterial(material27);
			Snowman.get<Transform>().SetLocalPosition(0.0f, 39.0f, -3.5f);
			Snowman.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			Snowman.get<Transform>().SetLocalScale(1.5f, 1.5f, 1.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Snowman);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject Snowman2 = scene2->CreateEntity("snowman2");
		{

			VertexArrayObject::sptr Snowman2VAO = ObjLoader::LoadFromFile("models/Ethan/snowman.obj");
			Snowman2.emplace<RendererComponent>().SetMesh(Snowman2VAO).SetMaterial(material27);
			Snowman2.get<Transform>().SetLocalPosition(-48.0f, 39.0f, -3.5f);
			Snowman2.get<Transform>().SetLocalRotation(90.0f, 0.0f, -45.0f);
			Snowman2.get<Transform>().SetLocalScale(1.5f, 1.5f, 1.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Snowman2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region SnowmanArray Object

		////// Row 1

		btCollisionShape* SnowmanA1Shape = new btCylinderShapeZ(btVector3(1.0f, 1.0f, 2.5f));

		btTransform SnowmanA1Transform;

		btScalar SnowmanA1Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isSnowmanA1Dynamic = (SnowmanA1Mass != 0.f);

		btVector3 localSnowmanA1Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* SnowmanA1MotionState;
		btRigidBody* SnowmanA1Body;

		VertexArrayObject::sptr SnowmanA1VAO = ObjLoader::LoadFromFile("models/Ethan/snowman.obj");

		GameObject SnowmanA1 = scene2->CreateEntity("snowman");
		{

			SnowmanA1.emplace<RendererComponent>().SetMesh(SnowmanA1VAO).SetMaterial(snowR1C1Mat);
			SnowmanA1.get<Transform>().SetLocalPosition(-11.0f, -60.0f, -3.9f);
			SnowmanA1.get<Transform>().SetLocalRotation(90.0f, 0.0f, -90.0f);
			SnowmanA1.get<Transform>().SetLocalScale(0.85f, 0.85f, 0.85f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(SnowmanA1);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(SnowmanA1Shape);
			SnowmanA1Transform.setIdentity();
			SnowmanA1Transform.setOrigin(btVector3(-11.0f, -10.0f, -3.9f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			SnowmanA1Transform.setRotation(rotation);
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setOrigin(glm2bt(island4.get<Transform>().GetLocalPosition()));
			//island4Transform.setIdentity();

			if (isSnowmanA1Dynamic)
				SnowmanA1Shape->calculateLocalInertia(SnowmanA1Mass, localSnowmanA1Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			SnowmanA1MotionState = new btDefaultMotionState(SnowmanA1Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(SnowmanA1Mass, SnowmanA1MotionState, SnowmanA1Shape, localSnowmanA1Inertia);
			SnowmanA1Body = new btRigidBody(rbInfo);

		}


		//////

		btCollisionShape* SnowmanA2Shape = new btCylinderShapeZ(btVector3(1.0f, 1.0f, 2.5f));

		btTransform SnowmanA2Transform;

		btScalar SnowmanA2Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isSnowmanA2Dynamic = (SnowmanA2Mass != 0.f);

		btVector3 localSnowmanA2Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* SnowmanA2MotionState;
		btRigidBody* SnowmanA2Body;

		GameObject SnowmanA2 = scene2->CreateEntity("snowman");
		{

			SnowmanA2.emplace<RendererComponent>().SetMesh(SnowmanA1VAO).SetMaterial(snowR1C2Mat);
			SnowmanA2.get<Transform>().SetLocalPosition(-11.0f, -50.0f, -3.9f);
			SnowmanA2.get<Transform>().SetLocalRotation(90.0f, 0.0f, -90.0f);
			SnowmanA2.get<Transform>().SetLocalScale(0.85f, 0.85f, 0.85f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(SnowmanA2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(SnowmanA2Shape);
			SnowmanA2Transform.setIdentity();
			SnowmanA2Transform.setOrigin(btVector3(-11.0f, 0.0f, -3.9f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			SnowmanA2Transform.setRotation(rotation);
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setOrigin(glm2bt(island4.get<Transform>().GetLocalPosition()));
			//island4Transform.setIdentity();

			if (isSnowmanA2Dynamic)
				SnowmanA2Shape->calculateLocalInertia(SnowmanA2Mass, localSnowmanA2Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			SnowmanA2MotionState = new btDefaultMotionState(SnowmanA2Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(SnowmanA2Mass, SnowmanA2MotionState, SnowmanA2Shape, localSnowmanA2Inertia);
			SnowmanA2Body = new btRigidBody(rbInfo);
		}


		//////

		btCollisionShape* SnowmanA3Shape = new btCylinderShapeZ(btVector3(1.0f, 1.0f, 2.5f));

		btTransform SnowmanA3Transform;

		btScalar SnowmanA3Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isSnowmanA3Dynamic = (SnowmanA3Mass != 0.f);

		btVector3 localSnowmanA3Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* SnowmanA3MotionState;
		btRigidBody* SnowmanA3Body;

		GameObject SnowmanA3 = scene2->CreateEntity("snowman");
		{

			SnowmanA3.emplace<RendererComponent>().SetMesh(SnowmanA1VAO).SetMaterial(snowR1C3Mat);
			SnowmanA3.get<Transform>().SetLocalPosition(-11.0f, -40.0f, -3.9f);
			SnowmanA3.get<Transform>().SetLocalRotation(90.0f, 0.0f, -90.0f);
			SnowmanA3.get<Transform>().SetLocalScale(0.85f, 0.85f, 0.85f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(SnowmanA3);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(SnowmanA3Shape);
			SnowmanA3Transform.setIdentity();
			SnowmanA3Transform.setOrigin(btVector3(-11.0f, 10.0f, -3.9f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			SnowmanA3Transform.setRotation(rotation);
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setOrigin(glm2bt(island4.get<Transform>().GetLocalPosition()));
			//island4Transform.setIdentity();

			if (isSnowmanA3Dynamic)
				SnowmanA3Shape->calculateLocalInertia(SnowmanA3Mass, localSnowmanA3Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			SnowmanA3MotionState = new btDefaultMotionState(SnowmanA3Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(SnowmanA3Mass, SnowmanA3MotionState, SnowmanA3Shape, localSnowmanA3Inertia);
			SnowmanA3Body = new btRigidBody(rbInfo);
		}


		////// Row 2

		btCollisionShape* SnowmanA4Shape = new btCylinderShapeZ(btVector3(1.0f, 1.0f, 2.5f));

		btTransform SnowmanA4Transform;

		btScalar SnowmanA4Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isSnowmanA4Dynamic = (SnowmanA4Mass != 0.f);

		btVector3 localSnowmanA4Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* SnowmanA4MotionState;
		btRigidBody* SnowmanA4Body;

		GameObject SnowmanA4 = scene2->CreateEntity("snowman");
		{
			SnowmanA4.emplace<RendererComponent>().SetMesh(SnowmanA1VAO).SetMaterial(snowR2C1Mat);
			SnowmanA4.get<Transform>().SetLocalPosition(-16.0f, -60.0f, -3.9f);
			SnowmanA4.get<Transform>().SetLocalRotation(90.0f, 0.0f, -90.0f);
			SnowmanA4.get<Transform>().SetLocalScale(0.85f, 0.85f, 0.85f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(SnowmanA4);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(SnowmanA4Shape);
			SnowmanA4Transform.setIdentity();
			SnowmanA4Transform.setOrigin(btVector3(-16.0f, -10.0f, -3.9f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			SnowmanA4Transform.setRotation(rotation);
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setOrigin(glm2bt(island4.get<Transform>().GetLocalPosition()));
			//island4Transform.setIdentity();

			if (isSnowmanA4Dynamic)
				SnowmanA4Shape->calculateLocalInertia(SnowmanA4Mass, localSnowmanA4Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			SnowmanA4MotionState = new btDefaultMotionState(SnowmanA4Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(SnowmanA4Mass, SnowmanA4MotionState, SnowmanA4Shape, localSnowmanA4Inertia);
			SnowmanA4Body = new btRigidBody(rbInfo);
		}


		//////

		btCollisionShape* SnowmanA5Shape = new btCylinderShapeZ(btVector3(1.0f, 1.0f, 2.5f));

		btTransform SnowmanA5Transform;

		btScalar SnowmanA5Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isSnowmanA5Dynamic = (SnowmanA5Mass != 0.f);

		btVector3 localSnowmanA5Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* SnowmanA5MotionState;
		btRigidBody* SnowmanA5Body;

		GameObject SnowmanA5 = scene2->CreateEntity("snowman");
		{
			SnowmanA5.emplace<RendererComponent>().SetMesh(SnowmanA1VAO).SetMaterial(snowR2C2Mat);
			SnowmanA5.get<Transform>().SetLocalPosition(-16.0f, -50.0f, -3.9f);
			SnowmanA5.get<Transform>().SetLocalRotation(90.0f, 0.0f, -90.0f);
			SnowmanA5.get<Transform>().SetLocalScale(0.85f, 0.85f, 0.85f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(SnowmanA5);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(SnowmanA5Shape);
			SnowmanA5Transform.setIdentity();
			SnowmanA5Transform.setOrigin(btVector3(-16.0f, 00.0f, -3.9f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			SnowmanA5Transform.setRotation(rotation);
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setOrigin(glm2bt(island4.get<Transform>().GetLocalPosition()));
			//island4Transform.setIdentity();

			if (isSnowmanA5Dynamic)
				SnowmanA5Shape->calculateLocalInertia(SnowmanA5Mass, localSnowmanA5Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			SnowmanA5MotionState = new btDefaultMotionState(SnowmanA5Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(SnowmanA5Mass, SnowmanA5MotionState, SnowmanA5Shape, localSnowmanA5Inertia);
			SnowmanA5Body = new btRigidBody(rbInfo);
		}


		//////

		btCollisionShape* SnowmanA6Shape = new btCylinderShapeZ(btVector3(1.0f, 1.0f, 2.5f));

		btTransform SnowmanA6Transform;

		btScalar SnowmanA6Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isSnowmanA6Dynamic = (SnowmanA6Mass != 0.f);

		btVector3 localSnowmanA6Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* SnowmanA6MotionState;
		btRigidBody* SnowmanA6Body;

		GameObject SnowmanA6 = scene2->CreateEntity("snowman");
		{
			SnowmanA6.emplace<RendererComponent>().SetMesh(SnowmanA1VAO).SetMaterial(snowR2C3Mat);
			SnowmanA6.get<Transform>().SetLocalPosition(-16.0f, -40.0f, -3.9f);
			SnowmanA6.get<Transform>().SetLocalRotation(90.0f, 0.0f, -90.0f);
			SnowmanA6.get<Transform>().SetLocalScale(0.85f, 0.85f, 0.85f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(SnowmanA6);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(SnowmanA6Shape);
			SnowmanA6Transform.setIdentity();
			SnowmanA6Transform.setOrigin(btVector3(-16.0f, 10.0f, -3.9f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			SnowmanA6Transform.setRotation(rotation);
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setOrigin(glm2bt(island4.get<Transform>().GetLocalPosition()));
			//island4Transform.setIdentity();

			if (isSnowmanA6Dynamic)
				SnowmanA6Shape->calculateLocalInertia(SnowmanA6Mass, localSnowmanA6Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			SnowmanA6MotionState = new btDefaultMotionState(SnowmanA6Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(SnowmanA6Mass, SnowmanA6MotionState, SnowmanA6Shape, localSnowmanA6Inertia);
			SnowmanA6Body = new btRigidBody(rbInfo);
		}


		////// Row 3

		btCollisionShape* SnowmanA7Shape = new btCylinderShapeZ(btVector3(1.0f, 1.0f, 2.5f));

		btTransform SnowmanA7Transform;

		btScalar SnowmanA7Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isSnowmanA7Dynamic = (SnowmanA7Mass != 0.f);

		btVector3 localSnowmanA7Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* SnowmanA7MotionState;
		btRigidBody* SnowmanA7Body;

		GameObject SnowmanA7 = scene2->CreateEntity("snowman");
		{
			SnowmanA7.emplace<RendererComponent>().SetMesh(SnowmanA1VAO).SetMaterial(snowR3C1Mat);
			SnowmanA7.get<Transform>().SetLocalPosition(-21.0f, -60.0f, -3.9f);
			SnowmanA7.get<Transform>().SetLocalRotation(90.0f, 0.0f, -90.0f);
			SnowmanA7.get<Transform>().SetLocalScale(0.85f, 0.85f, 0.85f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(SnowmanA7);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(SnowmanA7Shape);
			SnowmanA7Transform.setIdentity();
			SnowmanA7Transform.setOrigin(btVector3(-21.0f, -10.0f, -3.9f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			SnowmanA7Transform.setRotation(rotation);
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setOrigin(glm2bt(island4.get<Transform>().GetLocalPosition()));
			//island4Transform.setIdentity();

			if (isSnowmanA7Dynamic)
				SnowmanA7Shape->calculateLocalInertia(SnowmanA7Mass, localSnowmanA7Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			SnowmanA7MotionState = new btDefaultMotionState(SnowmanA7Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(SnowmanA7Mass, SnowmanA7MotionState, SnowmanA7Shape, localSnowmanA7Inertia);
			SnowmanA7Body = new btRigidBody(rbInfo);
		}


		//////

		btCollisionShape* SnowmanA8Shape = new btCylinderShapeZ(btVector3(1.0f, 1.0f, 2.5f));

		btTransform SnowmanA8Transform;

		btScalar SnowmanA8Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isSnowmanA8Dynamic = (SnowmanA8Mass != 0.f);

		btVector3 localSnowmanA8Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* SnowmanA8MotionState;
		btRigidBody* SnowmanA8Body;

		GameObject SnowmanA8 = scene2->CreateEntity("snowman");
		{
			SnowmanA8.emplace<RendererComponent>().SetMesh(SnowmanA1VAO).SetMaterial(snowR3C2Mat);
			SnowmanA8.get<Transform>().SetLocalPosition(-21.0f, -50.0f, -3.9f);
			SnowmanA8.get<Transform>().SetLocalRotation(90.0f, 0.0f, -90.0f);
			SnowmanA8.get<Transform>().SetLocalScale(0.85f, 0.85f, 0.85f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(SnowmanA8);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(SnowmanA8Shape);
			SnowmanA8Transform.setIdentity();
			SnowmanA8Transform.setOrigin(btVector3(-21.0f, 0.0f, -3.9f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			SnowmanA8Transform.setRotation(rotation);
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setOrigin(glm2bt(island4.get<Transform>().GetLocalPosition()));
			//island4Transform.setIdentity();

			if (isSnowmanA8Dynamic)
				SnowmanA8Shape->calculateLocalInertia(SnowmanA8Mass, localSnowmanA8Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			SnowmanA8MotionState = new btDefaultMotionState(SnowmanA8Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(SnowmanA8Mass, SnowmanA8MotionState, SnowmanA8Shape, localSnowmanA8Inertia);
			SnowmanA8Body = new btRigidBody(rbInfo);
		}


		//////

		btCollisionShape* SnowmanA9Shape = new btCylinderShapeZ(btVector3(1.0f, 1.0f, 2.5f));

		btTransform SnowmanA9Transform;

		btScalar SnowmanA9Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isSnowmanA9Dynamic = (SnowmanA9Mass != 0.f);

		btVector3 localSnowmanA9Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* SnowmanA9MotionState;
		btRigidBody* SnowmanA9Body;

		GameObject SnowmanA9 = scene2->CreateEntity("snowman");
		{
			SnowmanA9.emplace<RendererComponent>().SetMesh(SnowmanA1VAO).SetMaterial(snowR3C3Mat);
			SnowmanA9.get<Transform>().SetLocalPosition(-21.0f, -40.0f, -3.9f);
			SnowmanA9.get<Transform>().SetLocalRotation(90.0f, 0.0f, -90.0f);
			SnowmanA9.get<Transform>().SetLocalScale(0.85f, 0.85f, 0.85f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(SnowmanA9);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(SnowmanA9Shape);
			SnowmanA9Transform.setIdentity();
			SnowmanA9Transform.setOrigin(btVector3(-21.0f, 10.0f, -3.9f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 0.0f);
			SnowmanA9Transform.setRotation(rotation);
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island4Transform.setOrigin(glm2bt(island4.get<Transform>().GetLocalPosition()));
			//island4Transform.setIdentity();

			if (isSnowmanA9Dynamic)
				SnowmanA9Shape->calculateLocalInertia(SnowmanA9Mass, localSnowmanA9Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			SnowmanA9MotionState = new btDefaultMotionState(SnowmanA9Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(SnowmanA9Mass, SnowmanA9MotionState, SnowmanA9Shape, localSnowmanA9Inertia);
			SnowmanA9Body = new btRigidBody(rbInfo);
		}


		////// Row 4

		//btCollisionShape* SnowmanA10Shape = new btCylinderShapeZ(btVector3(1.0f, 1.0f, 2.5f));

		//btTransform SnowmanA10Transform;

		//btScalar SnowmanA10Mass(0.f);

		////rigidbody is dynamic if and only if mass is non zero, otherwise static
		//bool isSnowmanA10Dynamic = (SnowmanA10Mass != 0.f);

		//btVector3 localSnowmanA10Inertia(0, 0, 0);

		////using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		//btDefaultMotionState* SnowmanA10MotionState;
		//btRigidBody* SnowmanA10Body;

		//GameObject SnowmanA10 = scene2->CreateEntity("snowman");
		//{

		//	VertexArrayObject::sptr SnowmanA10VAO = ObjLoader::LoadFromFile("models/Ethan/snowman.obj");
		//	SnowmanA10.emplace<RendererComponent>().SetMesh(SnowmanA10VAO).SetMaterial(material27);
		//	SnowmanA10.get<Transform>().SetLocalPosition(-26.0f, -60.0f, -3.9f);
		//	SnowmanA10.get<Transform>().SetLocalRotation(90.0f, 0.0f, -90.0f);
		//	SnowmanA10.get<Transform>().SetLocalScale(0.85f, 0.85f, 0.85f);
		//	BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(SnowmanA10);
		//	//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

		//	//Collision Stuff
		//	collisionShapes.push_back(SnowmanA10Shape);
		//	SnowmanA10Transform.setIdentity();
		//	SnowmanA10Transform.setOrigin(btVector3(-26.0f, -10.0f, -3.9f));
		//	btQuaternion rotation;
		//	rotation.setEuler(0.0f, 0.0f, 0.0f);
		//	SnowmanA10Transform.setRotation(rotation);
		//	//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
		//	//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
		//	//island4Transform.setOrigin(glm2bt(island4.get<Transform>().GetLocalPosition()));
		//	//island4Transform.setIdentity();

		//	if (isSnowmanA10Dynamic)
		//		SnowmanA10Shape->calculateLocalInertia(SnowmanA10Mass, localSnowmanA10Inertia);

		//	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		//	SnowmanA10MotionState = new btDefaultMotionState(SnowmanA10Transform);
		//	btRigidBody::btRigidBodyConstructionInfo rbInfo(SnowmanA10Mass, SnowmanA10MotionState, SnowmanA10Shape, localSnowmanA10Inertia);
		//	SnowmanA10Body = new btRigidBody(rbInfo);
		//}


		////////

		//btCollisionShape* SnowmanA11Shape = new btCylinderShapeZ(btVector3(1.0f, 1.0f, 2.5f));

		//btTransform SnowmanA11Transform;

		//btScalar SnowmanA11Mass(0.f);

		////rigidbody is dynamic if and only if mass is non zero, otherwise static
		//bool isSnowmanA11Dynamic = (SnowmanA11Mass != 0.f);

		//btVector3 localSnowmanA11Inertia(0, 0, 0);

		////using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		//btDefaultMotionState* SnowmanA11MotionState;
		//btRigidBody* SnowmanA11Body;

		//GameObject SnowmanA11 = scene2->CreateEntity("snowman");
		//{

		//	VertexArrayObject::sptr SnowmanA11VAO = ObjLoader::LoadFromFile("models/Ethan/snowman.obj");
		//	SnowmanA11.emplace<RendererComponent>().SetMesh(SnowmanA11VAO).SetMaterial(material27);
		//	SnowmanA11.get<Transform>().SetLocalPosition(-26.0f, -50.0f, -3.9f);
		//	SnowmanA11.get<Transform>().SetLocalRotation(90.0f, 0.0f, -90.0f);
		//	SnowmanA11.get<Transform>().SetLocalScale(0.85f, 0.85f, 0.85f);
		//	BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(SnowmanA11);
		//	//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

		//	//Collision Stuff
		//	collisionShapes.push_back(SnowmanA11Shape);
		//	SnowmanA11Transform.setIdentity();
		//	SnowmanA11Transform.setOrigin(btVector3(-26.0f, 0.0f, -3.9f));
		//	btQuaternion rotation;
		//	rotation.setEuler(0.0f, 0.0f, 0.0f);
		//	SnowmanA11Transform.setRotation(rotation);
		//	//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
		//	//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
		//	//island4Transform.setOrigin(glm2bt(island4.get<Transform>().GetLocalPosition()));
		//	//island4Transform.setIdentity();

		//	if (isSnowmanA11Dynamic)
		//		SnowmanA11Shape->calculateLocalInertia(SnowmanA11Mass, localSnowmanA11Inertia);

		//	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		//	SnowmanA11MotionState = new btDefaultMotionState(SnowmanA11Transform);
		//	btRigidBody::btRigidBodyConstructionInfo rbInfo(SnowmanA11Mass, SnowmanA11MotionState, SnowmanA11Shape, localSnowmanA11Inertia);
		//	SnowmanA11Body = new btRigidBody(rbInfo);
		//}


		////////

		//btCollisionShape* SnowmanA12Shape = new btCylinderShapeZ(btVector3(1.0f, 1.0f, 2.5f));

		//btTransform SnowmanA12Transform;

		//btScalar SnowmanA12Mass(0.f);

		////rigidbody is dynamic if and only if mass is non zero, otherwise static
		//bool isSnowmanA12Dynamic = (SnowmanA12Mass != 0.f);

		//btVector3 localSnowmanA12Inertia(0, 0, 0);

		////using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		//btDefaultMotionState* SnowmanA12MotionState;
		//btRigidBody* SnowmanA12Body;

		//GameObject SnowmanA12 = scene2->CreateEntity("snowman");
		//{

		//	VertexArrayObject::sptr SnowmanA12VAO = ObjLoader::LoadFromFile("models/Ethan/snowman.obj");
		//	SnowmanA12.emplace<RendererComponent>().SetMesh(SnowmanA12VAO).SetMaterial(material27);
		//	SnowmanA12.get<Transform>().SetLocalPosition(-26.0f, -40.0f, -3.9f);
		//	SnowmanA12.get<Transform>().SetLocalRotation(90.0f, 0.0f, -90.0f);
		//	SnowmanA12.get<Transform>().SetLocalScale(0.85f, 0.85f, 0.85f);
		//	BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(SnowmanA12);
		//	//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

		//	//Collision Stuff
		//	collisionShapes.push_back(SnowmanA12Shape);
		//	SnowmanA12Transform.setIdentity();
		//	SnowmanA12Transform.setOrigin(btVector3(-26.0f, 10.0f, -3.9f));
		//	btQuaternion rotation;
		//	rotation.setEuler(0.0f, 0.0f, 0.0f);
		//	SnowmanA12Transform.setRotation(rotation);
		//	//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
		//	//island4Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
		//	//island4Transform.setOrigin(glm2bt(island4.get<Transform>().GetLocalPosition()));
		//	//island4Transform.setIdentity();

		//	if (isSnowmanA12Dynamic)
		//		SnowmanA12Shape->calculateLocalInertia(SnowmanA12Mass, localSnowmanA12Inertia);

		//	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		//	SnowmanA12MotionState = new btDefaultMotionState(SnowmanA12Transform);
		//	btRigidBody::btRigidBodyConstructionInfo rbInfo(SnowmanA12Mass, SnowmanA12MotionState, SnowmanA12Shape, localSnowmanA12Inertia);
		//	SnowmanA12Body = new btRigidBody(rbInfo);
		//}


		//////
		

		#pragma endregion
		

		#pragma region Sled Object

		GameObject Sled = scene2->CreateEntity("sled");
		{

			VertexArrayObject::sptr SledVAO = ObjLoader::LoadFromFile("models/Ethan/sled.obj");
			Sled.emplace<RendererComponent>().SetMesh(SledVAO).SetMaterial(material28);
			Sled.get<Transform>().SetLocalPosition(-33.0f, 61.5f, -4.0f);
			Sled.get<Transform>().SetLocalRotation(90.0f, 0.0f, 200.0f);
			Sled.get<Transform>().SetLocalScale(3.0f, 4.0f, 4.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Sled);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject Sled2 = scene2->CreateEntity("sled");
		{

			VertexArrayObject::sptr Sled2VAO = ObjLoader::LoadFromFile("models/Ethan/sled.obj");
			Sled2.emplace<RendererComponent>().SetMesh(Sled2VAO).SetMaterial(material28);
			Sled2.get<Transform>().SetLocalPosition(4.0f, 41.5f, -4.0f);
			Sled2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Sled2.get<Transform>().SetLocalScale(3.0f, 4.0f, 4.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Sled2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Ice Crystal Object

		GameObject IceCrystal = scene2->CreateEntity("crystal");
		{

			VertexArrayObject::sptr IceCrystalVAO = ObjLoader::LoadFromFile("models/Ethan/ice crystal.obj");
			IceCrystal.emplace<RendererComponent>().SetMesh(IceCrystalVAO).SetMaterial(material29);
			IceCrystal.get<Transform>().SetLocalPosition(-48.0f, 50.0f, -2.0f);
			IceCrystal.get<Transform>().SetLocalRotation(90.0f, 0.0f, 200.0f);
			IceCrystal.get<Transform>().SetLocalScale(0.5f, 0.5f, 0.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(IceCrystal);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Portal (Level 2) Object

		GameObject Portal2 = scene2->CreateEntity("Portal2");
		{
			Portal2.emplace<MorphRenderer>();

			//Idle Animation
			std::string IdlePrefix = "models/Portal/portal_updated0";
			std::string IdleFileName;

			for (int i = 0; i <= 4; i++)
			{
				IdleFileName = IdlePrefix + std::to_string(i) + ".obj";

				Portal2.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(IdleFileName), 0);

			}
			Portal2.get<MorphRenderer>().SetFrameTime(0.7f, 0);
			Portal2.get<MorphRenderer>().SetLooping(true, 0);

			VertexArrayObject::sptr Portal2VAO = ObjLoader::LoadFromFile("models/portal.obj");
			Portal2.emplace<RendererComponent>().SetMesh(Portal2VAO).SetMaterial(material8);
			Portal2.get<Transform>().SetLocalPosition(-52.0f, 50.0f, 1.0f);
			Portal2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Portal2.get<Transform>().SetLocalScale(3.0f, 3.0f, 3.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Portal2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Bridge (Level 2) Object

		btCollisionShape* bridge2Shape = new btBoxShape(btVector3(3.0f, 2.0f, 0.8f));

		btTransform bridge2Transform;

		btScalar bridge2Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isbridge2Dynamic = (bridge2Mass != 0.f);

		btVector3 localbridge2Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* bridge2MotionState;
		btRigidBody* bridge2Body;

		GameObject Bridge2 = scene2->CreateEntity("Middle Bridge");
		{
			VertexArrayObject::sptr Bridge2VAO = ObjLoader::LoadFromFile("models/hossain/NewBridge.obj");
			Bridge2.emplace<RendererComponent>().SetMesh(Bridge2VAO).SetMaterial(material13);
			Bridge2.get<Transform>().SetLocalPosition(-18.0f, 50.0f, -6.5f);
			Bridge2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Bridge2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Bridge2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(bridge2Shape);
			bridge2Transform.setIdentity();
			bridge2Transform.setOrigin(btVector3(-20.0f, 50.0f, -5.15f));
			btQuaternion rotation;
			//rotation.setEuler(90.0f, 0.0f, 90.0f);
			//bridge2Transform.setRotation(rotation);
			//bridge2Transform.setRotation(btQuaternion(btVector3(0, 0, 1), btScalar(3.)));
			//island1Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island1Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island1Transform.setOrigin(glm2bt(island1.get<Transform>().GetLocalPosition()));
			//island1Transform.setIdentity();

			if (isbridge2Dynamic)
				bridge2Shape->calculateLocalInertia(bridge2Mass, localbridge2Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			bridge2MotionState = new btDefaultMotionState(bridge2Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(bridge2Mass, bridge2MotionState, bridge2Shape, localbridge2Inertia);
			bridge2Body = new btRigidBody(rbInfo);



			//add the body to the dynamics world
			//dynamicsWorld->addRigidBody(bridge2Body, 1, 1);
		}


		btCollisionShape* bridge3Shape = new btBoxShape(btVector3(6.0f, 2.0f, 0.8f));

		btTransform bridge3Transform;

		btScalar bridge3Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isbridge3Dynamic = (bridge3Mass != 0.f);

		btVector3 localbridge3Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* bridge3MotionState;
		btRigidBody* bridge3Body;

		GameObject Bridge3 = scene2->CreateEntity("Left Bridge");
		{
			VertexArrayObject::sptr Bridge3VAO = ObjLoader::LoadFromFile("models/hossain/NewBridge.obj");
			Bridge3.emplace<RendererComponent>().SetMesh(Bridge3VAO).SetMaterial(material13);
			Bridge3.get<Transform>().SetLocalPosition(-10.0f, 26.0f, -5.0f);
			Bridge3.get<Transform>().SetLocalRotation(90.0f, 0.0f, 150.0f);
			Bridge3.get<Transform>().SetLocalScale(1.0f, 1.0f, 2.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Bridge3);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(bridge3Shape);
			bridge3Transform.setIdentity();
			bridge3Transform.setOrigin(btVector3(-10.0f, 26.0f, -5.15f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 1.06f);
			bridge3Transform.setRotation(rotation);
			//bridge3Transform.setRotation(btQuaternion(btVector3(0, 0, 1), btScalar(3.)));
			//island1Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island1Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island1Transform.setOrigin(glm2bt(island1.get<Transform>().GetLocalPosition()));
			//island1Transform.setIdentity();

			if (isbridge3Dynamic)
				bridge3Shape->calculateLocalInertia(bridge3Mass, localbridge3Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			bridge3MotionState = new btDefaultMotionState(bridge3Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(bridge3Mass, bridge3MotionState, bridge3Shape, localbridge3Inertia);
			bridge3Body = new btRigidBody(rbInfo);

		}


		btCollisionShape* bridge4Shape = new btBoxShape(btVector3(8.0f, 2.0f, 0.8f));

		btTransform bridge4Transform;

		btScalar bridge4Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isbridge4Dynamic = (bridge4Mass != 0.f);

		btVector3 localbridge4Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* bridge4MotionState;
		btRigidBody* bridge4Body;

		GameObject Bridge4 = scene2->CreateEntity("Right Bridge");
		{
			VertexArrayObject::sptr Bridge4VAO = ObjLoader::LoadFromFile("models/hossain/NewBridge.obj");
			Bridge4.emplace<RendererComponent>().SetMesh(Bridge4VAO).SetMaterial(material13);
			Bridge4.get<Transform>().SetLocalPosition(-10.0f, 74.0f, -5.0f);
			Bridge4.get<Transform>().SetLocalRotation(90.0f, 0.0f, 30.0f);
			Bridge4.get<Transform>().SetLocalScale(1.0f, 1.0f, 2.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Bridge4);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(bridge4Shape);
			bridge4Transform.setIdentity();
			bridge4Transform.setOrigin(btVector3(-10.8f, 75.2f, -5.15f));
			btQuaternion rotation;
			rotation.setEuler(0.0f, 0.0f, 2.1f);
			bridge4Transform.setRotation(rotation);
			//bridge4Transform.setRotation(btQuaternion(btVector3(0, 0, 1), btScalar(3.)));
			//island1Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island1Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island1Transform.setOrigin(glm2bt(island1.get<Transform>().GetLocalPosition()));
			//island1Transform.setIdentity();

			if (isbridge4Dynamic)
				bridge4Shape->calculateLocalInertia(bridge4Mass, localbridge4Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			bridge4MotionState = new btDefaultMotionState(bridge4Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(bridge4Mass, bridge4MotionState, bridge4Shape, localbridge4Inertia);
			bridge4Body = new btRigidBody(rbInfo);
		}

		#pragma endregion


		#pragma region PineTree (Level 2) Object

		GameObject PineTree1 = scene2->CreateEntity("PineTree1");
		{
			VertexArrayObject::sptr PineTree1VAO = ObjLoader::LoadFromFile("models/PineTree/PineTree0.obj");
			PineTree1.emplace<RendererComponent>().SetMesh(PineTree1VAO).SetMaterial(material30);
			PineTree1.get<Transform>().SetLocalPosition(-10.5f, 61.5f, -5.0f);
			PineTree1.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			PineTree1.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(PineTree1);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject PineTree2 = scene2->CreateEntity("PineTree2");
		{
			VertexArrayObject::sptr PineTree2VAO = ObjLoader::LoadFromFile("models/PineTree/PineTree0.obj");
			PineTree2.emplace<RendererComponent>().SetMesh(PineTree2VAO).SetMaterial(material30);
			PineTree2.get<Transform>().SetLocalPosition(-30.0f, 41.0f, -5.0f);
			PineTree2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			PineTree2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(PineTree2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject PineTree3 = scene2->CreateEntity("PineTree2");
		{
			VertexArrayObject::sptr PineTree3VAO = ObjLoader::LoadFromFile("models/PineTree/PineTree0.obj");
			PineTree3.emplace<RendererComponent>().SetMesh(PineTree3VAO).SetMaterial(material30);
			PineTree3.get<Transform>().SetLocalPosition(8.0f, 60.0f, -4.8f);
			PineTree3.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			PineTree3.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(PineTree3);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Tree2 (Level 2) Object

		GameObject Tree2 = scene2->CreateEntity("Tree2");
		{
			VertexArrayObject::sptr Tree2VAO = ObjLoader::LoadFromFile("models/tree1.2.obj");
			Tree2.emplace<RendererComponent>().SetMesh(Tree2VAO).SetMaterial(material31);
			Tree2.get<Transform>().SetLocalPosition(-23.5f, 17.5f, -4.8f);
			Tree2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Tree2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Tree2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject Tree2_2 = scene2->CreateEntity("Tree2");
		{
			VertexArrayObject::sptr Tree2_2VAO = ObjLoader::LoadFromFile("models/tree1.2.obj");
			Tree2_2.emplace<RendererComponent>().SetMesh(Tree2_2VAO).SetMaterial(material31);
			Tree2_2.get<Transform>().SetLocalPosition(-28.5f, 85.5f, -4.8f);
			Tree2_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Tree2_2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Tree2_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Tree3 (Level 2) Object

		GameObject Tree3 = scene2->CreateEntity("Tree3");
		{
			VertexArrayObject::sptr Tree3VAO = ObjLoader::LoadFromFile("models/tree2.obj");
			Tree3.emplace<RendererComponent>().SetMesh(Tree3VAO).SetMaterial(material32);
			Tree3.get<Transform>().SetLocalPosition(-23.5f, -13.5f, -4.8f);
			Tree3.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Tree3.get<Transform>().SetLocalScale(1.2f, 1.2f, 1.2f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Tree3);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject Tree3_2 = scene2->CreateEntity("Tree3");
		{
			VertexArrayObject::sptr Tree3_2VAO = ObjLoader::LoadFromFile("models/tree2.obj");
			Tree3_2.emplace<RendererComponent>().SetMesh(Tree3_2VAO).SetMaterial(material32);
			Tree3_2.get<Transform>().SetLocalPosition(-37.5f, -2.5f, -4.8f);
			Tree3_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Tree3_2.get<Transform>().SetLocalScale(1.2f, 1.2f, 1.2f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Tree3_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject Tree3_3 = scene2->CreateEntity("Tree3");
		{
			VertexArrayObject::sptr Tree3_3VAO = ObjLoader::LoadFromFile("models/tree2.obj");
			Tree3_3.emplace<RendererComponent>().SetMesh(Tree3_3VAO).SetMaterial(material32);
			Tree3_3.get<Transform>().SetLocalPosition(-28.5f, 106.5f, -4.8f);
			Tree3_3.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Tree3_3.get<Transform>().SetLocalScale(1.2f, 1.2f, 1.2f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Tree3_3);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject Tree3_4 = scene2->CreateEntity("Tree3");
		{
			VertexArrayObject::sptr Tree3_4VAO = ObjLoader::LoadFromFile("models/tree2.obj");
			Tree3_4.emplace<RendererComponent>().SetMesh(Tree3_4VAO).SetMaterial(material32);
			Tree3_4.get<Transform>().SetLocalPosition(-15.5f, 104.5f, -4.8f);
			Tree3_4.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Tree3_4.get<Transform>().SetLocalScale(1.2f, 1.2f, 1.2f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Tree3_4);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}



		#pragma endregion


		#pragma region Camera Objects

		// Create an object to be our camera
		GameObject cameraObject2 = scene2->CreateEntity("Camera");
		{
			cameraObject2.get<Transform>().SetLocalPosition(8.0f, 0.0f, 4.0f).LookAt(glm::vec3(0));

			// We'll make our camera a component of the camera object
			Camera& camera = cameraObject2.emplace<Camera>();// Camera::Create();
			camera.SetPosition(glm::vec3(6.0f, 3.0f, 8.0f));
			camera.SetUp(glm::vec3(0, 0, 1));
			//camera.LookAt(glm::vec3(-5.0f, 0.0f, 8.0f));
			camera.LookAt(glm::vec3(0));
			camera.SetFovDegrees(90.0f); // Set an initial FOV
			camera.SetOrthoHeight(3.0f);
			//BehaviourBinding::Bind<CameraControlBehaviour>(cameraObject2);
		}

		// Create an object to be our orthographic camera
		GameObject orthoCameraObject2 = scene->CreateEntity("OrthoCamera");
		{
			//cameraObject.get<Transform>().SetLocalPosition(0, 3, 3).LookAt(glm::vec3(0, 0, 0));
			//Looking at water and terrain positions
			//cameraObject.get<Transform>().SetLocalPosition(-43.89, 25.74, 3.89).LookAt(glm::vec3(-40.69, -0.53, -7.83));
			orthoCameraObject2.get<Transform>().SetLocalPosition(8.0f, 0.0f, 4.0f).LookAt(glm::vec3(0));

			// We'll make our camera a component of the camera object
			Camera& orthoCamera = orthoCameraObject2.emplace<Camera>();// Camera::Create();
			orthoCamera.SetPosition(glm::vec3(0, 3, 3));
			orthoCamera.SetUp(glm::vec3(0, 0, 1));
			orthoCamera.LookAt(glm::vec3(0));
			orthoCamera.SetFovDegrees(90.0f); // Set an initial FOV
			orthoCamera.SetOrthoHeight(3.0f);
			orthoCameraObject2.get<Camera>().ToggleOrtho();
			//BehaviourBinding::Bind<CameraControlBehaviour>(orthoCameraObject);
		}

		#pragma endregion


		#pragma region Post-Processing Effect Objects

		glfwGetWindowSize(BackendHandler::window, &width, &height);

		GameObject shadowBufferObject2 = scene2->CreateEntity("Shadow Buffer");
		{
			shadowBuffer2 = &shadowBufferObject2.emplace<Framebuffer>();
			shadowBuffer2->AddDepthTarget();
			shadowBuffer2->Init(shadowWidth, shadowHeight);
		}

		GameObject framebufferObject2 = scene2->CreateEntity("Basic Effect");
		{
			basicEffect2 = &framebufferObject2.emplace<PostEffect>();
			basicEffect2->Init(width, height);
		}

		GameObject greyscaleEffectObject2 = scene2->CreateEntity("Greyscale Effect");
		{
			greyscaleEffect2 = &greyscaleEffectObject2.emplace<GreyscaleEffect>();
			greyscaleEffect2->Init(width, height);
		}
		scene2Effects.push_back(greyscaleEffect2);

		GameObject warmEffectObject2 = scene2->CreateEntity("Warm Color Correction Effect");
		{
			warmEffect2 = &warmEffectObject2.emplace<WarmEffect>();
			warmEffect2->Init(width, height);
		}
		scene2Effects.push_back(warmEffect2);

		GameObject coolEffectObject2 = scene2->CreateEntity("Cool Color Correction Effect");
		{
			coolEffect2 = &coolEffectObject2.emplace<CoolEffect>();
			coolEffect2->Init(width, height);
		}
		scene2Effects.push_back(coolEffect2);

		GameObject colorCorrectionEffectObject2 = scene2->CreateEntity("Color Correction Effect");
		{
			colorCorrectionEffect2 = &colorCorrectionEffectObject2.emplace<ColorCorrectionEffect>();
			colorCorrectionEffect2->Init(width, height);
		}
		scene2Effects.push_back(colorCorrectionEffect2);

		GameObject bloomEffectObject2 = scene2->CreateEntity("Color Correction Effect");
		{
			bloomEffect2 = &bloomEffectObject2.emplace<BloomEffect>();
			bloomEffect2->Init(width, height);
		}
		scene2Effects.push_back(bloomEffect2);

		GameObject vignetteEffectObject2 = scene2->CreateEntity("Vignette Effect");
		{
			vignetteEffect2 = &vignetteEffectObject2.emplace<VignetteEffect>();
			vignetteEffect2->Init(width, height);
		}
		scene2Effects.push_back(vignetteEffect2);

		GameObject pixelateEffectObject2 = scene2->CreateEntity("Pixelate Effect");
		{
			pixelateEffect2 = &pixelateEffectObject2.emplace<PixelateEffect>();
			pixelateEffect2->Init(width, height);
		}
		scene2Effects.push_back(pixelateEffect2);

		#pragma endregion


		#pragma region Sprite Objects

		VertexArrayObject::sptr heartVao2;

		GameObject heart1Obj2 = scene2->CreateEntity("HeartSprite");
		{
			GLfloat vertices[] = { -1, -1, 0, // bottom left corner
						   -1,  1, 0, // top left corner
							1,  1, 0, // top right corner
							1, -1, 0 }; // bottom right corner


			GLuint elements[] = {
					0, 1, 2,
					2, 3, 0
			};


			heartVao2 = NotObjLoader::LoadFromFile("sprite.notobj");
			heart1Obj2.emplace<Sprite>().SetMesh(heartVao2).SetMaterial(heartMat);
			//heart1Obj.emplace<RendererComponent>().SetMesh(heart1Vao).SetMaterial(heart1Mat);
			heart1Obj2.get<Transform>().SetLocalPosition(0.0f, 0.0f, 5.0f);
			heart1Obj2.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			//heart1Obj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 180.0f);
			heart1Obj2.get<Transform>().SetLocalScale(0.07f, 0.07f, 0.07f);
			//heart1Obj.get<Transform>().LookAt(glm::vec3(0));

		}

		GameObject heart2Obj2 = scene2->CreateEntity("HeartSprite");
		{
			GLfloat vertices[] = { -1, -1, 0, // bottom left corner
						   -1,  1, 0, // top left corner
							1,  1, 0, // top right corner
							1, -1, 0 }; // bottom right corner


			GLuint elements[] = {
					0, 1, 2,
					2, 3, 0
			};


			heartVao2 = NotObjLoader::LoadFromFile("sprite.notobj");
			heart2Obj2.emplace<Sprite>().SetMesh(heartVao2).SetMaterial(heartMat);
			//heart1Obj.emplace<RendererComponent>().SetMesh(heart1Vao).SetMaterial(heart1Mat);
			heart2Obj2.get<Transform>().SetLocalPosition(0.0f, 0.0f, 5.0f);
			heart2Obj2.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			//heart1Obj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 180.0f);
			heart2Obj2.get<Transform>().SetLocalScale(0.07f, 0.07f, 0.07f);
			//heart1Obj.get<Transform>().LookAt(glm::vec3(0));

		}

		GameObject heart3Obj2 = scene2->CreateEntity("HeartSprite");
		{
			GLfloat vertices[] = { -1, -1, 0, // bottom left corner
						   -1,  1, 0, // top left corner
							1,  1, 0, // top right corner
							1, -1, 0 }; // bottom right corner


			GLuint elements[] = {
					0, 1, 2,
					2, 3, 0
			};


			heartVao2 = NotObjLoader::LoadFromFile("sprite.notobj");
			heart3Obj2.emplace<Sprite>().SetMesh(heartVao2).SetMaterial(heartMat);
			//heart1Obj.emplace<RendererComponent>().SetMesh(heart1Vao).SetMaterial(heart1Mat);
			heart3Obj2.get<Transform>().SetLocalPosition(0.0f, 0.0f, 5.0f);
			heart3Obj2.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			//heart1Obj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 180.0f);
			heart3Obj2.get<Transform>().SetLocalScale(0.07f, 0.07f, 0.07f);
			//heart1Obj.get<Transform>().LookAt(glm::vec3(0));

		}

		#pragma endregion


		#pragma region Dialogue Sprites

		VertexArrayObject::sptr dialogueLevel2VAO;

		GameObject dialogueL2 = scene2->CreateEntity("Dialogue Level 2");
		{
			dialogueLevel2VAO = NotObjLoader::LoadFromFile("sprite.notobj");
			dialogueL2.emplace<Sprite>().SetMesh(dialogueLevel2VAO).SetMaterial(dialogueMatL2D1);
			//dialogueL1D1.emplace<RendererComponent>().SetMesh(heart1Vao).SetMaterial(heart1Mat);
			dialogueL2.get<Transform>().SetLocalPosition(0.0f, 0.0f, 5.0f);
			dialogueL2.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			//dialogueL1D1.get<Transform>().SetLocalRotation(0.0f, 180.0f, 180.0f);
			dialogueL2.get<Transform>().SetLocalScale(0.223f, 0.223f, 0.223f);
			//dialogueL1D1.get<Transform>().LookAt(glm::vec3(0));

		}

		#pragma endregion


		#pragma region Interaction Sprites

		VertexArrayObject::sptr interactionVAO2;

		GameObject interactionObj2 = scene2->CreateEntity("Interaction Sprite");
		{
			interactionVAO2 = NotObjLoader::LoadFromFile("sprite.notobj");
			interactionObj2.emplace<Sprite>().SetMesh(interactionVAO2).SetMaterial(talkMat);
			interactionObj2.get<Transform>().SetLocalPosition(0.0f, 0.0f, 5.0f);
			interactionObj2.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			interactionObj2.get<Transform>().SetLocalScale(0.223f, 0.223f, 0.223f);
		}

		#pragma endregion


		#pragma endregion
		///////////////////////////////////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////// Scene 4 Generation //////////////////////////////////////////////////
		#pragma region Scene 4 (Level 3)

		// Create a scene, and set it to be the active scene in the application
		GameScene::sptr scene4 = GameScene::Create("Scene 4 (Level 3)");

		// We can create a group ahead of time to make iterating on the group faster
		entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<Transform>, RendererComponent> renderGroup4 =
			scene4->Registry().group<RendererComponent>(entt::get_t<Transform>());
		
		#pragma region Menu

		VertexArrayObject::sptr lastmenuVAO;

		GameObject lastMenuObj = scene4->CreateEntity("MenuPlay");
		{
			GLfloat vertices[] = { -1, -1, 0, // bottom left corner
						   -1,  1, 0, // top left corner
							1,  1, 0, // top right corner
							1, -1, 0 }; // bottom right corner


			GLuint elements[] = {
					0, 1, 2,
					2, 3, 0
			};


			lastmenuVAO = NotObjLoader::LoadFromFile("sprite.notobj");
			lastMenuObj.emplace<Sprite>().SetMesh(lastmenuVAO).SetMaterial(cutsceneMat1);
			//playMenuObj.emplace<RendererComponent>().SetMesh(playMenuVao).SetMaterial(playMenuMat);
			lastMenuObj.get<Transform>().SetLocalPosition(-5.0f, 0.0f, 10.0f);
			lastMenuObj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			//playMenuObj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 180.0f);
			lastMenuObj.get<Transform>().SetLocalScale(0.6f, 0.6f, 0.6f);
			//playMenuObj.get<Transform>().LookAt(glm::vec3(0));

		}

		#pragma endregion

		#pragma region Camera Object

		// Create an object to be our camera
		GameObject cameraObject4 = scene4->CreateEntity("Camera");
		{
			cameraObject4.get<Transform>().SetLocalPosition(8.0f, 0.0f, 4.0f).LookAt(glm::vec3(0));

			// We'll make our camera a component of the camera object
			Camera& camera = cameraObject4.emplace<Camera>();// Camera::Create();
			camera.SetPosition(glm::vec3(6.0f, 3.0f, 8.0f));
			camera.SetUp(glm::vec3(0, 0, 1));
			//camera.LookAt(glm::vec3(-5.0f, 0.0f, 8.0f));
			camera.LookAt(glm::vec3(0));
			camera.SetFovDegrees(90.0f); // Set an initial FOV
			camera.SetOrthoHeight(3.0f);
			//BehaviourBinding::Bind<CameraControlBehaviour>(cameraObject4);
		}

		// Create an object to be our orthographic camera
		GameObject orthoCameraObject4 = scene->CreateEntity("OrthoCamera");
		{
			//cameraObject.get<Transform>().SetLocalPosition(0, 3, 3).LookAt(glm::vec3(0, 0, 0));
			//Looking at water and terrain positions
			//cameraObject.get<Transform>().SetLocalPosition(-43.89, 25.74, 3.89).LookAt(glm::vec3(-40.69, -0.53, -7.83));
			orthoCameraObject4.get<Transform>().SetLocalPosition(8.0f, 0.0f, 4.0f).LookAt(glm::vec3(0));

			// We'll make our camera a component of the camera object
			Camera& orthoCamera = orthoCameraObject4.emplace<Camera>();// Camera::Create();
			orthoCamera.SetPosition(glm::vec3(0, 3, 3));
			orthoCamera.SetUp(glm::vec3(0, 0, 1));
			orthoCamera.LookAt(glm::vec3(0));
			orthoCamera.SetFovDegrees(90.0f); // Set an initial FOV
			orthoCamera.SetOrthoHeight(3.0f);
			orthoCameraObject4.get<Camera>().ToggleOrtho();
			//BehaviourBinding::Bind<CameraControlBehaviour>(orthoCameraObject);
		}

		#pragma endregion


		#pragma region FireIsland1 Object

		btCollisionShape* Fireisland1Shape = new btCylinderShapeZ(btVector3(20.0f, 20.0f, 2.0f));

		btTransform Fireisland1Transform;

		btScalar Fireisland1Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool Fireisisland1Dynamic = (Fireisland1Mass != 0.f);

		btVector3 Firelocalisland1Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* Fireisland1MotionState;
		btRigidBody* Fireisland1Body;

		GameObject Fireisland1 = scene4->CreateEntity("Fireisland1");
		{
			VertexArrayObject::sptr Fireisland1VAO = ObjLoader::LoadFromFile("models/FireIsland.obj");
			Fireisland1.emplace<RendererComponent>().SetMesh(Fireisland1VAO).SetMaterial(material21);
			Fireisland1.get<Transform>().SetLocalPosition(0.0f, 0.0f, -4.5f);
			Fireisland1.get<Transform>().SetLocalRotation(-90.0f, 180.0f, 0.0f);
			Fireisland1.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Fireisland1);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);


			//Collision Stuff
			collisionShapes.push_back(Fireisland1Shape);
			Fireisland1Transform.setIdentity();
			Fireisland1Transform.setOrigin(btVector3(0.0f, 0.0f, -4.3f));
			btQuaternion rotation;
			//rotation.setEuler(0.0f, 0.0f, 0.0f);
			//island1Transform.setRotation(rotation);
			//island1Transform.setRotation(btQuaternion(btVector3(0, 0, 1), btScalar(3.)));
			//island1Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island1Transform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(-1.57)));
			//island1Transform.setOrigin(glm2bt(island1.get<Transform>().GetLocalPosition()));
			//island1Transform.setIdentity();

			if (Fireisisland1Dynamic)
				Fireisland1Shape->calculateLocalInertia(Fireisland1Mass, Firelocalisland1Inertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			Fireisland1MotionState = new btDefaultMotionState(Fireisland1Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(Fireisland1Mass, Fireisland1MotionState, Fireisland1Shape, Firelocalisland1Inertia);
			Fireisland1Body = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			//dynamicsWorld->addRigidBody(Fireisland1Body);

		}


		#pragma endregion


		#pragma region FireIsland2 Object

		btCollisionShape* Fireisland2Shape = new btCylinderShapeZ(btVector3(20.0f, 20.0f, 2.0f));

		btTransform Fireisland2Transform;

		btScalar Fireisland2Mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool Fireisisland2Dynamic = (Fireisland2Mass != 0.f);

		btVector3 Firelocalisland2Inertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* Fireisland2MotionState;
		btRigidBody* Fireisland2Body;


		GameObject Fireisland2 = scene4->CreateEntity("FireIsland2");
		{

			//VertexArrayObject::sptr FireIsland2VAO = ObjLoader::LoadFromFile("models/FireIslandDone.obj");
			//Fireisland2.emplace<RendererComponent>().SetMesh(FireIsland2VAO).SetMaterial(material21);
			//Fireisland2.get<Transform>().SetLocalPosition(-55.0f, 0.0f, 3.0f);
			//Fireisland2.get<Transform>().SetLocalRotation(-90.0f, 180.0f, 0.0f);
			//Fireisland2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			//BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Fireisland2);
			////SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			////Collision Stuff
			//collisionShapes.push_back(Fireisland2Shape);
			//Fireisland2Transform.setIdentity();
			//Fireisland2Transform.setOrigin(btVector3(-55.0f, 0.0f, 3.0f));
			//btQuaternion rotation;

			//if (Fireisisland2Dynamic)
			//	Fireisland2Shape->calculateLocalInertia(Fireisland2Mass, Firelocalisland2Inertia);

			////using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			//Fireisland2MotionState = new btDefaultMotionState(Fireisland2Transform);
			//btRigidBody::btRigidBodyConstructionInfo rbInfo(Fireisland2Mass, Fireisland2MotionState, Fireisland2Shape, Firelocalisland2Inertia);
			//Fireisland2Body = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			//dynamicsWorld->addRigidBody(Fireisland2Body);
		}

		#pragma endregion


		#pragma region Volcano1 Object
		
		GameObject Volcano1 = scene4->CreateEntity("Volcano1");
		{
			VertexArrayObject::sptr Volcano1VAO = ObjLoader::LoadFromFile("models/Volcano.obj");
			Volcano1.emplace<RendererComponent>().SetMesh(Volcano1VAO).SetMaterial(material22);
			Volcano1.get<Transform>().SetLocalPosition(-19.0f, -20.0f, -3.0f);
			Volcano1.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			Volcano1.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Volcano1);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Volcano2 Object

		GameObject Volcano2 = scene4->CreateEntity("Volcano2");
		{
			VertexArrayObject::sptr Volcano2VAO = ObjLoader::LoadFromFile("models/Volcano.obj");
			Volcano2.emplace<RendererComponent>().SetMesh(Volcano2VAO).SetMaterial(material22);
			Volcano2.get<Transform>().SetLocalPosition(-35.0f, 35.0f, -5.0f);
			Volcano2.get<Transform>().SetLocalRotation(90.0f, 0.0f, -90.0f);
			Volcano2.get<Transform>().SetLocalScale(3.0f, 3.0f, 3.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Volcano2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Volcano3 Object

		GameObject Volcano3 = scene4->CreateEntity("Volcano3");
		{
			VertexArrayObject::sptr Volcano3VAO = ObjLoader::LoadFromFile("models/Volcano.obj");
			Volcano3.emplace<RendererComponent>().SetMesh(Volcano3VAO).SetMaterial(material22);
			Volcano3.get<Transform>().SetLocalPosition(-92.0f, -7.8f, -3.0f);
			Volcano3.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			Volcano3.get<Transform>().SetLocalScale(2.2f, 2.2f, 2.2f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Volcano3);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region DeadTree Object

		GameObject DeadTree = scene4->CreateEntity("DeadTree");
		{
			VertexArrayObject::sptr DeadTreeVAO = ObjLoader::LoadFromFile("models/DeadBerchTree.obj");
			DeadTree.emplace<RendererComponent>().SetMesh(DeadTreeVAO).SetMaterial(material23);
			DeadTree.get<Transform>().SetLocalPosition(-9.0f, 10.0f, -3.0f);
			DeadTree.get<Transform>().SetLocalRotation(90.0f, 0.0f, 30.0f);
			DeadTree.get<Transform>().SetLocalScale(2.0f, 2.0f, 2.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(DeadTree);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}
		#pragma endregion


		#pragma region DeadTree2 Object

		GameObject DeadTree2 = scene4->CreateEntity("DeadTree2");
		{
			VertexArrayObject::sptr DeadTreeVAO2 = ObjLoader::LoadFromFile("models/DeadBerchTree.obj");
			DeadTree2.emplace<RendererComponent>().SetMesh(DeadTreeVAO2).SetMaterial(material23);
			DeadTree2.get<Transform>().SetLocalPosition(-3.0f, -11.0f, -3.0f);
			DeadTree2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			DeadTree2.get<Transform>().SetLocalScale(1.8f, 1.8f, 1.8f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(DeadTree2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}
		#pragma endregion

		
		#pragma region DeadTree3 Object
		GameObject DeadTree3 = scene4->CreateEntity("DeadTree3");
		{
			//VertexArrayObject::sptr DeadTreeVAO3 = ObjLoader::LoadFromFile("models/DeadBerchTree.obj");
			//DeadTree3.emplace<RendererComponent>().SetMesh(DeadTreeVAO3).SetMaterial(material23);
			//DeadTree3.get<Transform>().SetLocalPosition(-50.0f, -13.0f, 4.7f);
			//DeadTree3.get<Transform>().SetLocalRotation(90.0f, 0.0f, -30.0f);
			//DeadTree3.get<Transform>().SetLocalScale(1.8f, 1.8f, 1.8f);
			//BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(DeadTree3);
			////SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}
		#pragma endregion


		#pragma region CreatureBones Object
		GameObject CreatureBones = scene4->CreateEntity("CreatureBones");
		{
			VertexArrayObject::sptr CreatureBonesVAO = ObjLoader::LoadFromFile("models/BoneCreature.obj");
			CreatureBones.emplace<RendererComponent>().SetMesh(CreatureBonesVAO).SetMaterial(material24);
			CreatureBones.get<Transform>().SetLocalPosition(2.0f, -9.5f, -2.5f);
			CreatureBones.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			CreatureBones.get<Transform>().SetLocalScale(1.3f, 1.3f, 1.3f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(CreatureBones);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject CreatureBones2 = scene4->CreateEntity("CreatureBones2");
		{
			//VertexArrayObject::sptr CreatureBonesVAO2 = ObjLoader::LoadFromFile("models/BoneCreature.obj");
			//CreatureBones2.emplace<RendererComponent>().SetMesh(CreatureBonesVAO2).SetMaterial(material24);
			//CreatureBones2.get<Transform>().SetLocalPosition(-60.0f, 11.0f, 5.0f);
			//CreatureBones2.get<Transform>().SetLocalRotation(90.0f, 0.0f, -150.0f);
			//CreatureBones2.get<Transform>().SetLocalScale(1.3f, 1.3f, 1.3f);
			//BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(CreatureBones2);
			////SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Barrel Object

		GameObject Barrel = scene4->CreateEntity("Barrel");
		{
			VertexArrayObject::sptr BarrelVAO = ObjLoader::LoadFromFile("models/Barrel_Unwrapped.obj");
			Barrel.emplace<RendererComponent>().SetMesh(BarrelVAO).SetMaterial(material25);
			Barrel.get<Transform>().SetLocalPosition(-2.0f, 9.5f, -2.3f);
			Barrel.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			Barrel.get<Transform>().SetLocalScale(0.7f, 0.7f, 0.7f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Barrel);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		GameObject Barrel2 = scene4->CreateEntity("Barrel2");
		{
			VertexArrayObject::sptr BarrelVAO2 = ObjLoader::LoadFromFile("models/Barrel_Unwrapped.obj");
			Barrel2.emplace<RendererComponent>().SetMesh(BarrelVAO2).SetMaterial(material25);
			Barrel2.get<Transform>().SetLocalPosition(-0.1f, 10.0f, -2.3f);
			Barrel2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			Barrel2.get<Transform>().SetLocalScale(0.7f, 0.7f, 0.7f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Barrel2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Fire Platform Objects

		btCollisionShape* FirePlatformShape = new btBoxShape(btVector3(1.7f, 2.0f, 0.2f));

		btCollisionObject* firePlatform1Obj = new btCollisionObject();
		firePlatform1Obj->setCollisionShape(FirePlatformShape);

		btTransform FirePlatform1Transform;

		btScalar FirePlatformMass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool FirePlatformisDynamic = (FirePlatformMass != 0.f);

		btVector3 localFirePlatformInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* FirePlatform1MotionState;
		btRigidBody* FirePlatform1Body;
		//FirePlatform1Body->isKinematicObject();
		

		GameObject FirePlatform = scene4->CreateEntity("FirePlatform");
		{
			VertexArrayObject::sptr FirePlatformVAO = ObjLoader::LoadFromFile("models/FirePlatform.obj");
			FirePlatform.emplace<RendererComponent>().SetMesh(FirePlatformVAO).SetMaterial(material26);
			FirePlatform.get<Transform>().SetLocalPosition(-25.0f, 0.5f, 0.5f);
			FirePlatform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			FirePlatform.get<Transform>().SetLocalScale(1.2f, 1.2f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(FirePlatform);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(FirePlatformShape);
			FirePlatform1Transform.setIdentity();
			FirePlatform1Transform.setOrigin(btVector3(-25.0f, -5.5f, 0.5f));
			
			if (FirePlatformisDynamic)
				FirePlatformShape->calculateLocalInertia(FirePlatformMass, localFirePlatformInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			FirePlatform1MotionState = new btDefaultMotionState(FirePlatform1Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(FirePlatformMass, FirePlatform1MotionState, FirePlatformShape, localFirePlatformInertia);
			FirePlatform1Body = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			//dynamicsWorld->addRigidBody(Fireisland1Body);
		}

		btDefaultMotionState* FirePlatform2MotionState;
		btTransform FirePlatform2Transform;
		btRigidBody* FirePlatform2Body;
		//FirePlatform2Body->isKinematicObject();

		GameObject FirePlatform2 = scene4->CreateEntity("FirePlatform2");
		{
			VertexArrayObject::sptr FirePlatform2VAO = ObjLoader::LoadFromFile("models/FirePlatform.obj");
			FirePlatform2.emplace<RendererComponent>().SetMesh(FirePlatform2VAO).SetMaterial(material26);
			FirePlatform2.get<Transform>().SetLocalPosition(-33.0f, 0.5f, 3.0f);
			FirePlatform2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			FirePlatform2.get<Transform>().SetLocalScale(1.2f, 1.2f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(FirePlatform2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(FirePlatformShape);
			FirePlatform2Transform.setIdentity();
			FirePlatform2Transform.setOrigin(btVector3(-30.0f, 0.5f, 3.0f));

			if (FirePlatformisDynamic)
				FirePlatformShape->calculateLocalInertia(FirePlatformMass, localFirePlatformInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			FirePlatform2MotionState = new btDefaultMotionState(FirePlatform2Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(FirePlatformMass, FirePlatform2MotionState, FirePlatformShape, localFirePlatformInertia);
			FirePlatform2Body = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			//dynamicsWorld->addRigidBody(Fireisland2Body);
		}


		btDefaultMotionState* FirePlatform3MotionState;
		btTransform FirePlatform3Transform;
		btRigidBody* FirePlatform3Body;
		//FirePlatform3Body->isKinematicObject();

		GameObject FirePlatform3 = scene4->CreateEntity("FirePlatform3");
		{
			VertexArrayObject::sptr FirePlatform3VAO = ObjLoader::LoadFromFile("models/FirePlatform.obj");
			FirePlatform3.emplace<RendererComponent>().SetMesh(FirePlatform3VAO).SetMaterial(material26);
			FirePlatform3.get<Transform>().SetLocalPosition(-33.0f, 0.5f, 3.0f);
			FirePlatform3.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			FirePlatform3.get<Transform>().SetLocalScale(1.2f, 1.2f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(FirePlatform3);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(FirePlatformShape);
			FirePlatform3Transform.setIdentity();
			FirePlatform3Transform.setOrigin(btVector3(-35.0f, 6.5f, 5.5f));

			if (FirePlatformisDynamic)
				FirePlatformShape->calculateLocalInertia(FirePlatformMass, localFirePlatformInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			FirePlatform3MotionState = new btDefaultMotionState(FirePlatform3Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(FirePlatformMass, FirePlatform3MotionState, FirePlatformShape, localFirePlatformInertia);
			FirePlatform3Body = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			//dynamicsWorld->addRigidBody(Fireisland2Body);
		}


		btDefaultMotionState* FirePlatform4MotionState;
		btTransform FirePlatform4Transform;
		btRigidBody* FirePlatform4Body;
		//FirePlatform4Body->isKinematicObject();

		GameObject FirePlatform4 = scene4->CreateEntity("FirePlatform4");
		{
			VertexArrayObject::sptr FirePlatform4VAO = ObjLoader::LoadFromFile("models/FirePlatform.obj");
			FirePlatform4.emplace<RendererComponent>().SetMesh(FirePlatform4VAO).SetMaterial(material26);
			FirePlatform4.get<Transform>().SetLocalPosition(-33.0f, 0.5f, 3.0f);
			FirePlatform4.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			FirePlatform4.get<Transform>().SetLocalScale(1.2f, 1.2f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(FirePlatform4);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(FirePlatformShape);
			FirePlatform4Transform.setIdentity();
			FirePlatform4Transform.setOrigin(btVector3(-40.0f, 0.5f, 8.0f));

			if (FirePlatformisDynamic)
				FirePlatformShape->calculateLocalInertia(FirePlatformMass, localFirePlatformInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			FirePlatform4MotionState = new btDefaultMotionState(FirePlatform4Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(FirePlatformMass, FirePlatform4MotionState, FirePlatformShape, localFirePlatformInertia);
			FirePlatform4Body = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			//dynamicsWorld->addRigidBody(Fireisland2Body);
		}


		btDefaultMotionState* FirePlatform5MotionState;
		btTransform FirePlatform5Transform;
		btRigidBody* FirePlatform5Body;
		//FirePlatform5Body->isKinematicObject();

		GameObject FirePlatform5 = scene4->CreateEntity("FirePlatform5");
		{
			VertexArrayObject::sptr FirePlatform5VAO = ObjLoader::LoadFromFile("models/FirePlatform.obj");
			FirePlatform5.emplace<RendererComponent>().SetMesh(FirePlatform5VAO).SetMaterial(material26);
			FirePlatform5.get<Transform>().SetLocalPosition(-33.0f, 0.5f, 3.0f);
			FirePlatform5.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			FirePlatform5.get<Transform>().SetLocalScale(1.2f, 1.2f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(FirePlatform5);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(FirePlatformShape);
			FirePlatform5Transform.setIdentity();
			FirePlatform5Transform.setOrigin(btVector3(-45.0f, -5.5f, 10.5f));

			if (FirePlatformisDynamic)
				FirePlatformShape->calculateLocalInertia(FirePlatformMass, localFirePlatformInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			FirePlatform5MotionState = new btDefaultMotionState(FirePlatform5Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(FirePlatformMass, FirePlatform5MotionState, FirePlatformShape, localFirePlatformInertia);
			FirePlatform5Body = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			//dynamicsWorld->addRigidBody(Fireisland2Body);
		}


		btDefaultMotionState* FirePlatform6MotionState;
		btTransform FirePlatform6Transform;
		btRigidBody* FirePlatform6Body;
		//FirePlatform6Body->isKinematicObject();

		GameObject FirePlatform6 = scene4->CreateEntity("FirePlatform6");
		{
			VertexArrayObject::sptr FirePlatform6VAO = ObjLoader::LoadFromFile("models/FirePlatform.obj");
			FirePlatform6.emplace<RendererComponent>().SetMesh(FirePlatform6VAO).SetMaterial(material26);
			FirePlatform6.get<Transform>().SetLocalPosition(-33.0f, 0.5f, 3.0f);
			FirePlatform6.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			FirePlatform6.get<Transform>().SetLocalScale(1.2f, 1.2f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(FirePlatform6);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(FirePlatformShape);
			FirePlatform6Transform.setIdentity();
			FirePlatform6Transform.setOrigin(btVector3(-50.0f, 0.5f, 13.0f));

			if (FirePlatformisDynamic)
				FirePlatformShape->calculateLocalInertia(FirePlatformMass, localFirePlatformInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			FirePlatform6MotionState = new btDefaultMotionState(FirePlatform6Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(FirePlatformMass, FirePlatform6MotionState, FirePlatformShape, localFirePlatformInertia);
			FirePlatform6Body = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			//dynamicsWorld->addRigidBody(Fireisland2Body);
		}

		#pragma endregion


		#pragma region Portal2 Object

		GameObject Portal3 = scene4->CreateEntity("Portal3");
		{
			Portal3.emplace<MorphRenderer>();

			//Idle Animation
			std::string IdlePrefix = "models/Portal/portal_updated0";
			std::string IdleFileName;

			for (int i = 0; i <= 4; i++)
			{
				IdleFileName = IdlePrefix + std::to_string(i) + ".obj";

				Portal3.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(IdleFileName), 0);

			}
			Portal3.get<MorphRenderer>().SetFrameTime(0.7f, 0);
			Portal3.get<MorphRenderer>().SetLooping(true, 0);

			VertexArrayObject::sptr FirePlatformVAO = ObjLoader::LoadFromFile("models/portal.obj");
			Portal3.emplace<RendererComponent>().SetMesh(FirePlatformVAO).SetMaterial(material8);
			Portal3.get<Transform>().SetLocalPosition(-73.0f, 0.0f, 9.5f);
			Portal3.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Portal3.get<Transform>().SetLocalScale(3.0f, 3.0f, 3.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Portal3);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}



		#pragma endregion


		#pragma region Post Effects
		
		//glfwGetWindowSize(BackendHandler::window, &width, &height);

		/*GameObject shadowBufferObject3 = scene4->CreateEntity("Shadow Buffer");
		{
			shadowBuffer3 = &shadowBufferObject3.emplace<Framebuffer>();
			shadowBuffer3->AddDepthTarget();
			shadowBuffer3->Init(shadowWidth, shadowHeight);
		}*/

		GameObject framebufferObject3 = scene4->CreateEntity("Basic Effect");
		{
			basicEffect3 = &framebufferObject3.emplace<PostEffect>();
			basicEffect3->Init(width, height);
		}

		GameObject greyscaleEffectObject3 = scene4->CreateEntity("Greyscale Effect");
		{
			greyscaleEffect3 = &greyscaleEffectObject3.emplace<GreyscaleEffect>();
			greyscaleEffect3->Init(width, height);
		}
		scene3Effects.push_back(greyscaleEffect3);

		GameObject warmEffectObject3 = scene4->CreateEntity("Warm Color Correction Effect");
		{
			warmEffect3 = &warmEffectObject3.emplace<WarmEffect>();
			warmEffect3->Init(width, height);
		}
		scene3Effects.push_back(warmEffect3);

		GameObject coolEffectObject3 = scene4->CreateEntity("Cool Color Correction Effect");
		{
			coolEffect3 = &coolEffectObject3.emplace<CoolEffect>();
			coolEffect3->Init(width, height);
		}
		scene3Effects.push_back(coolEffect3);

		GameObject colorCorrectionEffectObject3 = scene4->CreateEntity("Color Correction Effect");
		{
			colorCorrectionEffect3 = &colorCorrectionEffectObject3.emplace<ColorCorrectionEffect>();
			colorCorrectionEffect3->Init(width, height);
		}
		scene3Effects.push_back(colorCorrectionEffect3);

		GameObject bloomEffectObject3 = scene4->CreateEntity("Color Correction Effect");
		{
			bloomEffect3 = &bloomEffectObject3.emplace<BloomEffect>();
			bloomEffect3->Init(width, height);
		}
		scene3Effects.push_back(bloomEffect3);

		GameObject vignetteEffectObject3 = scene4->CreateEntity("Vignette Effect");
		{
			vignetteEffect3 = &vignetteEffectObject3.emplace<VignetteEffect>();
			vignetteEffect3->Init(width, height);
		}
		scene3Effects.push_back(vignetteEffect3);

		GameObject pixelateEffectObject3 = scene4->CreateEntity("Pixelate Effect");
		{
			pixelateEffect3 = &pixelateEffectObject3.emplace<PixelateEffect>();
			pixelateEffect3->Init(width, height);
		}
		scene3Effects.push_back(pixelateEffect3);

		#pragma endregion


		#pragma region Sprite Objects

		VertexArrayObject::sptr heartVao3;

		GameObject heart1Obj3 = scene4->CreateEntity("HeartSprite4");
		{
			GLfloat vertices[] = { -1, -1, 0, // bottom left corner
						   -1,  1, 0, // top left corner
							1,  1, 0, // top right corner
							1, -1, 0 }; // bottom right corner


			GLuint elements[] = {
					0, 1, 2,
					2, 3, 0
			};


			heartVao3 = NotObjLoader::LoadFromFile("sprite.notobj");
			heart1Obj3.emplace<Sprite>().SetMesh(heartVao3).SetMaterial(heartMat);
			//heart1Obj.emplace<RendererComponent>().SetMesh(heart1Vao).SetMaterial(heart1Mat);
			heart1Obj3.get<Transform>().SetLocalPosition(0.0f, 0.0f, 5.0f);
			heart1Obj3.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			//heart1Obj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 180.0f);
			heart1Obj3.get<Transform>().SetLocalScale(0.07f, 0.07f, 0.07f);
			//heart1Obj.get<Transform>().LookAt(glm::vec3(0));

		}

		GameObject heart2Obj3 = scene4->CreateEntity("HeartSprite4");
		{
			GLfloat vertices[] = { -1, -1, 0, // bottom left corner
						   -1,  1, 0, // top left corner
							1,  1, 0, // top right corner
							1, -1, 0 }; // bottom right corner


			GLuint elements[] = {
					0, 1, 2,
					2, 3, 0
			};


			heartVao3 = NotObjLoader::LoadFromFile("sprite.notobj");
			heart2Obj3.emplace<Sprite>().SetMesh(heartVao3).SetMaterial(heartMat);
			//heart1Obj.emplace<RendererComponent>().SetMesh(heart1Vao).SetMaterial(heart1Mat);
			heart2Obj3.get<Transform>().SetLocalPosition(0.0f, 0.0f, 5.0f);
			heart2Obj3.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			//heart1Obj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 180.0f);
			heart2Obj3.get<Transform>().SetLocalScale(0.07f, 0.07f, 0.07f);
			//heart1Obj.get<Transform>().LookAt(glm::vec3(0));

		}

		GameObject heart3Obj3 = scene4->CreateEntity("HeartSprite4");
		{
			GLfloat vertices[] = { -1, -1, 0, // bottom left corner
						   -1,  1, 0, // top left corner
							1,  1, 0, // top right corner
							1, -1, 0 }; // bottom right corner


			GLuint elements[] = {
					0, 1, 2,
					2, 3, 0
			};


			heartVao3 = NotObjLoader::LoadFromFile("sprite.notobj");
			heart3Obj3.emplace<Sprite>().SetMesh(heartVao3).SetMaterial(heartMat);
			//heart1Obj.emplace<RendererComponent>().SetMesh(heart1Vao).SetMaterial(heart1Mat);
			heart3Obj3.get<Transform>().SetLocalPosition(0.0f, 0.0f, 5.0f);
			heart3Obj3.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			//heart1Obj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 180.0f);
			heart3Obj3.get<Transform>().SetLocalScale(0.07f, 0.07f, 0.07f);
			//heart1Obj.get<Transform>().LookAt(glm::vec3(0));

		}

		#pragma endregion

		
		#pragma endregion
		///////////////////////////////////////////////////////////////////////////////////////////////////////////

		
		///////////////////////////////////// Menu Scene //////////////////////////////////////////////////////////
		#pragma region Menu Scene

		// Create a scene, and set it to be the active scene in the application
		GameScene::sptr scene0 = GameScene::Create("Menu Scene");
		Application::Instance().ActiveScene = scene0;

		// We can create a group ahead of time to make iterating on the group faster
		entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<Transform>, RendererComponent> renderGroup0 =
			scene0->Registry().group<RendererComponent>(entt::get_t<Transform>());

		#pragma region Materials

		ShaderMaterial::sptr playMenuMat = ShaderMaterial::Create();
		playMenuMat->Shader = spriteShader;
		playMenuMat->Set("s_Diffuse", playMenuDiffuse);
		playMenuMat->RenderLayer = 0;

		ShaderMaterial::sptr controlsMenuMat = ShaderMaterial::Create();
		controlsMenuMat->Shader = spriteShader;
		controlsMenuMat->Set("s_Diffuse", controlsMenuDiffuse);
		controlsMenuMat->RenderLayer = 1;

		ShaderMaterial::sptr exitMenuMat = ShaderMaterial::Create();
		exitMenuMat->Shader = spriteShader;
		exitMenuMat->Set("s_Diffuse", exitMenuDiffuse);
		exitMenuMat->RenderLayer = 2;

		ShaderMaterial::sptr controlsMat = ShaderMaterial::Create();
		controlsMat->Shader = spriteShader;
		controlsMat->Set("s_Diffuse", controlsDiffuse);
		controlsMat->RenderLayer = 3;

		ShaderMaterial::sptr story1Mat = ShaderMaterial::Create();
		story1Mat->Shader = spriteShader;
		story1Mat->Set("s_Diffuse", story1Diffuse);
		story1Mat->RenderLayer = 3;

		ShaderMaterial::sptr story2Mat = ShaderMaterial::Create();
		story2Mat->Shader = spriteShader;
		story2Mat->Set("s_Diffuse", story2Diffuse);
		story2Mat->RenderLayer = 3;

		ShaderMaterial::sptr story3Mat = ShaderMaterial::Create();
		story3Mat->Shader = spriteShader;
		story3Mat->Set("s_Diffuse", story3Diffuse);
		story3Mat->RenderLayer = 3;


		#pragma endregion


		#pragma region Island Objects

		GameObject island1_2 = scene0->CreateEntity("Island1");
		{
			VertexArrayObject::sptr Island1VAO = ObjLoader::LoadFromFile("models/plains_island.obj");
			island1_2.emplace<RendererComponent>().SetMesh(Island1VAO).SetMaterial(material1);
			island1_2.get<Transform>().SetLocalPosition(0.0f, 0.0f, -4.5f);
			island1_2.get<Transform>().SetLocalRotation(-90.0f, 180.0f, 0.0f);
			island1_2.get<Transform>().SetLocalScale(1.5f, 1.5f, 1.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(island1_2);


			GameObject island2_2 = scene0->CreateEntity("Island2");


			island2_2.emplace<RendererComponent>().SetMesh(Island1VAO).SetMaterial(material1);
			island2_2.get<Transform>().SetLocalPosition(-35.0f, 0.0f, -4.5f);
			island2_2.get<Transform>().SetLocalRotation(-90.0f, 180.0f, 0.0f);
			island2_2.get<Transform>().SetLocalScale(1.5f, 1.5f, 1.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(island2_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Coin Object

		GameObject Coin_2 = scene0->CreateEntity("Coin");
		{
			Coin_2.emplace<MorphRenderer>();

			std::string coinPrefix = "models/Coin/Coin0";
			std::string coinFileName;

			for (int i = 0; i < 4; i++)
			{
				coinFileName = coinPrefix + std::to_string(i) + ".obj";

				Coin_2.get<MorphRenderer>().addFrame(MorphLoader::LoadFromFile(coinFileName), 0);

			}

			Coin_2.get<MorphRenderer>().SetFrameTime(0.5f, 0);


			Coin_2.get<MorphRenderer>().SetMesh(Coin_2.get<MorphRenderer>().vao).SetMaterial(material4);

			Coin_2.get<Transform>().SetLocalPosition(6.0f, -7.0f, 0.0f);
			Coin_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Coin_2.get<Transform>().SetLocalScale(0.5f, 0.5f, 0.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Coin_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Mound Objects

		GameObject Mound_2 = scene0->CreateEntity("Mound");
		{
			VertexArrayObject::sptr MoundVAO = ObjLoader::LoadFromFile("models/Mound.obj");
			Mound_2.emplace<RendererComponent>().SetMesh(MoundVAO).SetMaterial(material5);
			Mound_2.get<Transform>().SetLocalPosition(-13.0f, 17.0f, -4.0f);
			Mound_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Mound_2.get<Transform>().SetLocalScale(1.5f, 1.5f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Mound_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			GameObject Mound2_2 = scene0->CreateEntity("Mound2");
			Mound2_2.emplace<RendererComponent>().SetMesh(MoundVAO).SetMaterial(material5);
			Mound2_2.get<Transform>().SetLocalPosition(-46.5f, -17.5f, -4.0f);
			Mound2_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Mound2_2.get<Transform>().SetLocalScale(1.5f, 1.5f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Mound2_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			GameObject Mound3_2 = scene0->CreateEntity("Mound3");
			Mound3_2.emplace<RendererComponent>().SetMesh(MoundVAO).SetMaterial(material5);
			Mound3_2.get<Transform>().SetLocalPosition(-17.5f, -9.5f, -4.0f);
			Mound3_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Mound3_2.get<Transform>().SetLocalScale(1.5f, 1.5f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Mound3_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			GameObject Mound4_2 = scene0->CreateEntity("Mound4");
			Mound4_2.emplace<RendererComponent>().SetMesh(MoundVAO).SetMaterial(material5);
			Mound4_2.get<Transform>().SetLocalPosition(-13.0f, -23.5f, -4.0f);
			Mound4_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Mound4_2.get<Transform>().SetLocalScale(2.5f, 1.5f, 2.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Mound4_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Wishing Well Object

		GameObject WishingWell_2 = scene0->CreateEntity("Wishing Well");
		{
			VertexArrayObject::sptr WishingWellVAO = ObjLoader::LoadFromFile("models/Wishing Well.obj");
			WishingWell_2.emplace<RendererComponent>().SetMesh(WishingWellVAO).SetMaterial(material6);
			WishingWell_2.get<Transform>().SetLocalPosition(-13.0f, 17.0f, -0.85f);
			WishingWell_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 125.0f);
			WishingWell_2.get<Transform>().SetLocalScale(0.4f, 0.4f, 0.4f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(WishingWell_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Flower Objects

		GameObject Flower_2 = scene0->CreateEntity("flower");
		{
			VertexArrayObject::sptr FlowerVAO = ObjLoader::LoadFromFile("models/flower.obj");
			Flower_2.emplace<RendererComponent>().SetMesh(FlowerVAO).SetMaterial(material7);
			Flower_2.get<Transform>().SetLocalPosition(-5.8f, -4.2f, -2.3f);
			Flower_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Flower_2.get<Transform>().SetLocalScale(1.6f, 1.6f, 1.6f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Flower_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			GameObject Flower2_2 = scene0->CreateEntity("flower2");

			Flower2_2.emplace<RendererComponent>().SetMesh(FlowerVAO).SetMaterial(material7);
			Flower2_2.get<Transform>().SetLocalPosition(-6.3f, -4.4f, -2.3f);
			Flower2_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Flower2_2.get<Transform>().SetLocalScale(1.6f, 1.6f, 1.6f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Flower2_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			GameObject Flower3_2 = scene0->CreateEntity("flower3");

			Flower3_2.emplace<RendererComponent>().SetMesh(FlowerVAO).SetMaterial(material7);
			Flower3_2.get<Transform>().SetLocalPosition(-5.6f, -3.6f, -2.3f);
			Flower3_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Flower3_2.get<Transform>().SetLocalScale(1.6f, 1.6f, 1.6f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Flower3_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			GameObject Flower4_2 = scene0->CreateEntity("flower4");

			VertexArrayObject::sptr FlowerVAO2 = ObjLoader::LoadFromFile("models/hossain/flower.obj");
			Flower4_2.emplace<RendererComponent>().SetMesh(FlowerVAO2).SetMaterial(material7);
			Flower4_2.get<Transform>().SetLocalPosition(-45.0f, -16.5f, -1.0f);
			Flower4_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, -20.0f);
			Flower4_2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Flower4_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);


		}

		#pragma endregion


		#pragma region Portal Object

		GameObject Portal_2 = scene0->CreateEntity("Portal");
		{
			VertexArrayObject::sptr PortalVAO = ObjLoader::LoadFromFile("models/portal.obj");
			Portal_2.emplace<RendererComponent>().SetMesh(PortalVAO).SetMaterial(material8);
			Portal_2.get<Transform>().SetLocalPosition(-48.0f, -0.0f, 1.0f);
			Portal_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Portal_2.get<Transform>().SetLocalScale(3.0f, 3.0f, 3.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Portal_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Crystal Object

		GameObject Crystal_2 = scene0->CreateEntity("Crystal");
		{
			VertexArrayObject::sptr CrystalVAO = ObjLoader::LoadFromFile("models/crystal.obj");
			Crystal_2.emplace<RendererComponent>().SetMesh(CrystalVAO).SetMaterial(material9);
			Crystal_2.get<Transform>().SetLocalPosition(-46.0f, -0.0f, -0.8f);
			Crystal_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Crystal_2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Crystal_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Bridge Object

		GameObject Bridge_2 = scene0->CreateEntity("Bridge");
		{
			VertexArrayObject::sptr BridgeVAO = ObjLoader::LoadFromFile("models/hossain/NewBridge.obj");
			Bridge_2.emplace<RendererComponent>().SetMesh(BridgeVAO).SetMaterial(material13);
			Bridge_2.get<Transform>().SetLocalPosition(-18.0f, -0.0f, -3.0f);
			Bridge_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Bridge_2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Bridge_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Cactus Object

		GameObject Cacti_2 = scene0->CreateEntity("Cactus");
		{

			VertexArrayObject::sptr CactiVAO = ObjLoader::LoadFromFile("models/hossain/cacti.obj");
			Cacti_2.emplace<RendererComponent>().SetMesh(CactiVAO).SetMaterial(material11);
			Cacti_2.get<Transform>().SetLocalPosition(-46.5f, -17.5f, -1.0f);
			Cacti_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Cacti_2.get<Transform>().SetLocalScale(0.5f, 0.5f, 0.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Cacti_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Bush Object

		GameObject Bush_2 = scene0->CreateEntity("Bush");
		{
			VertexArrayObject::sptr BushVAO = ObjLoader::LoadFromFile("models/Bush.obj");
			Bush_2.emplace<RendererComponent>().SetMesh(BushVAO).SetMaterial(material11);
			Bush_2.get<Transform>().SetLocalPosition(6.0f, 11.0f, -2.7f);
			Bush_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 40.0f);
			Bush_2.get<Transform>().SetLocalScale(2.0f, 2.0f, 2.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Bush_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Sign Object

		GameObject Sign_2 = scene0->CreateEntity("Sign");
		{
			VertexArrayObject::sptr SignVAO = ObjLoader::LoadFromFile("models/Fardeen/Sign.obj");
			Sign_2.emplace<RendererComponent>().SetMesh(SignVAO).SetMaterial(material14);
			Sign_2.get<Transform>().SetLocalPosition(-6.0f, -5.0f, -2.7f);
			Sign_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Sign_2.get<Transform>().SetLocalScale(0.8f, 0.8f, 0.8f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Sign_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Tree Object

		GameObject Tree_2 = scene0->CreateEntity("Tree");
		{
			VertexArrayObject::sptr TreeVAO = ObjLoader::LoadFromFile("models/Fardeen/Tree.obj");
			Tree_2.emplace<RendererComponent>().SetMesh(TreeVAO).SetMaterial(material15);
			Tree_2.get<Transform>().SetLocalPosition(-17.5f, -9.5f, -2.7f);
			Tree_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Tree_2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Tree_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Platform Object

		GameObject Platform_2 = scene0->CreateEntity("Platform");
		{
			VertexArrayObject::sptr PlatformVAO = ObjLoader::LoadFromFile("models/Fardeen/Platform.obj");
			Platform_2.emplace<RendererComponent>().SetMesh(PlatformVAO).SetMaterial(material16);
			Platform_2.get<Transform>().SetLocalPosition(9.5f, 9.5f, -2.7f);
			Platform_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, -60.0f);
			Platform_2.get<Transform>().SetLocalScale(2.0f, 2.0f, 2.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Platform_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Mailbox Object

		GameObject Mailbox_2 = scene0->CreateEntity("mailbox");
		{
			VertexArrayObject::sptr MailboxVAO = ObjLoader::LoadFromFile("models/hossain/mailbox.obj");
			Mailbox_2.emplace<RendererComponent>().SetMesh(MailboxVAO).SetMaterial(material18);
			Mailbox_2.get<Transform>().SetLocalPosition(11.0f, 7.5f, -2.7f);
			Mailbox_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, -60.0f);
			Mailbox_2.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Mailbox_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion

		
		#pragma region House Object

		GameObject House_2 = scene0->CreateEntity("House");
		{
			VertexArrayObject::sptr HouseVAO = ObjLoader::LoadFromFile("models/CompletedHouse.obj");
			House_2.emplace<RendererComponent>().SetMesh(HouseVAO).SetMaterial(material20);
			House_2.get<Transform>().SetLocalPosition(-13.0f, -23.5f, -1.1f);
			House_2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 225.0f);
			House_2.get<Transform>().SetLocalScale(0.45f, 0.8f, 0.8f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(House_2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Playtext Object

		GameObject PlayText = scene0->CreateEntity("PlayText");
		{
			VertexArrayObject::sptr PlayVAO = ObjLoader::LoadFromFile("models/PlayText.obj");
			PlayText.emplace<RendererComponent>().SetMesh(PlayVAO).SetMaterial(material15);
			PlayText.get<Transform>().SetLocalPosition(5.0f, 2.7f, 8.0f);
			PlayText.get<Transform>().SetLocalRotation(-90.0f, -180.0f, 22.5f);
			PlayText.get<Transform>().SetLocalScale(0.5f, 0.5f, 0.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(PlayText);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Sprite Objects

		VertexArrayObject::sptr menuVAO;

		GameObject playMenuObj = scene0->CreateEntity("MenuPlay");
		{
			GLfloat vertices[] = { -1, -1, 0, // bottom left corner
						   -1,  1, 0, // top left corner
							1,  1, 0, // top right corner
							1, -1, 0 }; // bottom right corner


			GLuint elements[] = {
					0, 1, 2,
					2, 3, 0
			};


			menuVAO = NotObjLoader::LoadFromFile("sprite.notobj");
			playMenuObj.emplace<Sprite>().SetMesh(menuVAO).SetMaterial(playMenuMat);
			//playMenuObj.emplace<RendererComponent>().SetMesh(playMenuVao).SetMaterial(playMenuMat);
			playMenuObj.get<Transform>().SetLocalPosition(-5.0f, 0.0f, 10.0f);
			playMenuObj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			//playMenuObj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 180.0f);
			playMenuObj.get<Transform>().SetLocalScale(0.6f, 0.6f, 0.6f);
			//playMenuObj.get<Transform>().LookAt(glm::vec3(0));

		}

		#pragma endregion


		#pragma region Camera Objects

		// Create an object to be our camera
		GameObject cameraObject0 = scene0->CreateEntity("Camera");
		{
			//cameraObject0.get<Transform>().SetLocalPosition(6.0f, 3.0f, 8.0f).LookAt(glm::vec3(-5.0f, 0.0f, 8.0f));
			cameraObject0.get<Transform>().SetLocalPosition(10.0f, 0.0f, 10.0f).LookAt(playMenuObj.get<Transform>().GetLocalPosition());

			// We'll make our camera a component of the camera object
			Camera& camera = cameraObject0.emplace<Camera>();// Camera::Create();
			camera.SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));
			camera.SetUp(glm::vec3(0, 0, 1));
			camera.LookAt(glm::vec3(-5.0f, 0.0f, 8.0f));
			camera.SetFovDegrees(90.0f); // Set an initial FOV
			camera.SetOrthoHeight(3.0f);
		}

		// Create an object to be our orthographic camera
		GameObject orthoCameraObject0 = scene0->CreateEntity("OrthoCamera");
		{
			//cameraObject.get<Transform>().SetLocalPosition(0, 3, 3).LookAt(glm::vec3(0, 0, 0));
			//Looking at water and terrain positions
			//cameraObject.get<Transform>().SetLocalPosition(-43.89, 25.74, 3.89).LookAt(glm::vec3(-40.69, -0.53, -7.83));
			orthoCameraObject0.get<Transform>().SetLocalPosition(10.0f, 0.0f, 10.0f).LookAt(playMenuObj.get<Transform>().GetLocalPosition());

			// We'll make our camera a component of the camera object
			Camera& orthoCamera = orthoCameraObject0.emplace<Camera>();// Camera::Create();
			orthoCamera.SetPosition(glm::vec3(0, 3, 3));
			orthoCamera.SetUp(glm::vec3(0, 0, 1));
			orthoCamera.LookAt(glm::vec3(0));
			orthoCamera.SetFovDegrees(90.0f); // Set an initial FOV
			orthoCamera.SetOrthoHeight(3.0f);
			orthoCameraObject0.get<Camera>().ToggleOrtho();
			//BehaviourBinding::Bind<CameraControlBehaviour>(orthoCameraObject);
		}

		#pragma endregion


		#pragma region Post Effects

		glfwGetWindowSize(BackendHandler::window, &width, &height);

		GameObject framebufferObject0 = scene0->CreateEntity("Basic Effect");
		{
			basicEffect0 = &framebufferObject0.emplace<PostEffect>();
			basicEffect0->Init(width, height);
		}

		#pragma endregion

		#pragma endregion
		///////////////////////////////////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////// Death Scene /////////////////////////////////////////////////////////
		#pragma region Death Scene

		// Create a scene, and set it to be the active scene in the application
		GameScene::sptr scene3 = GameScene::Create("Death Scene");

		// We can create a group ahead of time to make iterating on the group faster
		entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<Transform>, RendererComponent> renderGroup3 =
			scene3->Registry().group<RendererComponent>(entt::get_t<Transform>());

		#pragma region Materials

		ShaderMaterial::sptr deathMenuMat = ShaderMaterial::Create();
		deathMenuMat->Shader = spriteShader;
		deathMenuMat->Set("s_Diffuse", deathMenuDiffuse);
		deathMenuMat->RenderLayer = 0;

		#pragma endregion


		#pragma region Game Over ObjectS

		GameObject GameOver = scene3->CreateEntity("Game Over");
		{

			VertexArrayObject::sptr GameOverVAO = ObjLoader::LoadFromFile("models/YouDied.obj");
			GameOver.emplace<RendererComponent>().SetMesh(GameOverVAO).SetMaterial(material12);
			GameOver.get<Transform>().SetLocalPosition(-5.0f, 0.0f, 0.0f);
			GameOver.get<Transform>().SetLocalRotation(90.0f, 0.0f, -90.0f);
			GameOver.get<Transform>().SetLocalScale(3.0f, 3.0f, 3.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(GameOver);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}

		#pragma endregion


		#pragma region Sprite Objects

		VertexArrayObject::sptr deathVAO;

		GameObject deathMenuObj = scene3->CreateEntity("MenuPlay");
		{
			GLfloat vertices[] = { -1, -1, 0, // bottom left corner
						   -1,  1, 0, // top left corner
							1,  1, 0, // top right corner
							1, -1, 0 }; // bottom right corner


			GLuint elements[] = {
					0, 1, 2,
					2, 3, 0
			};


			menuVAO = NotObjLoader::LoadFromFile("sprite.notobj");
			deathMenuObj.emplace<Sprite>().SetMesh(menuVAO).SetMaterial(deathMenuMat);
			//daethMenuObj.emplace<RendererComponent>().SetMesh(daethMenuVao).SetMaterial(daethMenuMat);
			deathMenuObj.get<Transform>().SetLocalPosition(-5.0f, 0.0f, 10.0f);
			deathMenuObj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 270.0f);
			//daethMenuObj.get<Transform>().SetLocalRotation(0.0f, 180.0f, 180.0f);
			deathMenuObj.get<Transform>().SetLocalScale(0.6f, 0.6f, 0.6f);
			//daethMenuObj.get<Transform>().LookAt(glm::vec3(0));

		}

		#pragma endregion


		#pragma region Camera Objects

		// Create an object to be our camera
		GameObject cameraObject3 = scene3->CreateEntity("Camera");
		{
			cameraObject3.get<Transform>().SetLocalPosition(10.0f, 0.0f, 10.0f).LookAt(playMenuObj.get<Transform>().GetLocalPosition());

			// We'll make our camera a component of the camera object
			Camera& camera = cameraObject3.emplace<Camera>();// Camera::Create();
			camera.SetPosition(glm::vec3(-10, 0, 10));
			camera.SetUp(glm::vec3(0, 0, 1));
			camera.LookAt(glm::vec3(-5.0f, 0.0f, 0.0f));
			camera.SetFovDegrees(90.0f); // Set an initial FOV
			camera.SetOrthoHeight(3.0f);
		}

		// Create an object to be our orthographic camera
		GameObject orthoCameraObject3 = scene3->CreateEntity("OrthoCamera");
		{
			//cameraObject.get<Transform>().SetLocalPosition(0, 3, 3).LookAt(glm::vec3(0, 0, 0));
			//Looking at water and terrain positions
			//cameraObject.get<Transform>().SetLocalPosition(-43.89, 25.74, 3.89).LookAt(glm::vec3(-40.69, -0.53, -7.83));
			orthoCameraObject3.get<Transform>().SetLocalPosition(10.0f, 0.0f, 10.0f).LookAt(deathMenuObj.get<Transform>().GetLocalPosition());

			// We'll make our camera a component of the camera object
			Camera& orthoCamera = orthoCameraObject3.emplace<Camera>();// Camera::Create();
			orthoCamera.SetPosition(glm::vec3(0, 3, 3));
			orthoCamera.SetUp(glm::vec3(0, 0, 1));
			orthoCamera.LookAt(glm::vec3(0));
			orthoCamera.SetFovDegrees(90.0f); // Set an initial FOV
			orthoCamera.SetOrthoHeight(3.0f);
			orthoCameraObject3.get<Camera>().ToggleOrtho();
			//BehaviourBinding::Bind<CameraControlBehaviour>(orthoCameraObject);
		}


		#pragma endregion

		#pragma endregion
		///////////////////////////////////////////////////////////////////////////////////////////////////////////

		
		/////////////////////////////////// SKYBOX ///////////////////////////////////////////////
		{
			// Load our shaders
			Shader::sptr skybox = std::make_shared<Shader>();
			skybox->LoadShaderPartFromFile("shaders/skybox-shader.vert.glsl", GL_VERTEX_SHADER);
			skybox->LoadShaderPartFromFile("shaders/skybox-shader.frag.glsl", GL_FRAGMENT_SHADER);
			skybox->Link();
			ShaderMaterial::sptr skyboxMat = ShaderMaterial::Create();
			skyboxMat->Shader = skybox;  
			skyboxMat->Set("s_Environment", environmentMap);
			skyboxMat->Set("u_EnvironmentRotation", glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0))));
			skyboxMat->RenderLayer = 100;

			MeshBuilder<VertexPosNormTexCol> mesh;
			MeshFactory::AddIcoSphere(mesh, glm::vec3(0.0f), 1.0f);
			MeshFactory::InvertFaces(mesh);
			VertexArrayObject::sptr meshVao = mesh.Bake();
			
			GameObject skyboxObj0 = scene0->CreateEntity("skybox0");  
			skyboxObj0.get<Transform>().SetLocalPosition(0.0f, 0.0f, 0.0f);
			skyboxObj0.get_or_emplace<RendererComponent>().SetMesh(meshVao).SetMaterial(skyboxMat).SetCastShadow(false);;

			GameObject skyboxObj = scene->CreateEntity("skybox");
			skyboxObj.get<Transform>().SetLocalPosition(0.0f, 0.0f, 0.0f);
			skyboxObj.get_or_emplace<RendererComponent>().SetMesh(meshVao).SetMaterial(skyboxMat).SetCastShadow(false);;

			GameObject skyboxObj2 = scene2->CreateEntity("skybox2");
			skyboxObj2.get<Transform>().SetLocalPosition(0.0f, 0.0f, 0.0f);
			skyboxObj2.get_or_emplace<RendererComponent>().SetMesh(meshVao).SetMaterial(skyboxMat).SetCastShadow(false);;

			GameObject skyboxObj4 = scene4->CreateEntity("skybox4");
			skyboxObj4.get<Transform>().SetLocalPosition(0.0f, 0.0f, 0.0f);
			skyboxObj4.get_or_emplace<RendererComponent>().SetMesh(meshVao).SetMaterial(skyboxMat).SetCastShadow(false);;
		}
		////////////////////////////////////////////////////////////////////////////////////////
		

		// We'll use a vector to store all our key press events for now (this should probably be a behaviour eventually)
		std::vector<KeyPressWatcher> keyToggles;
		{
			// This is an example of a key press handling helper. Look at InputHelpers.h an .cpp to see
			// how this is implemented. Note that the ampersand here is capturing the variables within
			// the scope. If you wanted to do some method on the class, your best bet would be to give it a method and
			// use std::bind
			keyToggles.emplace_back(GLFW_KEY_T, [&]() { cameraObject.get<Camera>().ToggleOrtho(); });
			bool isLooping = true;
			keyToggles.emplace_back(GLFW_KEY_U, [&]() { 
				isLooping = !isLooping;
				Wizard.get<MorphRenderer>().SetLooping(isLooping, 0);
				});

			keyToggles.emplace_back(GLFW_KEY_KP_2, [&]() 
			{
				if (!startPanCamera)
				{
					posTimer = 0.0f;
					startPanCamera = true;
					playerControlLock = true;
					std::cout << "\nPanned Camera!" << std::endl;
				}
				else
				{
					startPanCamera = false;
					playerControlLock = false;

				}
			});

			keyToggles.emplace_back(GLFW_KEY_B, [&]() 
			{ 
				glfwGetWindowSize(BackendHandler::window, &width, &height);

				std::cout << "\nCurrent Window Width: " << width << std::endl;
				std::cout << "Current Window Height: " << height << std::endl;

				/*if (Application::Instance().ActiveScene == scene)
				{
					std::cout << "\nIt is Scene 1" << std::endl;
				}
				else
				{
					std::cout << "\nIt is not Scene 1" << std::endl;
				}*/
			});

			controllables.push_back(player);
			controllables.push_back(Wizard);

			keyToggles.emplace_back(GLFW_KEY_KP_ADD, [&]() {
				BehaviourBinding::Get<SimpleMoveBehaviour>(controllables[selectedVao])->Enabled = false;
				selectedVao++;
				if (selectedVao >= controllables.size())
					selectedVao = 0;
				BehaviourBinding::Get<SimpleMoveBehaviour>(controllables[selectedVao])->Enabled = true;
				});
			keyToggles.emplace_back(GLFW_KEY_KP_SUBTRACT, [&]() {
				BehaviourBinding::Get<SimpleMoveBehaviour>(controllables[selectedVao])->Enabled = false;
				selectedVao--;
				if (selectedVao < 0)
					selectedVao = controllables.size() - 1;
				BehaviourBinding::Get<SimpleMoveBehaviour>(controllables[selectedVao])->Enabled = true;
				});

			keyToggles.emplace_back(GLFW_KEY_Y, [&]() {
				auto behaviour = BehaviourBinding::Get<SimpleMoveBehaviour>(controllables[selectedVao]);
				behaviour->Relative = !behaviour->Relative;
				});

			keyToggles.emplace_back(GLFW_KEY_C, [&]() {

				if (!playerControlLock)
				{
					BehaviourBinding::Bind<CameraControlBehaviour>(cameraObject);
					playerControlLock = true;
				}
				else
				{
					BehaviourBinding::BindDisabled<CameraControlBehaviour>(cameraObject);
					playerControlLock = false;
				}

			});

			keyToggles.emplace_back(GLFW_KEY_SPACE, [&]() {
				if (!playerAirborne && !playerControlLock)
				{
					//playerBody->applyForce(btVector3(0.0f, 0.0f, 3000.0f), btVector3(0.0f, 0.0f, 0.0f));
					playerJump = true;
				}
			});

			keyToggles.emplace_back(GLFW_KEY_J, [&]() {
				wizardTransform.setOrigin(btVector3(-14.5f, 0.0f, 15.5f));
				wizardBody->setWorldTransform(wizardTransform);
				});

			keyToggles.emplace_back(GLFW_KEY_P, [&]() {
				if (!drawPhysics)
					drawPhysics = true;
				else
					drawPhysics = false;
			});

			keyToggles.emplace_back(GLFW_KEY_L, [&]() {
				RenderGroupBool = 2;
				Application::Instance().ActiveScene = scene2;
			});

			#pragma region Main Menu Keys

			keyToggles.emplace_back(GLFW_KEY_LEFT, [&]() {
				if (RenderGroupBool == 0)
				{
					menuSelect--;

					if (menuSelect < 0)
						menuSelect = 2;

					if (menuSelect == 0)
					{
						playMenuObj.get<Sprite>().SetMaterial(controlsMenuMat);
					}
					else if (menuSelect == 1)
					{
						playMenuObj.get<Sprite>().SetMaterial(playMenuMat);
					}
					else
					{
						playMenuObj.get<Sprite>().SetMaterial(exitMenuMat);
					}
				}
			});

			keyToggles.emplace_back(GLFW_KEY_RIGHT, [&]() {
				if (RenderGroupBool == 0)
				{
					menuSelect++;

					if (menuSelect > 2)
						menuSelect = 0;

					if (menuSelect == 0)
					{
						playMenuObj.get<Sprite>().SetMaterial(controlsMenuMat);
					}
					else if (menuSelect == 1)
					{
						playMenuObj.get<Sprite>().SetMaterial(playMenuMat);
					}
					else
					{
						playMenuObj.get<Sprite>().SetMaterial(exitMenuMat);
					}
				}
			});
			

			keyToggles.emplace_back(GLFW_KEY_ENTER, [&]() {
				if (menuSelect == 0)
				{
					menuSelect = -1;
					playMenuObj.get<Sprite>().SetMaterial(controlsMat);
				}
				else if (menuSelect == 1)
				{
					menuSelect = 3;
					//menu.StopImmediately();
					//Prelude1.Play();
					//RenderGroupBool = 1;
					//Application::Instance().ActiveScene = scene;
				}
				else if (menuSelect == 2)
				{
					exit(0);
				}
				else if (menuSelect == 3)
				{
					menuSelect = 4;
					//Prelude1.StopImmediately();
					//Prelude2.Play();
				}
				else if (menuSelect == 4)
				{
					menuSelect = 5;
					//Prelude2.StopImmediately();
					//Prelude3.Play();
				}
				else if (menuSelect == 5)
				{
					menuSelect = 6;
					//Prelude3.StopImmediately();
				}
				
			});

			keyToggles.emplace_back(GLFW_KEY_BACKSPACE, [&]() {
				if (menuSelect == -1)
				{
					menuSelect = 0;
					playMenuObj.get<Sprite>().SetMaterial(controlsMenuMat);
				}

			});

			#pragma endregion

			int sortCounter = 0;
			keyToggles.emplace_back(GLFW_KEY_G, [&]() {
				if (RenderGroupBool == 2 && !sortingItemHeld)
				{
					if (sortCounter == 0 && blueDonutObj.get<Transform>().GetLocalPosition().y < carrotObj.get<Transform>().GetLocalPosition().y &&
						appleObj.get<Transform>().GetLocalPosition().y < purpleGrapeObj.get<Transform>().GetLocalPosition().y &&
						bananasObj.get<Transform>().GetLocalPosition().y < greenGrapeObj.get<Transform>().GetLocalPosition().y)
					{
						sortByQuantity = true;
						sortCounter++;
						coinDescend2 = true;
					}
					else if (sortCounter == 1 && appleObj.get<Transform>().GetLocalPosition().y < carrotObj.get<Transform>().GetLocalPosition().y &&
						bananasObj.get<Transform>().GetLocalPosition().y < greenGrapeObj.get<Transform>().GetLocalPosition().y &&
						bananasObj.get<Transform>().GetLocalPosition().y < purpleGrapeObj.get<Transform>().GetLocalPosition().y)
					{
						sortByLetter = true;
						sortCounter++;
					}
					else if (sortCounter == 2 && appleObj.get<Transform>().GetLocalPosition().y < carrotObj.get<Transform>().GetLocalPosition().y &&
						bananasObj.get<Transform>().GetLocalPosition().y < greenGrapeObj.get<Transform>().GetLocalPosition().y &&
						bananasObj.get<Transform>().GetLocalPosition().y < purpleGrapeObj.get<Transform>().GetLocalPosition().y)
					{
						sortByLetter = true;
						sortCounter++;
						coinDescend2 = true;
					}
					else
					{
						PlayerHealth--;
					}

				}

				
			});

			#pragma region Interaction Keys

			keyToggles.emplace_back(GLFW_KEY_E, [&]() 
			{	
				if (RenderGroupBool == 1)
				{
					if (interactDistance <= 8.0f && !playerAirborne && Wizard.get<Transform>().GetLocalPosition().x > -30.0f)
					{
						if (!startPanCamera)
						{
							posTimer = 0.0f;
							startPanCamera = true;
							playerControlLock = true;
							std::cout << "\nPanned Camera!" << std::endl;

							if (RenderGroupBool == 1 && CoinCount > 0)
							{
								dialogueL1D1.get<Sprite>().SetMaterial(dialogueMatL1D11);
							}
						}
						else
						{
							startPanCamera = false;
							playerControlLock = false;

						}
					}
				}
				else if (RenderGroupBool == 2)
				{
					if ((interactDistance <= 8.0f || interactSketchyGuy1Distance <= 8.0f || interactSketchyGuy2Distance <= 8.0f) && !playerAirborne)
					{
						if (!startPanCamera)
						{
							posTimer = 0.0f;
							startPanCamera = true;
							playerControlLock = true;
							std::cout << "\nPanned Camera!" << std::endl;

							if (RenderGroupBool == 2 && interactDistance <= 8.0f)
							{
								dialogueL2.get<Sprite>().SetMaterial(dialogueMatL2D1);
							}

							if (RenderGroupBool == 2 && interactSketchyGuy1Distance <= 8.0f)
							{
								dialogueL2.get<Sprite>().SetMaterial(sketchyGuy1Mat1);
							}

							if (RenderGroupBool == 2 && interactSketchyGuy2Distance <= 8.0f)
							{
								dialogueL2.get<Sprite>().SetMaterial(sketchyGuy2Mat1);
							}

						}
						else
						{
							startPanCamera = false;
							playerControlLock = false;

						}
					}

					if (interactButtonDistance <= 5.0f && !playerAirborne)
					{
						if (!startPanCamera)
						{
							posTimer = 0.0f;
							startPanCamera = true;
							playerControlLock = true;
							std::cout << "\nPanned Camera!" << std::endl;

							puzzleSelect = true;

							puzzleTracker = 0;

							/*if (RenderGroupBool == 2 && interactDistance <= 8.0f)
							{
								dialogueL2.get<Sprite>().SetMaterial(dialogueMatL2D1);
							}

							if (RenderGroupBool == 2 && interactSketchyGuy1Distance <= 8.0f)
							{
								dialogueL2.get<Sprite>().SetMaterial(sketchyGuy1Mat1);
							}*/

						}
						else
						{
							startPanCamera = false;
							playerControlLock = false;
							puzzleSelect = false;

						}
					}

					#pragma region Snowmen Mat Switching

					if (RenderGroupBool == 2 && redSnowmanDistance <= 3.0f)
					{

						if (!startPanCamera)
						{
							posTimer = 0.0f;
							startPanCamera = true;
							playerControlLock = true;
							std::cout << "\nPanned Camera!" << std::endl;
							dialogueL2.get<Sprite>().SetMaterial(redSnowmanTextMat);
						}
						else
						{
							startPanCamera = false;
							playerControlLock = false;
						}
					}
					else if (RenderGroupBool == 2 && greenSnowmanDistance <= 3.0f)
					{
						if (!startPanCamera)
						{
							posTimer = 0.0f;
							startPanCamera = true;
							playerControlLock = true;
							std::cout << "\nPanned Camera!" << std::endl;
							dialogueL2.get<Sprite>().SetMaterial(greenSnowmanTextMat);
						}
						else
						{
							startPanCamera = false;
							playerControlLock = false;
						}
					}
					else if (RenderGroupBool == 2 && blueSnowmanDistance <= 3.0f)
					{
						if (!startPanCamera)
						{
							posTimer = 0.0f;
							startPanCamera = true;
							playerControlLock = true;
							std::cout << "\nPanned Camera!" << std::endl;
							dialogueL2.get<Sprite>().SetMaterial(blueSnowmanTextMat);
						}
						else
						{
							startPanCamera = false;
							playerControlLock = false;
						}
					}
					else if (RenderGroupBool == 2 && brownSnowmanDistance <= 3.0f)
					{
						if (!startPanCamera)
						{
							posTimer = 0.0f;
							startPanCamera = true;
							playerControlLock = true;
							std::cout << "\nPanned Camera!" << std::endl;
							dialogueL2.get<Sprite>().SetMaterial(brownSnowmanTextMat);
						}
						else
						{
							startPanCamera = false;
							playerControlLock = false;
						}
					}
					else if (RenderGroupBool == 2 && purpleSnowmanDistance <= 3.0f)
					{
						if (!startPanCamera)
						{
							posTimer = 0.0f;
							startPanCamera = true;
							playerControlLock = true;
							std::cout << "\nPanned Camera!" << std::endl;
							dialogueL2.get<Sprite>().SetMaterial(purpleSnowmanTextMat);
						}
						else
						{
							startPanCamera = false;
							playerControlLock = false;
						}
					}
					else if (RenderGroupBool == 2 && orangeSnowmanDistance <= 3.0f)
					{
						if (!startPanCamera)
						{
							posTimer = 0.0f;
							startPanCamera = true;
							playerControlLock = true;
							std::cout << "\nPanned Camera!" << std::endl;
							dialogueL2.get<Sprite>().SetMaterial(orangeSnowmanTextMat);
						}
						else
						{
							startPanCamera = false;
							playerControlLock = false;
						}
					}
					else if (RenderGroupBool == 2 && yellowSnowmanDistance <= 3.0f)
					{
						if (!startPanCamera)
						{
							posTimer = 0.0f;
							startPanCamera = true;
							playerControlLock = true;
							std::cout << "\nPanned Camera!" << std::endl;
							dialogueL2.get<Sprite>().SetMaterial(yellowSnowmanTextMat);
						}
						else
						{
							startPanCamera = false;
							playerControlLock = false;
						}
					}
					else if (RenderGroupBool == 2 && aquaSnowmanDistance <= 3.0f)
					{
						if (!startPanCamera)
						{
							posTimer = 0.0f;
							startPanCamera = true;
							playerControlLock = true;
							std::cout << "\nPanned Camera!" << std::endl;
							dialogueL2.get<Sprite>().SetMaterial(aquaSnowmanTextMat);
						}
						else
						{
							startPanCamera = false;
							playerControlLock = false;
						}
					}
					else if (RenderGroupBool == 2 && pinkSnowmanDistance <= 3.0f)
					{
						if (!startPanCamera)
						{
							posTimer = 0.0f;
							startPanCamera = true;
							playerControlLock = true;
							std::cout << "\nPanned Camera!" << std::endl;
							dialogueL2.get<Sprite>().SetMaterial(pinkSnowmanTextMat);
						}
						else
						{
							startPanCamera = false;
							playerControlLock = false;
						}
					}

					#pragma endregion


					#pragma region Sorting Objects Switching

					if (RenderGroupBool == 2 && appleDistance <= 2.0f)
					{
						if (!sortingItemHeld)
						{
							sortingItemHeld = true;
							appleHeld = true;
							sortingItemHeldPos = appleObj.get<Transform>().GetLocalPosition();
						}
						else
						{
							sortingItemHeld = false;
							if (bananaHeld)
							{
								swapPositions(bananasObj, appleObj);
								clearSortingItems();
							}
							else if (greenGrapeHeld)
							{
								swapPositions(greenGrapeObj, appleObj);
								clearSortingItems();
							}
							else if (purpleGrapeHeld)
							{
								swapPositions(purpleGrapeObj, appleObj);
								clearSortingItems();
							}
							else if (carrotHeld)
							{
								swapPositions(carrotObj, appleObj);
								clearSortingItems();
							}
							else if (blueDonutHeld)
							{
								swapPositions(blueDonutObj, appleObj);
								clearSortingItems();
							}
						}
					}
					else if (RenderGroupBool == 2 && bananaDistance <= 2.0f)
					{
						if (!sortingItemHeld)
						{
							sortingItemHeld = true;
							bananaHeld = true;
							sortingItemHeldPos = bananasObj.get<Transform>().GetLocalPosition();
						}
						else
						{
							sortingItemHeld = false;
							if (appleHeld)
							{
								swapPositions(appleObj, bananasObj);
								clearSortingItems();
							}
							else if (greenGrapeHeld)
							{
								swapPositions(greenGrapeObj, bananasObj);
								clearSortingItems();
							}
							else if (purpleGrapeHeld)
							{
								swapPositions(purpleGrapeObj, bananasObj);
								clearSortingItems();
							}
							else if (carrotHeld)
							{
								swapPositions(carrotObj, bananasObj);
								clearSortingItems();
							}
							else if (blueDonutHeld)
							{
								swapPositions(blueDonutObj, bananasObj);
								clearSortingItems();
							}
						}
					}
					else if (RenderGroupBool == 2 && greenGrapeDistance <= 2.0f)
					{
						if (!sortingItemHeld)
						{
							sortingItemHeld = true;
							greenGrapeHeld = true;
							sortingItemHeldPos = greenGrapeObj.get<Transform>().GetLocalPosition();
						}
						else
						{
							sortingItemHeld = false;
							if (appleHeld)
							{
								swapPositions(appleObj, greenGrapeObj);
								clearSortingItems();
							}
							else if (bananaHeld)
							{
								swapPositions(bananasObj, greenGrapeObj);
								clearSortingItems();
							}
							else if (purpleGrapeHeld)
							{
								swapPositions(purpleGrapeObj, greenGrapeObj);
								clearSortingItems();
							}
							else if (carrotHeld)
							{
								swapPositions(carrotObj, greenGrapeObj);
								clearSortingItems();
							}
							else if (blueDonutHeld)
							{
								swapPositions(blueDonutObj, greenGrapeObj);
								clearSortingItems();
							}
						}
					}
					else if (RenderGroupBool == 2 && purpleGrapeDistance <= 2.0f)
					{
						if (!sortingItemHeld)
						{
							sortingItemHeld = true;
							purpleGrapeHeld = true;
							sortingItemHeldPos = purpleGrapeObj.get<Transform>().GetLocalPosition();
						}
						else
						{
							sortingItemHeld = false;
							if (appleHeld)
							{
								swapPositions(appleObj, purpleGrapeObj);
								clearSortingItems();
							}
							else if (bananaHeld)
							{
								swapPositions(bananasObj, purpleGrapeObj);
								clearSortingItems();
							}
							else if (greenGrapeHeld)
							{
								swapPositions(greenGrapeObj, purpleGrapeObj);
								clearSortingItems();
							}
							else if (carrotHeld)
							{
								swapPositions(carrotObj, purpleGrapeObj);
								clearSortingItems();
							}
							else if (blueDonutHeld)
							{
								swapPositions(blueDonutObj, purpleGrapeObj);
								clearSortingItems();
							}
						}
					}
					else if (RenderGroupBool == 2 && carrotDistance <= 2.0f)
					{
						if (!sortingItemHeld)
						{
							sortingItemHeld = true;
							carrotHeld = true;
							sortingItemHeldPos = carrotObj.get<Transform>().GetLocalPosition();
						}
						else
						{
							sortingItemHeld = false;
							if (appleHeld)
							{
								swapPositions(appleObj, carrotObj);
								clearSortingItems();
							}
							else if (bananaHeld)
							{
								swapPositions(bananasObj, carrotObj);
								clearSortingItems();
							}
							else if (greenGrapeHeld)
							{
								swapPositions(greenGrapeObj, carrotObj);
								clearSortingItems();
							}
							else if (purpleGrapeHeld)
							{
								swapPositions(purpleGrapeObj, carrotObj);
								clearSortingItems();
							}
							else if (blueDonutHeld)
							{
								swapPositions(blueDonutObj, carrotObj);
								clearSortingItems();
							}
						}
					}
					else if (RenderGroupBool == 2 && blueDonutDistance <= 2.0f)
					{
						if (!sortingItemHeld)
						{
							sortingItemHeld = true;
							blueDonutHeld = true;
							sortingItemHeldPos = blueDonutObj.get<Transform>().GetLocalPosition();
						}
						else
						{
							sortingItemHeld = false;
							if (appleHeld)
							{
								swapPositions(appleObj, blueDonutObj);
								clearSortingItems();
							}
							else if (bananaHeld)
							{
								swapPositions(bananasObj, blueDonutObj);
								clearSortingItems();
							}
							else if (greenGrapeHeld)
							{
								swapPositions(greenGrapeObj, blueDonutObj);
								clearSortingItems();
							}
							else if (purpleGrapeHeld)
							{
								swapPositions(purpleGrapeObj, blueDonutObj);
								clearSortingItems();
							}
							else if (carrotHeld)
							{
								swapPositions(carrotObj, blueDonutObj);
								clearSortingItems();
							}
						}
					}

					#pragma endregion


					#pragma Sorting Puzzle

					if (startSortingPuzzle && interactSketchyGuy2Distance <= 8.0f)
					{
						panToObjects = true;

					}

					#pragma endregion

				}
				
			});

			keyToggles.emplace_back(GLFW_KEY_A, [&]() {
				if (puzzleSelect)
				{
					puzzleTracker--;

					if (puzzleTracker < 0)
						puzzleTracker = 8;
				}
			});

			keyToggles.emplace_back(GLFW_KEY_D, [&]() {
				if (puzzleSelect)
				{
					puzzleTracker++;

					if (puzzleTracker > 8)
						puzzleTracker = 0;
				}
			});

			keyToggles.emplace_back(GLFW_KEY_ENTER, [&]() {
				if (puzzleSelect && puzzleTracker == 8)
				{
					std::cout << "You Got a Coin!!" << std::endl;
					startPanCamera = false;
					playerControlLock = false;
					puzzleSelect = false;
					coinDescend1 = true;
				}
				else if(puzzleSelect && puzzleTracker != 8)
				{
					PlayerHealth--;
					startPanCamera = false;
					playerControlLock = false;
					puzzleSelect = false;
				}
			});

			#pragma endregion


			#pragma region Dialogue Keys

			keyToggles.emplace_back(GLFW_KEY_SPACE, [&]() {
				if (RenderGroupBool == 1 && startPanCamera && CoinCount == 0)
				{
					if (dialogueSelect == 0)
					{
						dialogueL1D1.get<Sprite>().SetMaterial(dialogueMatL1D2);
						dialogueSelect = 1;
					}
					else if (dialogueSelect == 1)
					{
						dialogueL1D1.get<Sprite>().SetMaterial(dialogueMatL1D3);
						dialogueSelect = 2;
					}
					else if (dialogueSelect == 2)
					{
						dialogueL1D1.get<Sprite>().SetMaterial(dialogueMatL1D4);
						dialogueSelect = 3;
					}
					else if (dialogueSelect == 3)
					{
						dialogueL1D1.get<Sprite>().SetMaterial(dialogueMatL1D5);
						dialogueSelect = 4;
					}
					else if (dialogueSelect == 4)
					{
						dialogueL1D1.get<Sprite>().SetMaterial(dialogueMatL1D6);
						dialogueSelect = 5;
					}
					else if (dialogueSelect == 5)
					{
						dialogueL1D1.get<Sprite>().SetMaterial(dialogueMatL1D7);
						dialogueSelect = 6;
					}
					else if (dialogueSelect == 6)
					{
						dialogueL1D1.get<Sprite>().SetMaterial(dialogueMatL1D8);
						dialogueSelect = 7;
					}
					else if (dialogueSelect == 7)
					{
						dialogueL1D1.get<Sprite>().SetMaterial(dialogueMatL1D9);
						dialogueSelect = 8;
					}
					else if (dialogueSelect == 8)
					{
						dialogueL1D1.get<Sprite>().SetMaterial(dialogueMatL1D10);
						dialogueSelect = 9;
					}
					else if (dialogueSelect == 9)
					{
						dialogueL1D1.get<Sprite>().SetMaterial(dialogueMatL1D1);
						dialogueSelect = 0;
						startPanCamera = false;
						playerControlLock = false;
					}
					
				}
				else if(RenderGroupBool == 1 && startPanCamera && CoinCount > 0)
				{
					showInteraction = false;
					startPanCamera = false;
					playerControlLock = false;
					moveWizard1 = true;

					
				}

				if (RenderGroupBool == 2 && startPanCamera && interactDistance <= 8.0f)
				{
					if (dialogueSelect == 0)
					{
						dialogueL2.get<Sprite>().SetMaterial(dialogueMatL2D2);
						dialogueSelect = 1;
					}
					else if (dialogueSelect == 1)
					{
						dialogueL2.get<Sprite>().SetMaterial(dialogueMatL2D3);
						dialogueSelect = 2;
					}
					else if (dialogueSelect == 2)
					{
						dialogueL2.get<Sprite>().SetMaterial(dialogueMatL2D4);
						dialogueSelect = 3;
					}
					else if (dialogueSelect == 3)
					{
						dialogueL2.get<Sprite>().SetMaterial(dialogueMatL2D1);
						dialogueSelect = 0;
						startPanCamera = false;
						playerControlLock = false;
					}

				}

				if (RenderGroupBool == 2 && startPanCamera && interactSketchyGuy1Distance <= 8.0f)
				{
					if (dialogueSelect == 0)
					{
						dialogueL2.get<Sprite>().SetMaterial(sketchyGuy1Mat2);
						dialogueSelect = 1;
					}
					else if (dialogueSelect == 1)
					{
						dialogueL2.get<Sprite>().SetMaterial(sketchyGuy1Mat3);
						dialogueSelect = 2;
					}
					else if (dialogueSelect == 2)
					{
						dialogueL2.get<Sprite>().SetMaterial(sketchyGuy1Mat4);
						dialogueSelect = 3;
					}
					else if (dialogueSelect == 3)
					{
						dialogueL2.get<Sprite>().SetMaterial(sketchyGuy1Mat5);
						dialogueSelect = 4;
					}
					else if (dialogueSelect == 4)
					{
						dialogueL2.get<Sprite>().SetMaterial(sketchyGuy1Mat1);
						dialogueSelect = 0;
						startPanCamera = false;
						playerControlLock = false;
					}

				}

				if (RenderGroupBool == 2 && startPanCamera && interactSketchyGuy2Distance <= 8.0f)
				{
					if (!startAftermath)
					{
						if (dialogueSelect == 0)
						{
							dialogueL2.get<Sprite>().SetMaterial(sketchyGuy2Mat2);
							dialogueSelect = 1;
						}
						else if (dialogueSelect == 1)
						{
							dialogueL2.get<Sprite>().SetMaterial(sketchyGuy2Mat3);
							dialogueSelect = 2;
						}
						else if (dialogueSelect == 2)
						{
							dialogueL2.get<Sprite>().SetMaterial(sketchyGuy2Mat4);
							dialogueSelect = 3;
						}
						else if (dialogueSelect == 3)
						{
							dialogueL2.get<Sprite>().SetMaterial(sketchyGuy2Mat5);
							dialogueSelect = 4;
						}
						else if (dialogueSelect == 4)
						{
							dialogueL2.get<Sprite>().SetMaterial(sketchyGuy2Mat6);
							dialogueSelect = 5;
						}
						else if (dialogueSelect == 5)
						{
							dialogueL2.get<Sprite>().SetMaterial(quizMat1);
							startQuiz = true;
							dialogueSelect = 0;
						}
					}
					else
					{
						if (dialogueSelect == 0)
						{
							dialogueL2.get<Sprite>().SetMaterial(afterMathMat2);
							dialogueSelect = 1;
						}
						else if (dialogueSelect == 1)
						{
							dialogueL2.get<Sprite>().SetMaterial(afterMathMat3);
							dialogueSelect = 2;
						}
						else if (dialogueSelect == 2)
						{
							dialogueL2.get<Sprite>().SetMaterial(afterMathMat4);
							dialogueSelect = 3;
						}
						else if (dialogueSelect == 3)
						{
							dialogueL2.get<Sprite>().SetMaterial(afterMathMat5);
							dialogueSelect = 4;
						}
						else if (dialogueSelect == 4)
						{
							dialogueL2.get<Sprite>().SetMaterial(afterMathMat6);
							dialogueSelect = 5;
						}
						else if (dialogueSelect == 5)
						{
							dialogueL2.get<Sprite>().SetMaterial(afterMathMat1);
							startSortingPuzzle = true;
							dialogueSelect = 0;
							startPanCamera = false;
							playerControlLock = false;
						}
					}

				}

				#pragma region Snowmen Mat Switching

				if (RenderGroupBool == 2 && redSnowmanDistance <= 3.0f && startPanCamera)
				{
					startPanCamera = false;
					playerControlLock = false;
				}
				else if (RenderGroupBool == 2 && greenSnowmanDistance <= 3.0f && startPanCamera)
				{
					startPanCamera = false;
					playerControlLock = false;
				}
				else if (RenderGroupBool == 2 && blueSnowmanDistance <= 3.0f && startPanCamera)
				{
					startPanCamera = false;
					playerControlLock = false;
				}
				else if (RenderGroupBool == 2 && brownSnowmanDistance <= 3.0f && startPanCamera)
				{
					startPanCamera = false;
					playerControlLock = false;
				}
				else if (RenderGroupBool == 2 && purpleSnowmanDistance <= 3.0f && startPanCamera)
				{
					startPanCamera = false;
					playerControlLock = false;
				}
				else if (RenderGroupBool == 2 && orangeSnowmanDistance <= 3.0f && startPanCamera)
				{
					startPanCamera = false;
					playerControlLock = false;
				}
				else if (RenderGroupBool == 2 && yellowSnowmanDistance <= 3.0f && startPanCamera)
				{
					startPanCamera = false;
					playerControlLock = false;
				}
				else if (RenderGroupBool == 2 && aquaSnowmanDistance <= 3.0f && startPanCamera)
				{
					startPanCamera = false;
					playerControlLock = false;
				}
				else if (RenderGroupBool == 2 && pinkSnowmanDistance <= 3.0f && startPanCamera)
				{
					startPanCamera = false;
					playerControlLock = false;
				}

				#pragma endregion
			});

			keyToggles.emplace_back(GLFW_KEY_Y, [&]() {
				if (startQuiz)
				{
					if (quizTracker == 0)
					{
						dialogueL2.get<Sprite>().SetMaterial(quizMat2);
						quizTracker++;
					}
					else if (quizTracker == 1)
					{
						dialogueL2.get<Sprite>().SetMaterial(quizMat3);
						quizTracker++;
					}
					else if (quizTracker == 2)
					{
						dialogueL2.get<Sprite>().SetMaterial(afterMathMat1);
						quizTracker++;
					}
				}
			});

			keyToggles.emplace_back(GLFW_KEY_N, [&]() {
				if (startQuiz)
				{
					if (quizTracker == 0)
					{
						dialogueL2.get<Sprite>().SetMaterial(quizMat2);
						quizTracker++;
					}
					else if (quizTracker == 1)
					{
						dialogueL2.get<Sprite>().SetMaterial(quizMat3);
						quizTracker++;
					}
					else if (quizTracker == 2)
					{
						dialogueL2.get<Sprite>().SetMaterial(afterMathMat1);
						quizTracker++;
					}
				}
			});

			#pragma endregion
		}

		// Initialize our timing instance and grab a reference for our use
		Timing& time = Timing::Instance();
		time.LastFrame = glfwGetTime();


		#pragma region Speed Variables (+ Misc.)

		//Speed Variables
		float speed = 1.0f;
		float speedTimer = 0.0f;
		float speedTimeLimit = 2.0f;
		bool canSprint = true;

		glm::vec3 CoinDistance = glm::vec3();
		glm::vec3 WizardDistance = glm::vec3();

		float Timer = 0.0f;

		float storyTimer = 0.0f;

		#pragma endregion


		#pragma region Sound Booleans

		bool Level1Music = false;

		#pragma endregion

		int sortingValueArray[6] = { bananasObj.get<SortingInfo>().GetQuantity(), appleObj.get<SortingInfo>().GetQuantity(), 
			carrotObj.get<SortingInfo>().GetQuantity(), greenGrapeObj.get<SortingInfo>().GetQuantity(), 
			purpleGrapeObj.get<SortingInfo>().GetQuantity(), blueDonutObj.get<SortingInfo>().GetQuantity() };

		char sortingCharArray[6] = { bananasObj.get<SortingInfo>().GetAlphabet(), appleObj.get<SortingInfo>().GetAlphabet(),
			carrotObj.get<SortingInfo>().GetAlphabet(), greenGrapeObj.get<SortingInfo>().GetAlphabet(),
			purpleGrapeObj.get<SortingInfo>().GetAlphabet(), blueDonutObj.get<SortingInfo>().GetAlphabet() };

		int sortingColorArray[6] = { bananasObj.get<SortingInfo>().GetColor(), appleObj.get<SortingInfo>().GetColor(),
			carrotObj.get<SortingInfo>().GetColor(), greenGrapeObj.get<SortingInfo>().GetColor(),
			purpleGrapeObj.get<SortingInfo>().GetColor(), blueDonutObj.get<SortingInfo>().GetColor() };

		//bubbleSort(sortingValueArray, 6);
		//bubbleSort(sortingCharArray, 6);
		//bubbleSort(sortingColorArray, 6);


		PhysicsDrawer mydebugDrawer;
		dynamicsWorld->setDebugDrawer(&mydebugDrawer);

		btOverlapFilterCallback* filterCallback = new CustomFilterCallback();
		dynamicsWorld->getPairCache()->setOverlapFilterCallback(filterCallback);

		bool postEffectInit = false;
		bool initRenderer = false;

		float cutsceneTimer = 0.0f;
		bool setFinalCamera = true;
		bool isGameFinished = false;
		float vigTimer = 0.0f;
		float teleportTimer = 0.0f;
		bool isTeleporting = false;

		Application::Instance().scenes.push_back(scene0);
		Application::Instance().scenes.push_back(scene);
		Application::Instance().scenes.push_back(scene2);
		Application::Instance().scenes.push_back(scene3);
		Application::Instance().scenes.push_back(scene4);

		menu.Play();


		PixelateEffect* pixelEffect = (PixelateEffect*)scene3Effects[6];
		float intensity = pixelEffect->GetIntensity();

		

		pixelEffect->SetIntensity(intensity);

		///// Game loop /////
		while (!glfwWindowShouldClose(BackendHandler::window)) {
			glfwPollEvents();

			// Update the timing
			time.CurrentFrame = glfwGetTime();
			time.DeltaTime = static_cast<float>(time.CurrentFrame - time.LastFrame);

			time.DeltaTime = time.DeltaTime > 1.0f ? 1.0f : time.DeltaTime;

			//Update Physics World
			dynamicsWorld->stepSimulation(time.DeltaTime, 1);
			myTickCallback(dynamicsWorld, time.DeltaTime);
			dynamicsWorld->setInternalTickCallback(myTickCallback);
			dynamicsWorld->getPairCache()->setOverlapFilterCallback(filterCallback);

			#pragma region Update Sound

			//Game.cpp Update & Render//
			Update(time.DeltaTime);
			Render();

			#pragma endregion

			// Update our FPS tracker data
			fpsBuffer[frameIx] = 1.0f / time.DeltaTime;
			frameIx++;
			if (frameIx >= 128)
				frameIx = 0;
			

			#pragma region Printing Position of all Physics Bodies

			//Print positions of all objects
			for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
			{
				btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
				btRigidBody* body = btRigidBody::upcast(obj);
				btTransform trans;
				if (body && body->getMotionState())
				{
					body->getMotionState()->getWorldTransform(trans);
				}
				else
				{
					trans = obj->getWorldTransform();
				}
				//printf("World Pos Object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
			}

			//checkPosition(player);
			
			#pragma endregion

			if (RenderGroupBool == 4)
			{
				if (pixelEffect->GetIntensity() >= 400.0f && !isGameFinished)
				{
					intensity = intensity - 1.0f;
					pixelEffect->SetIntensity(intensity);
				}
			}
			

			#pragma region PhantomCol
			if (RenderGroupBool == 1)
			{
				CheckPhantomCollision(player, playerBody, playerTransform, Phantom, 0.8f, 0.8f, 0.8f, 0.8f);
				CheckPhantomCollision(player, playerBody, playerTransform, Phantom2, 0.8f, 0.8f, 0.8f, 0.8f);
			}
			else if (RenderGroupBool == 2)
			{
				//CheckPhantomCollision(player, playerBody, playerTransform, PhantomLevel2, 0.8f, 0.8f, 0.8f, 0.8f);
				//CheckPhantomCollision(player, playerBody, playerTransform, Phantom2Level2, 0.8f, 0.8f, 0.8f, 0.8f);
			}
			#pragma endregion

			#pragma region vignette controls
			if (isHit == true)
			{
				activeEffect = 5;
				vigTimer += time.DeltaTime;



				if (vigTimer >= 0.2f)
				{
					if (RenderGroupBool == 1)
						activeEffect = 4;
					else if (RenderGroupBool == 2)
						activeEffect = 2;
					else if (RenderGroupBool == 3)
						activeEffect = 1;

					isHit = false;
					vigTimer = 0.0f;


				}



			}
			#pragma endregion


			//printObjectPosition(Wizard);

			//Physics Body Slider
			btQuaternion rotation;
			rotation.setEuler(angleRotation.x, angleRotation.y, angleRotation.z);
			taigaIsland4Transform.setRotation(rotation);
			taigaIsland4Transform.setOrigin(btVector3(bodyTranslation.x, bodyTranslation.y, bodyTranslation.z));

			//taigaIsland4Body->setWorldTransform(taigaIsland4Transform);

			// We'll make sure our UI isn't focused before we start handling input for our game
			if (!ImGui::IsAnyWindowFocused()) {
				// We need to poll our key watchers so they can do their logic with the GLFW state
				// Note that since we want to make sure we don't copy our key handlers, we need a const
				// reference!
				for (const KeyPressWatcher& watcher : keyToggles) {
					watcher.Poll(BackendHandler::window);
				}
			}


			// Iterate over all the behaviour binding components
			Application::Instance().ActiveScene->Registry().view<BehaviourBinding>().each([&](entt::entity entity, BehaviourBinding& binding) {
				// Iterate over all the behaviour scripts attached to the entity, and update them in sequence (if enabled)
				for (const auto& behaviour : binding.Behaviours) {
					if (behaviour->Enabled) {
						behaviour->Update(entt::handle(Application::Instance().ActiveScene->Registry(), entity));
					}
				}
			});

			
			#pragma region Texture Toggles

			if (ToggleTextures == false)
			{

				material0->Set("s_Diffuse", WhiteDiffuse);
				material1->Set("s_Diffuse", WhiteDiffuse);
				material2->Set("s_Diffuse", WhiteDiffuse);
				material3->Set("s_Diffuse", WhiteDiffuse);
				material4->Set("s_Diffuse", WhiteDiffuse);
				material5->Set("s_Diffuse", WhiteDiffuse);
				material6->Set("s_Diffuse", WhiteDiffuse);
				material7->Set("s_Diffuse", WhiteDiffuse);
				material8->Set("s_Diffuse", WhiteDiffuse);
				material9->Set("s_Diffuse", WhiteDiffuse);
				material10->Set("s_Diffuse", WhiteDiffuse);
				material11->Set("s_Diffuse", WhiteDiffuse);
				material12->Set("s_Diffuse", WhiteDiffuse);
				material13->Set("s_Diffuse", WhiteDiffuse);
				material14->Set("s_Diffuse", WhiteDiffuse);
				material15->Set("s_Diffuse", WhiteDiffuse);
				material16->Set("s_Diffuse", WhiteDiffuse);
				material17->Set("s_Diffuse", WhiteDiffuse);
				material18->Set("s_Diffuse", WhiteDiffuse);
				material19->Set("s_Diffuse", WhiteDiffuse);
				material20->Set("s_Diffuse", WhiteDiffuse);
				material21->Set("s_Diffuse", WhiteDiffuse);
				material22->Set("s_Diffuse", WhiteDiffuse);
				material23->Set("s_Diffuse", WhiteDiffuse);
				material24->Set("s_Diffuse", WhiteDiffuse);
				material25->Set("s_Diffuse", WhiteDiffuse);
				material26->Set("s_Diffuse", WhiteDiffuse);
				material27->Set("s_Diffuse", WhiteDiffuse);
				material28->Set("s_Diffuse", WhiteDiffuse);
				material29->Set("s_Diffuse", WhiteDiffuse);
				material30->Set("s_Diffuse", WhiteDiffuse);
				material31->Set("s_Diffuse", WhiteDiffuse);
				material32->Set("s_Diffuse", WhiteDiffuse);
				material33->Set("s_Diffuse", WhiteDiffuse);

			}
			else if (ToggleTextures == true)
			{

				material0->Set("s_Diffuse", PlayerDiffuse);
				material1->Set("s_Diffuse", diffuseMp02);
				material2->Set("s_Diffuse", diffuseMp03);
				material3->Set("s_Diffuse", diffuseMp04);
				material4->Set("s_Diffuse", diffuseMp05);
				material5->Set("s_Diffuse", diffuseMp06);
				material6->Set("s_Diffuse", diffuseMp07);
				material7->Set("s_Diffuse", diffuseMp08);
				material8->Set("s_Diffuse", diffuseMp09);
				material9->Set("s_Diffuse", diffuseMp10);
				material10->Set("s_Diffuse", diffuseMp11);
				material11->Set("s_Diffuse", diffuseMp12);
				material12->Set("s_Diffuse", diffuseMp13);
				material13->Set("s_Diffuse", diffuseMp14);
				material14->Set("s_Diffuse", diffuseMp15);
				material15->Set("s_Diffuse", diffuseMp16);
				material16->Set("s_Diffuse", diffuseMp17);
				material17->Set("s_Diffuse", diffuseMp18);
				material18->Set("s_Diffuse", diffuseMp19);
				material19->Set("s_Diffuse", diffuseMp20);
				material20->Set("s_Diffuse", diffuseMp21);
				material21->Set("s_Diffuse", diffuseMp22);
				material22->Set("s_Diffuse", diffuseMp23);
				material23->Set("s_Diffuse", diffuseMp24);
				material24->Set("s_Diffuse", diffuseMp25);
				material25->Set("s_Diffuse", diffuseMp26);
				material26->Set("s_Diffuse", diffuseMp27);
				material27->Set("s_Diffuse", diffuseMp28);
				material28->Set("s_Diffuse", diffuseMp29);
				material29->Set("s_Diffuse", diffuseMp30);
				material30->Set("s_Diffuse", diffuseMp31);
				material31->Set("s_Diffuse", diffuseMp32);
				material32->Set("s_Diffuse", diffuseMp33);


			}



			#pragma endregion
			

			#pragma region Respawning

			if (glfwGetKey(BackendHandler::window, GLFW_KEY_R) == GLFW_PRESS && RenderGroupBool == 3) {
				PlayerHealth = 3;
				RenderGroupBool = 1;
				Application::Instance().ActiveScene = scene;

				CoinCount = 0;
				gotCoin = false;

				wizardTransform.setOrigin(btVector3(-14.5f, 0.0f, -2.5f));
				wizardBody->setWorldTransform(wizardTransform);
				//Wizard.get<Transform>().SetLocalPosition(-14.5f, 0.0f, -2.5f);
				Coin.get<Transform>().SetLocalPosition(6.0f, -7.0f, 0.0f);

			}

			#pragma endregion

			if (RenderGroupBool == 2 && CoinCount == 2)
			{
				parkingMeterPoleTransform.setOrigin(btVector3(50.0f, 50.0f, 50.0f));
				parkingMeterPoleBody->setWorldTransform(parkingMeterPoleTransform);
			}


			#pragma region Distance Checking

			bool playerFall = DistanceCheck(player, island1);
			bool playerFall2 = DistanceCheck(player, island2);

			bool playerFall3 = DistanceCheck(player, TaigaGround);
			bool playerFall4 = DistanceCheck(player, TaigaGround2);

			interactDistance = Distance(player, Wizard);

			if (RenderGroupBool == 2)
			{
				interactSketchyGuy1Distance = Distance(player, sketchyGuy);
				interactSketchyGuy2Distance = Distance(player, sketchyGuy2);

				redSnowmanDistance = Distance(player, SnowmanA1);
				greenSnowmanDistance = Distance(player, SnowmanA2);
				blueSnowmanDistance = Distance(player, SnowmanA3);

				brownSnowmanDistance = Distance(player, SnowmanA4);
				purpleSnowmanDistance = Distance(player, SnowmanA5);
				orangeSnowmanDistance = Distance(player, SnowmanA6);

				yellowSnowmanDistance = Distance(player, SnowmanA7);
				aquaSnowmanDistance = Distance(player, SnowmanA8);
				pinkSnowmanDistance = Distance(player, SnowmanA9);

				interactButtonDistance = Distance(player, wallButton);

				appleDistance = Distance(player, appleObj);
				bananaDistance = Distance(player, bananasObj);
				carrotDistance = Distance(player, carrotObj);

				greenGrapeDistance = Distance(player, greenGrapeObj);
				purpleGrapeDistance = Distance(player, purpleGrapeObj);
				blueDonutDistance = Distance(player, blueDonutObj);
			}

			#pragma endregion
			

			#pragma region Check for Player Falling and Player Jumping 

			checkAirborne(player);

			if (player.get<Transform>().GetLocalPosition().z <= -7.0f)
			{
				PlayerHealth--;
				//isHit = true;
				if (RenderGroupBool != 2)
				{
					playerTransform.setOrigin(btVector3(0.0f, 0.0f, 5.0f));
					playerBody->setWorldTransform(playerTransform);
				}
				else
				{
					playerTransform.setOrigin(btVector3(0.0f, 50.0f, 5.0f));
					playerBody->setWorldTransform(playerTransform);
				}

				if (RenderGroupBool == 4)
				{
					FirePlatform1Body->setLinearVelocity(btVector3(0, 0, 0));
					FirePlatform1Transform.setOrigin(btVector3(-25.0f, -5.5f, 0.5f));
					FirePlatform1Body->setWorldTransform(FirePlatform1Transform);

					FirePlatform2Body->setLinearVelocity(btVector3(0, 0, 0));
					FirePlatform2Transform.setOrigin(btVector3(-30.0f, 0.5f, 3.0f));
					FirePlatform2Body->setWorldTransform(FirePlatform2Transform);
				}
			}



			#pragma endregion


			#pragma region Switching Scenes

			//if (glfwGetKey(BackendHandler::window, GLFW_KEY_ENTER) == GLFW_PRESS && RenderGroupBool == 0) {
			//	RenderGroupBool = 1;
			//	Application::Instance().ActiveScene = scene;
			//	//BG.Play();
			//	//Level1Music == true;
			//}

			//Switching scenes when player reaches a certain point
			if (player.get<Transform>().GetLocalPosition().x <= -49.0f && RenderGroupBool == 1)
			{
				playerTransform.setOrigin(btVector3(0.0f, 50.0f, 5.0f));
				playerBody->setWorldTransform(playerTransform);
				RenderGroupBool = 2;
				Application::Instance().ActiveScene = scene2;
				BG.StopImmediately();
				Islandmusic2.Play();
				CoinCount = 0;
			}

			if (player.get<Transform>().GetLocalPosition().x <= -52.0f && RenderGroupBool == 2)
			{
				playerTransform.setOrigin(btVector3(0.0f, 0.0f, 5.0f));
				playerBody->setWorldTransform(playerTransform);
				RenderGroupBool = 4;
				Application::Instance().ActiveScene = scene4;
				Islandmusic2.StopImmediately();
				Islandmusic3.Play();
			}

			if (player.get<Transform>().GetLocalPosition().x <= -73.0f && RenderGroupBool == 4)
			{
				Islandmusic3.StopImmediately();

				footsteps.StopImmediately();
				PlayerHealth = 3;
				playerControlLock = true;
				lastMenuObj.get<Transform>().SetLocalPosition(-5.0f, 0.0f, 10.0f);

				isGameFinished = true;
			}

			if (PlayerHealth <= 0)
			{
				RenderGroupBool = 3;
				Application::Instance().ActiveScene = scene3;
			}

			/*if (RenderGroupBool == 4)
			{
				playerTransform.setOrigin(btVector3(0.0f, 0.0f, 5.0f));
				playerBody->setWorldTransform(playerTransform);
			}*/

			#pragma endregion

			if (isGameFinished)
			{
				intensity = 10000.0f;
				pixelEffect->SetIntensity(intensity);

				if (setFinalCamera)
				{
					cameraObject4.get<Transform>().SetLocalPosition(0.0f, 0.0f, 0.0f).LookAt(lastMenuObj.get<Transform>().GetLocalPosition());
					orthoCameraObject4.get<Transform>().SetLocalPosition(0.0f, 0.0f, 0.0f).LookAt(lastMenuObj.get<Transform>().GetLocalPosition());
					setFinalCamera = false;
				}

				//lastMenuObj.get<Transform>().SetLocalScale(2.0f, 2.0f, 2.0f);
				lastMenuObj.get<Sprite>().SetMaterial(cutsceneMat1);

				cutsceneTimer += time.DeltaTime;

				if (cutsceneTimer >= 5.0f)
				{
					lastMenuObj.get<Sprite>().SetMaterial(cutsceneMat2);

					if (cutsceneTimer >= 10.0f)
					{
						lastMenuObj.get<Sprite>().SetMaterial(cutsceneMat3);

						if (cutsceneTimer >= 15.0f)
						{
							lastMenuObj.get<Sprite>().SetMaterial(winMenuMat);
						}
					}
				}
			}


			#pragma region Showing Storyboard

			if (menuSelect == 3)
			{
				menu.StopImmediately();
				playMenuObj.get<Sprite>().SetMaterial(story1Mat);
				
				//Prelude2.Play();
				storyTimer += time.DeltaTime;
				Prelude1.Play();
				if (storyTimer > 8.3f)
				{
					menuSelect = 4;
					storyTimer = 0.0f;
				}
			}
			else if (menuSelect == 4)
			{
				playMenuObj.get<Sprite>().SetMaterial(story2Mat);
				
				storyTimer += time.DeltaTime;
				Prelude1.StopImmediately();
				Prelude2.Play();
				if (storyTimer > 7.4f)
				{
					menuSelect = 5;
					storyTimer = 0.0f;
				}
			}
			else if (menuSelect == 5)
			{
				playMenuObj.get<Sprite>().SetMaterial(story3Mat);
				Prelude1.StopImmediately();
				Prelude2.StopImmediately();
				Prelude3.Play();
				storyTimer += time.DeltaTime;

				if (storyTimer > 7.0f)
				{
					menuSelect = 6;
					storyTimer = 0.0f;
				}
			}
			else if (menuSelect == 6)
			{
				Prelude1.StopImmediately();
				Prelude2.StopImmediately();
				Prelude3.StopImmediately();
				menuSelect = 7;
				RenderGroupBool = 1;
				//menu.StopImmediately();
				Application::Instance().ActiveScene = scene;
			}

			#pragma endregion


			#pragma region Sorting Puzzle

			if (sortByQuantity && RenderGroupBool == 2)
			{
				GameObject arr[6] = { bananasObj2, appleObj2,
				carrotObj2, greenGrapeObj2,
				purpleGrapeObj2, blueDonutObj2 };

				bubbleSortQuantity(arr, 6, time.DeltaTime);

				sortByQuantity = false;
			}

			if (sortByLetter && RenderGroupBool == 2)
			{
				sortByLetter == false;

				GameObject arr[6] = { bananasObj2, appleObj2,
				carrotObj2, greenGrapeObj2,
				purpleGrapeObj2, blueDonutObj2 };

				bubbleSortLetter(arr, 6, time.DeltaTime);

				sortByLetter == false;
			}

			if (sortByColor && RenderGroupBool == 2)
			{

				GameObject arr[6] = { bananasObj2, appleObj2,
				carrotObj2, greenGrapeObj2,
				purpleGrapeObj2, blueDonutObj2 };

				bubbleSortColor(arr, 6, time.DeltaTime);
				sortByColor == false;
			}

			#pragma endregion


			#pragma region Puzzle Selection

			if (coinDescend1)
			{
				coin2DescendTimer += time.DeltaTime;

				//Resetting timers and adjusting back/forth booleans.
				if (coin2DescendTimer >= coin2DescendLimit)
				{
					//playerControlLock = true;
					coinDescend1 = false;
				}

				//Calculate t.
				float coin2DescendTPos = coin2DescendTimer / coin2DescendLimit;

				Coin2.get<Transform>().SetLocalPosition(LERP(glm::vec3(-19.6f, 6.6f, 50.5f), glm::vec3(-19.6f, 6.6f, -1.5f), coin2DescendTPos));
			}

			if (coinDescend2)
			{
				coin2DescendTimer += time.DeltaTime;

				//Resetting timers and adjusting back/forth booleans.
				if (coin2DescendTimer >= coin2DescendLimit)
				{
					//playerControlLock = true;
					coinDescend2 = false;
				}

				//Calculate t.
				float coin2DescendTPos = coin2DescendTimer / coin2DescendLimit;

				Coin2.get<Transform>().SetLocalPosition(LERP(sketchyGuy2.get<Transform>().GetLocalPosition() + glm::vec3(0, -5.0f, 50), sketchyGuy2.get<Transform>().GetLocalPosition() + glm::vec3(0, -5.0f, 0), coin2DescendTPos));
			}

			if (puzzleSelect)
			{
				playerControlLock = true;
				if (puzzleTracker == 0)
				{
					IceCrystal.get<Transform>().SetLocalPosition(SnowmanA1.get<Transform>().GetLocalPosition() + glm::vec3(0, 0, 4.0f));
				}
				else if (puzzleTracker == 1)
				{
					IceCrystal.get<Transform>().SetLocalPosition(SnowmanA2.get<Transform>().GetLocalPosition() + glm::vec3(0, 0, 4.0f));
				}
				else if (puzzleTracker == 2)
				{
					IceCrystal.get<Transform>().SetLocalPosition(SnowmanA3.get<Transform>().GetLocalPosition() + glm::vec3(0, 0, 4.0f));
				}
				else if (puzzleTracker == 3)
				{
					IceCrystal.get<Transform>().SetLocalPosition(SnowmanA4.get<Transform>().GetLocalPosition() + glm::vec3(0, 0, 4.0f));
				}
				else if (puzzleTracker == 4)
				{
					IceCrystal.get<Transform>().SetLocalPosition(SnowmanA5.get<Transform>().GetLocalPosition() + glm::vec3(0, 0, 4.0f));
				}
				else if (puzzleTracker == 5)
				{
					IceCrystal.get<Transform>().SetLocalPosition(SnowmanA6.get<Transform>().GetLocalPosition() + glm::vec3(0, 0, 4.0f));
				}
				else if (puzzleTracker == 6)
				{
					IceCrystal.get<Transform>().SetLocalPosition(SnowmanA7.get<Transform>().GetLocalPosition() + glm::vec3(0, 0, 4.0f));
				}
				else if (puzzleTracker == 7)
				{
					IceCrystal.get<Transform>().SetLocalPosition(SnowmanA8.get<Transform>().GetLocalPosition() + glm::vec3(0, 0, 4.0f));
				}
				else if (puzzleTracker == 8)
				{
					IceCrystal.get<Transform>().SetLocalPosition(SnowmanA9.get<Transform>().GetLocalPosition() + glm::vec3(0, 0, 4.0f));
				}

				if (puzzleTracker > 8)
				{
					IceCrystal.get<Transform>().SetLocalPosition(SnowmanA1.get<Transform>().GetLocalPosition() + glm::vec3(0, 0, 4.0f));
				}

				if (puzzleTracker < 0)
				{
					IceCrystal.get<Transform>().SetLocalPosition(SnowmanA9.get<Transform>().GetLocalPosition() + glm::vec3(0, 0, 4.0f));
				}
			}

			#pragma endregion


			#pragma region Quiz

			if (startQuiz && quizTracker == 0)
			{
				dialogueL2.get<Sprite>().SetMaterial(quizMat1);
			}

			if (quizTracker == 3)
			{
				startQuiz = false;
				startAftermath = true;
			}

			#pragma endregion


			#pragma region Interaction and Talk Sprites

			//std::cout << "\nDistance: " << purpleSnowmanDistance << std::endl;
		

			#pragma endregion


			#pragma region Phantom and Misc. Objects Movement

			if (RenderGroupBool == 1 || RenderGroupBool == 0)
			{
				UpdateCatmull(phantomWaypoints, Phantom, time.DeltaTime);

				PhantomTimer2 += time.DeltaTime;

				if (PhantomTimer2 >= PhantomTimeLimit2)
				{
					PhantomTimer2 = 0.0f;
					PhantomMove2 = !PhantomMove2;
				}

				float phantomTPos2 = PhantomTimer2 / PhantomTimeLimit2;

				if (PhantomMove2 == true)
				{
					Phantom2.get<Transform>().SetLocalPosition(LERP(startPos2, endPos2, phantomTPos2));
				}
				else if (PhantomMove2 == false)
				{
					Phantom2.get<Transform>().SetLocalPosition(LERP(endPos2, startPos2, phantomTPos2));
				}


				if (Phantom2.get<Transform>().GetLocalPosition() == startPos2)
				{
					Phantom2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
				}

				if (Phantom2.get<Transform>().GetLocalPosition() == endPos2)
				{
					flipPhantom2 = true;

					Phantom2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 180.0f);
					
				}


				ObjectTimer2 += time.DeltaTime;

				if (ObjectTimer2 >= ObjectTimeLimit)
				{
					ObjectTimer2 = 0.0f;
					moundMove = !moundMove;
					wellMove = !wellMove;
				}

				float objectTPos = ObjectTimer2 / ObjectTimeLimit;

				if (moundMove == true)
				{
					Mound.get<Transform>().SetLocalPosition(LERP(startPos4, endPos4, objectTPos));
				}
				else if (moundMove == false)
				{
					Mound.get<Transform>().SetLocalPosition(LERP(endPos4, startPos4, objectTPos));
				}

				if (wellMove == true)
				{
					WishingWell.get<Transform>().SetLocalPosition(LERP(startPos3, endPos3, objectTPos));
				}
				else if (wellMove == false)
				{
					WishingWell.get<Transform>().SetLocalPosition(LERP(endPos3, startPos3, objectTPos));
				}

				
			}
			else if (RenderGroupBool == 2)
			{
				/*Phantom.get<Transform>().SetLocalPosition(100, 100, 100);
				Phantom2.get<Transform>().SetLocalPosition(100, 100, 100);


				ObjectTimer2 += time.DeltaTime;

				if (ObjectTimer2 >= ObjectTimeLimit)
				{
					ObjectTimer2 = 0.0f;
					
				}
				float objectTPos = ObjectTimer2 / ObjectTimeLimit;


				currentPosition = glm::vec3(PhantomLevel2.get<Transform>().GetLocalPosition());

				if (currentPosition == startPos5)
				{
					Phantom2Move = !Phantom2Move;
					PhantomLevel2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 270.0f);
				}
					
				else if (currentPosition == endPos5)
				{
					Phantom2Move = !Phantom2Move;
					PhantomLevel2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
				}
					

				if (Phantom2Move == true)
					PhantomLevel2.get<Transform>().SetLocalPosition(LERP(startPos5, endPos5, objectTPos));
				else if (Phantom2Move == false)
					PhantomLevel2.get<Transform>().SetLocalPosition(LERP(endPos5, startPos5, objectTPos));

				/// ///
				
				Object2Timer2 += time.DeltaTime;

				if (Object2Timer2 >= ObjectTimeLimit)
				{
					Object2Timer2 = 0.0f;

				}
				float object2TPos = Object2Timer2 / ObjectTimeLimit;

				currentPosition2 = glm::vec3(Phantom2Level2.get<Transform>().GetLocalPosition());

				if (currentPosition2 == startPos6)
				{
					Phantom2Move2 = !Phantom2Move2;
					Phantom2Level2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
				}

				else if (currentPosition2 == endPos6)
				{
					Phantom2Move2 = !Phantom2Move2;
					Phantom2Level2.get<Transform>().SetLocalRotation(90.0f, 0.0f, 270.0f);
				}


				if (Phantom2Move2 == true)
					Phantom2Level2.get<Transform>().SetLocalPosition(LERP(startPos6, endPos6, object2TPos));
				else if (Phantom2Move2 == false)
					Phantom2Level2.get<Transform>().SetLocalPosition(LERP(endPos6, startPos6, object2TPos));*/






			}
			else if (RenderGroupBool == 4)
			{
				FirePlatform1Body->activate(true);
				FirePlatform2Body->activate(true);

				float platform1Timer;

				if (isPlatform1Collision)
				{
					platform1Timer += time.DeltaTime;

					if (platform1Timer > 2.0f)
					{
						//FirePlatform1Body->setLinearFactor(btVector3(0, 0, 1));
					}
				}

				float platform2Timer;

				if (isPlatform2Collision)
				{
					platform2Timer += time.DeltaTime;

					if (platform2Timer > 2.0f)
					{
						//FirePlatform2Body->setLinearFactor(btVector3(0, 0, 1));
					}
				}

				/*if (FirePlatform.get<Transform>().GetLocalPosition() == glm::vec3(-25.0f, -5.5f, 0.5f))
				{
					FirePlatform1Body->setLinearVelocity(btVector3(0.0, 4.0, 0.0));
				}

				if (FirePlatform.get<Transform>().GetLocalPosition() == glm::vec3(-25.0f, 5.5f, 0.5f))
				{
					FirePlatform1Body->setLinearVelocity(btVector3(0.0, -4.0, 0.0));
				}*/


				//firePlatform1Timer += time.DeltaTime;

				//if (firePlatform1Timer >= firePlatform1Limit)
				//{
				//	firePlatform1Timer = 0.0f;
				//	firePlatform1Move = !firePlatform1Move;
				//}

				//float firePlatform1Tpos = firePlatform1Timer / firePlatform1Limit;

				//if (firePlatform1Move == true)
				//{
				//	//FirePlatform1Transform.setOrigin(LERP(firePlatform1Pos1, firePlatform1Pos2, firePlatform1Tpos));
				//	//FirePlatform1Body->setWorldTransform(FirePlatform1Transform);
				//	//Greater than -35 less than -20
				//	//playerBody->setLinearVelocity(FirePlatform1Body->getLinearVelocity());

				//	/*if (player.get<Transform>().GetLocalPosition().x < -20.0f &&
				//		player.get<Transform>().GetLocalPosition().x > -35.0f &&
				//		numManifolds != 0)
				//	{
				//		playerBody->setLinearVelocity(playerBody->getLinearVelocity() + FirePlatform1Body->getLinearVelocity());
				//		std::cout << "Active 1" << std::endl;
				//	}*/
				//}
				//else if (firePlatform1Move == false)
				//{
				//	//FirePlatform1Transform.setOrigin(LERP(firePlatform1Pos2, firePlatform1Pos1, firePlatform1Tpos));
				//	//FirePlatform1Body->setWorldTransform(FirePlatform1Transform);
				//	//playerBody->setLinearVelocity(FirePlatform1Body->getLinearVelocity());

				//	/*if (player.get<Transform>().GetLocalPosition().x < -20.0f &&
				//		player.get<Transform>().GetLocalPosition().x > -35.0f &&
				//		numManifolds != 0)
				//	{
				//		playerBody->setLinearVelocity(playerBody->getLinearVelocity() + FirePlatform1Body->getLinearVelocity());
				//		std::cout << "Active 1" << std::endl;
				//	}*/
				//}


				//firePlatform2Timer += time.DeltaTime;

				//if (firePlatform2Timer >= firePlatform2Limit)
				//{
				//	firePlatform2Timer = 0.0f;
				//	firePlatform2Move = !firePlatform2Move;
				//}

				//float firePlatform2Tpos = firePlatform2Timer / firePlatform2Limit;

				//if (firePlatform2Move == true)
				//{
				//	//FirePlatform2Transform.setOrigin(LERP(firePlatform2Pos2, firePlatform2Pos1, firePlatform2Tpos));
				//	//FirePlatform2Body->setWorldTransform(FirePlatform2Transform);
				//	//playerBody->setLinearVelocity(FirePlatform2Body->getLinearVelocity());

				//	/*if (player.get<Transform>().GetLocalPosition().x < -20.0f &&
				//		player.get<Transform>().GetLocalPosition().x > -35.0f &&
				//		numManifolds != 0)
				//	{
				//		playerBody->setLinearVelocity(playerBody->getLinearVelocity() + FirePlatform2Body->getLinearVelocity());
				//		std::cout << "Active 2" << std::endl;
				//	}*/
				//}
				//else if (firePlatform2Move == false)
				//{
				//	//FirePlatform2Transform.setOrigin(LERP(firePlatform2Pos1, firePlatform2Pos2, firePlatform2Tpos));
				//	//FirePlatform2Body->setWorldTransform(FirePlatform2Transform);
				//	//playerBody->setLinearVelocity(FirePlatform2Body->getLinearVelocity());

				//	/*if (player.get<Transform>().GetLocalPosition().x < -20.0f &&
				//		player.get<Transform>().GetLocalPosition().x > -35.0f &&
				//		numManifolds != 0)
				//	{
				//		playerBody->setLinearVelocity(playerBody->getLinearVelocity() + FirePlatform2Body->getLinearVelocity());
				//		std::cout << "Active 2" << std::endl;
				//	}*/
				//}

				
			}

			#pragma endregion


			#pragma region Setting Camera and Sprite Positions

			if (RenderGroupBool == 0)
			{

			}
			else if (RenderGroupBool == 1)
			{
				if (startPanCamera)
				{
					panCamera(cameraObject, orthoCameraObject, Wizard, time.DeltaTime, 4.0f, 0.0f, 5.0f);

					dialogueL1D1.get<Transform>().SetLocalPosition(Wizard.get<Transform>().GetLocalPosition().x,
																   Wizard.get<Transform>().GetLocalPosition().y + 1.8f,
																   Wizard.get<Transform>().GetLocalPosition().z + 4.8f);

					interactionObj.get<Sprite>().SetMaterial(continueMat);

					interactionObj.get<Transform>().SetLocalPosition(Wizard.get<Transform>().GetLocalPosition().x,
						Wizard.get<Transform>().GetLocalPosition().y,
						Wizard.get<Transform>().GetLocalPosition().z + 0.3f);

					showInteraction = true;
				}

				if (interactDistance <= 8.0f && !startPanCamera && !playerAirborne && Wizard.get<Transform>().GetLocalPosition().x > -30.0f)
				{
					interactionObj.get<Sprite>().SetMaterial(talkMat);

					interactionObj.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						playerBody->getCenterOfMassTransform().getOrigin().getZ() - 2.7f);

					showInteraction = true;
				}
				else if((interactDistance > 8.0f && !startPanCamera) || playerAirborne && Wizard.get<Transform>().GetLocalPosition().x < -30.0f)
				{
					showInteraction = false;
				}

				if (!playerControlLock)
				{
					cameraObject.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 8.0f,
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						4.0f);

					orthoCameraObject.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 8.0f,
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						4.0f);

					#pragma region Player Health

					if (PlayerHealth == 3)
					{
						//Sprites
						heart1Obj.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.5f,
							cameraObject.get<Transform>().GetLocalPosition().z - 0.02f);

						heart2Obj.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.0f,
							cameraObject.get<Transform>().GetLocalPosition().z - 0.02f);

						heart3Obj.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 1.5f,
							cameraObject.get<Transform>().GetLocalPosition().z - 0.02f);
					}
					else if (PlayerHealth == 2)
					{
						//Sprites
						heart1Obj.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.5f,
							cameraObject.get<Transform>().GetLocalPosition().z - 0.02f);

						heart2Obj.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.0f,
							cameraObject.get<Transform>().GetLocalPosition().z - 0.02f);

						heart3Obj.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 1.5f,
							cameraObject.get<Transform>().GetLocalPosition().z + 6.0f);
					}
					else if (PlayerHealth == 1)
					{
						//Sprites
						heart1Obj.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.5f,
							cameraObject.get<Transform>().GetLocalPosition().z - 0.02f);

						heart2Obj.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.0f,
							cameraObject.get<Transform>().GetLocalPosition().z + 6.0f);

						heart3Obj.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 1.5f,
							cameraObject.get<Transform>().GetLocalPosition().z + 6.0f);
					}

					#pragma endregion
				}

			}
			else if (RenderGroupBool == 2)
			{
				if (startPanCamera)
				{
					if (interactDistance <= 8.0f)
					{
						panCamera(cameraObject2, orthoCameraObject2, Wizard, time.DeltaTime, 4.0f, 0.0f, 5.0f);

						dialogueL2.get<Transform>().SetLocalPosition(Wizard.get<Transform>().GetLocalPosition().x,
							Wizard.get<Transform>().GetLocalPosition().y + 1.8f,
							Wizard.get<Transform>().GetLocalPosition().z + 4.8f);

						interactionObj2.get<Sprite>().SetMaterial(continueMat);

						interactionObj2.get<Transform>().SetLocalPosition(Wizard.get<Transform>().GetLocalPosition().x,
							Wizard.get<Transform>().GetLocalPosition().y,
							Wizard.get<Transform>().GetLocalPosition().z + 0.3f);

						showInteraction = true;
					}
					else if (interactSketchyGuy1Distance <= 8.0f)
					{
						panCamera(cameraObject2, orthoCameraObject2, sketchyGuy, time.DeltaTime, 4.0f, 0.0f, 5.0f);

						dialogueL2.get<Transform>().SetLocalPosition(sketchyGuy.get<Transform>().GetLocalPosition().x,
							sketchyGuy.get<Transform>().GetLocalPosition().y + 1.8f,
							sketchyGuy.get<Transform>().GetLocalPosition().z + 4.8f);

						interactionObj2.get<Sprite>().SetMaterial(continueMat);

						interactionObj2.get<Transform>().SetLocalPosition(sketchyGuy.get<Transform>().GetLocalPosition().x,
							sketchyGuy.get<Transform>().GetLocalPosition().y,
							sketchyGuy.get<Transform>().GetLocalPosition().z + 0.3f);

						showInteraction = true;
					}
					else if (interactSketchyGuy2Distance <= 8.0f)
					{
						panCamera(cameraObject2, orthoCameraObject2, sketchyGuy2, time.DeltaTime, 4.0f, 0.0f, 5.0f);

						dialogueL2.get<Transform>().SetLocalPosition(sketchyGuy2.get<Transform>().GetLocalPosition().x,
							sketchyGuy2.get<Transform>().GetLocalPosition().y + 1.8f,
							sketchyGuy2.get<Transform>().GetLocalPosition().z + 4.8f);

						if (startQuiz)
						{
							interactionObj2.get<Sprite>().SetMaterial(quizSelectMat);
						}
						else
						{
							interactionObj2.get<Sprite>().SetMaterial(continueMat);
						}

						interactionObj2.get<Transform>().SetLocalPosition(sketchyGuy2.get<Transform>().GetLocalPosition().x,
							sketchyGuy2.get<Transform>().GetLocalPosition().y,
							sketchyGuy2.get<Transform>().GetLocalPosition().z + 0.3f);

						showInteraction = true;
					}
					else if (interactButtonDistance <= 5.0f)
					{
						panCamera(cameraObject2, orthoCameraObject2, wallButton, time.DeltaTime, 45.0f, 0.0f, 20.0f);

						/*dialogueL2.get<Transform>().SetLocalPosition(wallButton.get<Transform>().GetLocalPosition().x,
							sketchyGuy.get<Transform>().GetLocalPosition().y + 1.8f,
							sketchyGuy.get<Transform>().GetLocalPosition().z + 4.8f);*/

						interactionObj2.get<Sprite>().SetMaterial(continueMat);

						interactionObj2.get<Transform>().SetLocalPosition(wallButton.get<Transform>().GetLocalPosition().x,
							sketchyGuy.get<Transform>().GetLocalPosition().y,
							sketchyGuy.get<Transform>().GetLocalPosition().z + 0.3f);

						showInteraction = true;
					}
					else if (startSortingPuzzle && interactSketchyGuy2Distance <= 8.0f)
					{
						panCamera(cameraObject2, orthoCameraObject2, sketchyGuy2, time.DeltaTime, -20.0f, 0.0f, 20.0f);

						/*dialogueL2.get<Transform>().SetLocalPosition(wallButton.get<Transform>().GetLocalPosition().x,
							sketchyGuy.get<Transform>().GetLocalPosition().y + 1.8f,
							sketchyGuy.get<Transform>().GetLocalPosition().z + 4.8f);*/

						interactionObj2.get<Transform>().SetLocalPosition(sketchyGuy2.get<Transform>().GetLocalPosition().x - 20.0f,
							sketchyGuy2.get<Transform>().GetLocalPosition().y,
							sketchyGuy2.get<Transform>().GetLocalPosition().z + 15.0f);
						GameObject arr[6] = { bananasObj2, appleObj2,
												carrotObj2, greenGrapeObj2,
												purpleGrapeObj2, blueDonutObj2 };

						showInteraction = true;

						
					}
					#pragma region Snowmen Distances

					else if (redSnowmanDistance <= 3.0f)
					{
						panCamera(cameraObject2, orthoCameraObject2, SnowmanA1, time.DeltaTime, 4.0f, 0.0f, 5.0f);

						dialogueL2.get<Transform>().SetLocalPosition(SnowmanA1.get<Transform>().GetLocalPosition().x,
							SnowmanA1.get<Transform>().GetLocalPosition().y + 1.8f,
							SnowmanA1.get<Transform>().GetLocalPosition().z + 4.8f);

						interactionObj2.get<Sprite>().SetMaterial(continueMat);

						interactionObj2.get<Transform>().SetLocalPosition(SnowmanA1.get<Transform>().GetLocalPosition().x,
							SnowmanA1.get<Transform>().GetLocalPosition().y,
							SnowmanA1.get<Transform>().GetLocalPosition().z + 0.3f);

						showInteraction = true;
					}
					else if (greenSnowmanDistance <= 3.0f)
					{
						panCamera(cameraObject2, orthoCameraObject2, SnowmanA2, time.DeltaTime, 4.0f, 0.0f, 5.0f);

						dialogueL2.get<Transform>().SetLocalPosition(SnowmanA2.get<Transform>().GetLocalPosition().x,
							SnowmanA2.get<Transform>().GetLocalPosition().y + 1.8f,
							SnowmanA2.get<Transform>().GetLocalPosition().z + 4.8f);

						interactionObj2.get<Sprite>().SetMaterial(continueMat);

						interactionObj2.get<Transform>().SetLocalPosition(SnowmanA2.get<Transform>().GetLocalPosition().x,
							SnowmanA2.get<Transform>().GetLocalPosition().y,
							SnowmanA2.get<Transform>().GetLocalPosition().z + 0.3f);

						showInteraction = true;
					}
					else if (blueSnowmanDistance <= 3.0f)
					{
						panCamera(cameraObject2, orthoCameraObject2, SnowmanA3, time.DeltaTime, 4.0f, 0.0f, 5.0f);

						dialogueL2.get<Transform>().SetLocalPosition(SnowmanA3.get<Transform>().GetLocalPosition().x,
							SnowmanA3.get<Transform>().GetLocalPosition().y + 1.8f,
							SnowmanA3.get<Transform>().GetLocalPosition().z + 4.8f);

						interactionObj2.get<Sprite>().SetMaterial(continueMat);

						interactionObj2.get<Transform>().SetLocalPosition(SnowmanA3.get<Transform>().GetLocalPosition().x,
							SnowmanA3.get<Transform>().GetLocalPosition().y,
							SnowmanA3.get<Transform>().GetLocalPosition().z + 0.3f);

						showInteraction = true;
					}
					else if (brownSnowmanDistance <= 3.0f)
					{
						panCamera(cameraObject2, orthoCameraObject2, SnowmanA4, time.DeltaTime, 4.0f, 0.0f, 5.0f);

						dialogueL2.get<Transform>().SetLocalPosition(SnowmanA4.get<Transform>().GetLocalPosition().x,
							SnowmanA4.get<Transform>().GetLocalPosition().y + 1.8f,
							SnowmanA4.get<Transform>().GetLocalPosition().z + 4.8f);

						interactionObj2.get<Sprite>().SetMaterial(continueMat);

						interactionObj2.get<Transform>().SetLocalPosition(SnowmanA4.get<Transform>().GetLocalPosition().x,
							SnowmanA4.get<Transform>().GetLocalPosition().y,
							SnowmanA4.get<Transform>().GetLocalPosition().z + 0.3f);

						showInteraction = true;
					}
					else if (purpleSnowmanDistance <= 3.0f)
					{
						panCamera(cameraObject2, orthoCameraObject2, SnowmanA5, time.DeltaTime, 4.0f, 0.0f, 5.0f);

						dialogueL2.get<Transform>().SetLocalPosition(SnowmanA5.get<Transform>().GetLocalPosition().x,
							SnowmanA5.get<Transform>().GetLocalPosition().y + 1.8f,
							SnowmanA5.get<Transform>().GetLocalPosition().z + 4.8f);

						interactionObj2.get<Sprite>().SetMaterial(continueMat);

						interactionObj2.get<Transform>().SetLocalPosition(SnowmanA5.get<Transform>().GetLocalPosition().x,
							SnowmanA5.get<Transform>().GetLocalPosition().y,
							SnowmanA5.get<Transform>().GetLocalPosition().z + 0.3f);

						showInteraction = true;
					}
					else if (orangeSnowmanDistance <= 3.0f)
					{
						panCamera(cameraObject2, orthoCameraObject2, SnowmanA6, time.DeltaTime, 4.0f, 0.0f, 5.0f);

						dialogueL2.get<Transform>().SetLocalPosition(SnowmanA6.get<Transform>().GetLocalPosition().x,
							SnowmanA6.get<Transform>().GetLocalPosition().y + 1.8f,
							SnowmanA6.get<Transform>().GetLocalPosition().z + 4.8f);

						interactionObj2.get<Sprite>().SetMaterial(continueMat);

						interactionObj2.get<Transform>().SetLocalPosition(SnowmanA6.get<Transform>().GetLocalPosition().x,
							SnowmanA6.get<Transform>().GetLocalPosition().y,
							SnowmanA6.get<Transform>().GetLocalPosition().z + 0.3f);

						showInteraction = true;
					}
					else if (yellowSnowmanDistance <= 3.0f)
					{
						panCamera(cameraObject2, orthoCameraObject2, SnowmanA7, time.DeltaTime, 4.0f, 0.0f, 5.0f);

						dialogueL2.get<Transform>().SetLocalPosition(SnowmanA7.get<Transform>().GetLocalPosition().x,
							SnowmanA7.get<Transform>().GetLocalPosition().y + 1.8f,
							SnowmanA7.get<Transform>().GetLocalPosition().z + 4.8f);

						interactionObj2.get<Sprite>().SetMaterial(continueMat);

						interactionObj2.get<Transform>().SetLocalPosition(SnowmanA7.get<Transform>().GetLocalPosition().x,
							SnowmanA7.get<Transform>().GetLocalPosition().y,
							SnowmanA7.get<Transform>().GetLocalPosition().z + 0.3f);

						showInteraction = true;
					}
					else if (aquaSnowmanDistance <= 3.0f)
					{
						panCamera(cameraObject2, orthoCameraObject2, SnowmanA8, time.DeltaTime, 4.0f, 0.0f, 5.0f);

						dialogueL2.get<Transform>().SetLocalPosition(SnowmanA8.get<Transform>().GetLocalPosition().x,
							SnowmanA8.get<Transform>().GetLocalPosition().y + 1.8f,
							SnowmanA8.get<Transform>().GetLocalPosition().z + 4.8f);

						interactionObj2.get<Sprite>().SetMaterial(continueMat);

						interactionObj2.get<Transform>().SetLocalPosition(SnowmanA8.get<Transform>().GetLocalPosition().x,
							SnowmanA8.get<Transform>().GetLocalPosition().y,
							SnowmanA8.get<Transform>().GetLocalPosition().z + 0.3f);

						showInteraction = true;
					}
					else if (pinkSnowmanDistance <= 3.0f)
					{
						panCamera(cameraObject2, orthoCameraObject2, SnowmanA9, time.DeltaTime, 4.0f, 0.0f, 5.0f);

						dialogueL2.get<Transform>().SetLocalPosition(SnowmanA9.get<Transform>().GetLocalPosition().x,
							SnowmanA9.get<Transform>().GetLocalPosition().y + 1.8f,
							SnowmanA9.get<Transform>().GetLocalPosition().z + 4.8f);

						interactionObj2.get<Sprite>().SetMaterial(continueMat);

						interactionObj2.get<Transform>().SetLocalPosition(SnowmanA9.get<Transform>().GetLocalPosition().x,
							SnowmanA9.get<Transform>().GetLocalPosition().y,
							SnowmanA9.get<Transform>().GetLocalPosition().z + 0.3f);

						showInteraction = true;
					}
					#pragma endregion

					
				}
				
				if ((interactDistance <= 8.0f || interactSketchyGuy1Distance <= 8.0f || interactSketchyGuy2Distance <= 8.0f || interactButtonDistance <= 5.0f) &&
					!startPanCamera && !playerAirborne)
				{
					if (interactDistance <= 8.0f)
					{
						dialogueL2.get<Sprite>().SetMaterial(dialogueMatL2D1);
					}

					if (interactSketchyGuy1Distance <= 8.0f)
					{
						dialogueL2.get<Sprite>().SetMaterial(sketchyGuy1Mat1);
					}

					if (interactSketchyGuy2Distance <= 8.0f)
					{
						dialogueL2.get<Sprite>().SetMaterial(sketchyGuy2Mat1);
					}

					if (interactButtonDistance <= 5.0f)
					{
						interactionObj2.get<Sprite>().SetMaterial(interactMat);
					}
					else
					{
						interactionObj2.get<Sprite>().SetMaterial(talkMat);
					}

					interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

					showInteraction = true;
				}
				else if (((interactDistance > 8.0f && interactSketchyGuy1Distance > 8.0f && interactSketchyGuy2Distance > 8.0f && interactButtonDistance > 5.0f) && !startPanCamera) || playerAirborne)
				{
					showInteraction = false;
				}


				#pragma region Snowmen Distances

				if (redSnowmanDistance <= 3.0f && !startPanCamera && !playerAirborne)
				{
					interactionObj2.get<Sprite>().SetMaterial(interactMat);

					interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

					showInteraction = true;
				}
				else if (greenSnowmanDistance <= 3.0f && !startPanCamera && !playerAirborne)
				{
					interactionObj2.get<Sprite>().SetMaterial(interactMat);

					interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

					showInteraction = true;
				}
				else if (blueSnowmanDistance <= 3.0f && !startPanCamera && !playerAirborne)
				{
					interactionObj2.get<Sprite>().SetMaterial(interactMat);

					interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

					showInteraction = true;
				}
				else if (brownSnowmanDistance <= 3.0f && !startPanCamera && !playerAirborne)
				{
					interactionObj2.get<Sprite>().SetMaterial(interactMat);

					interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

					showInteraction = true;
				}
				else if (purpleSnowmanDistance <= 3.0f && !startPanCamera && !playerAirborne)
				{
					interactionObj2.get<Sprite>().SetMaterial(interactMat);

					interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

					showInteraction = true;
				}
				else if (orangeSnowmanDistance <= 3.0f && !startPanCamera && !playerAirborne)
				{
					interactionObj2.get<Sprite>().SetMaterial(interactMat);

					interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

					showInteraction = true;
				}
				else if (yellowSnowmanDistance <= 3.0f && !startPanCamera && !playerAirborne)
				{
					interactionObj2.get<Sprite>().SetMaterial(interactMat);

					interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

					showInteraction = true;
				}
				else if (aquaSnowmanDistance <= 3.0f && !startPanCamera && !playerAirborne)
				{
					interactionObj2.get<Sprite>().SetMaterial(interactMat);

					interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

					showInteraction = true;
				}
				else if (pinkSnowmanDistance <= 3.0f && !startPanCamera && !playerAirborne)
				{
					interactionObj2.get<Sprite>().SetMaterial(interactMat);

					interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

					showInteraction = true;
				}
				

				#pragma endregion


				#pragma region Sorting Objects Interaction Displays
				if (!sortingItemHeld)
				{
					if (bananaDistance <= 3.0f && !startPanCamera && !playerAirborne)
					{
						interactionObj2.get<Sprite>().SetMaterial(selectObjectMat);

						interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
							playerBody->getCenterOfMassTransform().getOrigin().getY(),
							playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

						showInteraction = true;
					}
					else if (appleDistance <= 3.0f && !startPanCamera && !playerAirborne)
					{
						interactionObj2.get<Sprite>().SetMaterial(selectObjectMat);

						interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
							playerBody->getCenterOfMassTransform().getOrigin().getY(),
							playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

						showInteraction = true;
					}
					else if (carrotDistance <= 3.0f && !startPanCamera && !playerAirborne)
					{
						interactionObj2.get<Sprite>().SetMaterial(selectObjectMat);

						interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
							playerBody->getCenterOfMassTransform().getOrigin().getY(),
							playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

						showInteraction = true;
					}
					else if (greenGrapeDistance <= 3.0f && !startPanCamera && !playerAirborne)
					{
						interactionObj2.get<Sprite>().SetMaterial(selectObjectMat);

						interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
							playerBody->getCenterOfMassTransform().getOrigin().getY(),
							playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

						showInteraction = true;
					}
					else if (purpleGrapeDistance <= 3.0f && !startPanCamera && !playerAirborne)
					{
						interactionObj2.get<Sprite>().SetMaterial(selectObjectMat);

						interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
							playerBody->getCenterOfMassTransform().getOrigin().getY(),
							playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

						showInteraction = true;
					}
					else if (blueDonutDistance <= 3.0f && !startPanCamera && !playerAirborne)
					{
						interactionObj2.get<Sprite>().SetMaterial(selectObjectMat);

						interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
							playerBody->getCenterOfMassTransform().getOrigin().getY(),
							playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

						showInteraction = true;
					}
				}
				else
				{
					if (bananaDistance <= 3.0f && !startPanCamera && !playerAirborne)
					{
						interactionObj2.get<Sprite>().SetMaterial(swapObjectMat);

						interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
							playerBody->getCenterOfMassTransform().getOrigin().getY(),
							playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

						showInteraction = true;
					}
					else if (appleDistance <= 3.0f && !startPanCamera && !playerAirborne)
					{
						interactionObj2.get<Sprite>().SetMaterial(swapObjectMat);

						interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
							playerBody->getCenterOfMassTransform().getOrigin().getY(),
							playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

						showInteraction = true;
					}
					else if (carrotDistance <= 3.0f && !startPanCamera && !playerAirborne)
					{
						interactionObj2.get<Sprite>().SetMaterial(swapObjectMat);

						interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
							playerBody->getCenterOfMassTransform().getOrigin().getY(),
							playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

						showInteraction = true;
					}
					else if (greenGrapeDistance <= 3.0f && !startPanCamera && !playerAirborne)
					{
						interactionObj2.get<Sprite>().SetMaterial(swapObjectMat);

						interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
							playerBody->getCenterOfMassTransform().getOrigin().getY(),
							playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

						showInteraction = true;
					}
					else if (purpleGrapeDistance <= 3.0f && !startPanCamera && !playerAirborne)
					{
						interactionObj2.get<Sprite>().SetMaterial(swapObjectMat);

						interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
							playerBody->getCenterOfMassTransform().getOrigin().getY(),
							playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

						showInteraction = true;
					}
					else if (blueDonutDistance <= 3.0f && !startPanCamera && !playerAirborne)
					{
						interactionObj2.get<Sprite>().SetMaterial(swapObjectMat);

						interactionObj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(),
							playerBody->getCenterOfMassTransform().getOrigin().getY(),
							playerBody->getCenterOfMassTransform().getOrigin().getZ() - 0.7f);

						showInteraction = true;
					}
				}

				#pragma endregion

				

				if (!playerControlLock)
				{
					cameraObject2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 8.0f,
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						4.0f);

					orthoCameraObject2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 8.0f,
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						4.0f);

					#pragma region Player Health

					if (PlayerHealth == 3)
					{
						//Sprites
						heart1Obj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.5f,
							cameraObject2.get<Transform>().GetLocalPosition().z - 0.02f);

						heart2Obj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.0f,
							cameraObject2.get<Transform>().GetLocalPosition().z - 0.02f);

						heart3Obj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 1.5f,
							cameraObject2.get<Transform>().GetLocalPosition().z - 0.02f);
					}
					else if (PlayerHealth == 2)
					{
						//Sprites
						heart1Obj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.5f,
							cameraObject2.get<Transform>().GetLocalPosition().z - 0.02f);

						heart2Obj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.0f,
							cameraObject2.get<Transform>().GetLocalPosition().z - 0.02f);

						heart3Obj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 1.5f,
							cameraObject2.get<Transform>().GetLocalPosition().z + 6.0f);
					}
					else if (PlayerHealth == 1)
					{
						//Sprites
						heart1Obj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.5f,
							cameraObject2.get<Transform>().GetLocalPosition().z - 0.02f);

						heart2Obj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.0f,
							cameraObject2.get<Transform>().GetLocalPosition().z + 6.0f);

						heart3Obj2.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 1.5f,
							cameraObject2.get<Transform>().GetLocalPosition().z + 6.0f);
					}

					#pragma endregion

				}
			}
			else if (RenderGroupBool == 4)
			{
				if (!playerControlLock)
				{
					cameraObject4.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 8.0f,
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						playerBody->getCenterOfMassTransform().getOrigin().getZ() + 5.0f);

					orthoCameraObject4.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 8.0f,
						playerBody->getCenterOfMassTransform().getOrigin().getY(),
						4.0f);

					#pragma region Player Health

					if (PlayerHealth == 3)
					{
						//Sprites
						heart1Obj3.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.5f,
							3.98f);

						heart2Obj3.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.0f,
							3.98f);

						heart3Obj3.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 1.5f,
							3.98f);
					}
					else if (PlayerHealth == 2)
					{
						//Sprites
						heart1Obj3.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.5f,
							3.98f);

						heart2Obj3.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.0f,
							3.98f);

						heart3Obj3.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 1.5f,
							6.0f);
					}
					else if (PlayerHealth == 1)
					{
						//Sprites
						heart1Obj3.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.5f,
							3.98f);

						heart2Obj3.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 2.0f,
							6.0f);

						heart3Obj3.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX() + 2.0f,
							playerBody->getCenterOfMassTransform().getOrigin().getY() - 1.5f,
							6.0f);
					}

					#pragma endregion
				}
			}


			#pragma endregion


			#pragma region Move Coin and Wizard Level 1

			//GetDistance(player, Coin, CoinDistance);

			if (moveWizard1)
			{
				
				wizardMoveTimer += time.DeltaTime;
				isTeleporting = true;
				 //Teleport

				if (wizardMoveTimer >= 1.5f)
				{
					wizardTransform.setOrigin(btVector3(-47.5f, 5.0f, -2.5f));
					wizardBody->setWorldTransform(wizardTransform);
					
					btCollisionShape* wizardShape = new btBoxShape(btVector3(1.1f, 1.1f, 2.4f));
					wizardBody->setCollisionShape(wizardShape);

					dialogueL1D1.get<Sprite>().SetMaterial(dialogueMatL2D1);
					

					wizardMoveTimer = 0.0f;
					moveWizard1 = false;
					isTeleporting = false;
				}
				
			}
			if (isTeleporting)
			{
				Wizard.get<MorphRenderer>().nextFrame(time.DeltaTime, 1);
			}
			else
			{
				Wizard.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
			}
			//MoveWizard(player, Wizard, wizardBody, wizardTransform, WizardDistance);

			#pragma endregion


			#pragma region Checking Collision

			//CheckCollision(player, Wizard, 0.85f, 0.85f, 0.85f, 0.85f);
		

			#pragma endregion


			#pragma region Player and Wizard Physics

			//playerBody->applyDamping(time.DeltaTime);
			playerBody->setAngularFactor(btVector3(0, 0, 0));

			if (playerJump)
			{
				playerBody->activate(true);

				glfwGetWindowSize(BackendHandler::window, &width, &height);

				if (width > 900)
				{
					playerBody->applyCentralImpulse(btVector3(0.0f, 0.0f, 6000.0f) * time.DeltaTime);
				}
				else
				{
					playerBody->applyCentralImpulse(btVector3(0.0f, 0.0f, 3000.0f)* time.DeltaTime);
				}
				playerJump = false;
			}

			//Zero Velocity when not moving
			if (glfwGetKey(BackendHandler::window, GLFW_KEY_W) != GLFW_PRESS &&
				glfwGetKey(BackendHandler::window, GLFW_KEY_A) != GLFW_PRESS &&
				glfwGetKey(BackendHandler::window, GLFW_KEY_S) != GLFW_PRESS &&
				glfwGetKey(BackendHandler::window, GLFW_KEY_D) != GLFW_PRESS &&
				glfwGetKey(BackendHandler::window, GLFW_KEY_SPACE) != GLFW_PRESS)
			{
				playerBody->setLinearVelocity(btVector3(0, 0, playerBody->getLinearVelocity().getZ()));
			}

			player.get<Transform>().SetLocalPosition(playerBody->getCenterOfMassTransform().getOrigin().getX(), playerBody->getCenterOfMassTransform().getOrigin().getY(), playerBody->getCenterOfMassTransform().getOrigin().getZ());

			if (RenderGroupBool == 2)
			{
				wizardTransform.setOrigin(btVector3(-10.1f, 45.9f, -1.8f));
				wizardBody->setWorldTransform(wizardTransform);

				//Coin2Transform.setOrigin(btVector3(bodyTranslation.x, bodyTranslation.y, bodyTranslation.z));
				//sketchyGuy2Body->setWorldTransform(sketchyGuy2Transform);


				//bananasObj.get<Transform>().SetLocalPosition(bodyTranslation.x, bodyTranslation.y, bodyTranslation.z);
				//bananasObj.get<Transform>().SetLocalRotation(angleRotation.x, angleRotation.y, angleRotation.z);

				//btQuaternion rotation2;
				//rotation2.setEuler(angleRotation.x, angleRotation.y, angleRotation.z);
				//parkingMeterTransform.setRotation(rotation2);
				//parkingMeterPole.get<Transform>().SetLocalRotation(angleRotation.x, angleRotation.y, angleRotation.z);

				//parkingMeterTransform.setOrigin(btVector3(bodyTranslation.x, bodyTranslation.y, bodyTranslation.z));
				//parkingMeterBody->setWorldTransform(parkingMeterTransform);

				//btCollisionShape* parkingMeterPoleShape = new btBoxShape(btVector3(bodyTranslation.x, bodyTranslation.y, bodyTranslation.z));
				//parkingMeterPoleBody->setCollisionShape(parkingMeterPoleShape);
			}

			#pragma endregion


			#pragma region Player Sprint

			if (glfwGetKey(BackendHandler::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			{
				speed = 2.2f;
				player.get<MorphRenderer>().SetFrameTime(0.15, 1);
			}
			else
			{
				speed = 1.0f;
				player.get<MorphRenderer>().SetFrameTime(0.25, 1);
			}

			#pragma endregion


			
			// Clear the screen
			basicEffect0->Clear();
			basicEffect->Clear();
			basicEffect2->Clear();
			basicEffect3->Clear();
			/*greyscaleEffect->Clear();
			sepiaEffect->Clear();*/
			for (int i = 0; i < effects.size(); i++)
			{
				effects[i]->Clear();
			}
			for (int i = 0; i < scene2Effects.size(); i++)
			{
				scene2Effects[i]->Clear();
			}
			for (int i = 0; i < scene3Effects.size(); i++)
			{
				scene3Effects[i]->Clear();
			}
			shadowBuffer->Clear();
			//shadowBuffer2->Clear();

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glEnable(GL_DEPTH_TEST);
			glClearDepth(1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Update all world matrices for this frame
			Application::Instance().ActiveScene->Registry().view<Transform>().each([](entt::entity entity, Transform& t) {
				t.UpdateWorldMatrix();
			});
			
			

			#pragma region Render Menu Scene (Scene 0)

			if (RenderGroupBool == 0)
			{
				// Grab out camera info from the camera object
				Transform& camTransform = cameraObject0.get<Transform>();
				glm::mat4 view = glm::inverse(camTransform.LocalTransform());
				glm::mat4 projection = cameraObject0.get<Camera>().GetProjection();
				glm::mat4 viewProjection = projection * view;

				// Grab out camera info from the camera object
				Transform& orthoCamTransform = orthoCameraObject0.get<Transform>();
				glm::mat4 orthoView = glm::inverse(orthoCamTransform.LocalTransform());
				glm::mat4 orthoProjection = orthoCameraObject0.get<Camera>().GetProjection();
				glm::mat4 orthoViewProjection = orthoProjection * orthoView;

				// Sort the renderers by shader and material, we will go for a minimizing context switches approach here,
				// but you could for instance sort front to back to optimize for fill rate if you have intensive fragment shaders
				renderGroup0.sort<RendererComponent>([](const RendererComponent& l, const RendererComponent& r) {
					// Sort by render layer first, higher numbers get drawn last
					if (l.Material->RenderLayer < r.Material->RenderLayer) return true;
					if (l.Material->RenderLayer > r.Material->RenderLayer) return false;

					// Sort by shader pointer next (so materials using the same shader run sequentially where possible)
					if (l.Material->Shader < r.Material->Shader) return true;
					if (l.Material->Shader > r.Material->Shader) return false;

					// Sort by material pointer last (so we can minimize switching between materials)
					if (l.Material < r.Material) return true;
					if (l.Material > r.Material) return false;

					return false;
				});

				// Start by assuming no shader or material is applied
				Shader::sptr current = nullptr;
				ShaderMaterial::sptr currentMat = nullptr;

				activeEffect = 0;

				glfwGetWindowSize(BackendHandler::window, &width, &height);

				glViewport(0, 0, width, height);
				basicEffect0->BindBuffer(0);


				if (width > 900)
				{
					playMenuObj.get<Transform>().SetLocalScale(1.14f, 1.0f, 0.6f);
				}
				else
				{
					playMenuObj.get<Transform>().SetLocalScale(0.6f, 0.6f, 0.6f);
				}

				// Iterate over the render group components and draw them
				renderGroup0.each([&](entt::entity e, RendererComponent& renderer, Transform& transform) {
					// If the shader has changed, set up it's uniforms
					if (current != renderer.Material->Shader) {
						current = renderer.Material->Shader;
						current->Bind();
						BackendHandler::SetupShaderForFrame(current, view, projection);
					}
					// If the material has changed, apply it
					if (currentMat != renderer.Material) {
						currentMat = renderer.Material;
						currentMat->Apply();
					}

					// Render the mesh
					BackendHandler::RenderVAO(renderer.Material->Shader, renderer.Mesh, viewProjection, transform);
				});

				#pragma region Rendering Animated Models

				//Animated Models
				Wizard.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				Wizard.get<MorphRenderer>().render(morphShader, viewProjection, Wizard.get<Transform>(), view, viewProjection);

				Phantom.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				Phantom.get<MorphRenderer>().render(morphShader, viewProjection, Phantom.get<Transform>(), view, viewProjection);

				Phantom2.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				Phantom2.get<MorphRenderer>().render(morphShader, viewProjection, Phantom2.get<Transform>(), view, viewProjection);

				Coin.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				Coin.get<MorphRenderer>().render(morphShader, viewProjection, Coin.get<Transform>(), view, viewProjection);

				#pragma endregion


				#pragma region Render Player

				//Update Animation
				player.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				player.get<MorphRenderer>().render(morphShader, viewProjection, player.get<Transform>(), view, viewProjection);

				

				#pragma endregion


				#pragma region Scene Sounds

				//menu.Play();

				#pragma endregion


				#pragma region Rendering Sprites

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				//spriteShader->Bind();
				ShaderMaterial::sptr currentMenuMat = nullptr;
				BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
				currentMenuMat = playMenuObj.get<Sprite>().Material;
				currentMenuMat->Apply();
				BackendHandler::RenderVAO(spriteShader, menuVAO, orthoViewProjection, playMenuObj.get<Transform>());
				glDisable(GL_BLEND);

				#pragma endregion


				basicEffect0->UnbindBuffer();

				basicEffect0->ApplyEffect(basicEffect0);

				basicEffect0->DrawToScreen();

			}

			#pragma endregion


			#pragma region Render Level 1 (Scene 1)

			if (RenderGroupBool == 1)
			{
				// Grab out camera info from the camera object
				Transform& camTransform = cameraObject.get<Transform>();
				glm::mat4 view = glm::inverse(camTransform.LocalTransform());
				glm::mat4 projection = cameraObject.get<Camera>().GetProjection();
				glm::mat4 viewProjection = projection * view;

				//Set up light space matrix (Increase ortho values to expand size for scene)
				//glm::mat4 lightProjectionMatrix = glm::ortho(-80.0f, 80.0f, -80.0f, 80.0f, -120.0f, 120.0f);
				glm::mat4 lightProjectionMatrix = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -30.0f, 30.0f);
				glm::mat4 lightViewMatrix = glm::lookAt(glm::vec3(-theSun._lightDirection), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f));
				glm::mat4 lightSpaceViewProj = lightProjectionMatrix * lightViewMatrix;

				// Grab out camera info from the camera object
				Transform& orthoCamTransform = orthoCameraObject.get<Transform>();
				glm::mat4 orthoView = glm::inverse(orthoCamTransform.LocalTransform());
				glm::mat4 orthoProjection = orthoCameraObject.get<Camera>().GetProjection();
				glm::mat4 orthoViewProjection = orthoProjection * orthoView;
				

				//Initializes first frame variables
				if (initScene1)
				{
					activeEffect = 4;
					
					initScene1 = false;
				}

				// Sort the renderers by shader and material, we will go for a minimizing context switches approach here,
				// but you could for instance sort front to back to optimize for fill rate if you have intensive fragment shaders
				renderGroup.sort<RendererComponent>([](const RendererComponent& l, const RendererComponent& r) {
					// Sort by render layer first, higher numbers get drawn last
					if (l.Material->RenderLayer < r.Material->RenderLayer) return true;
					if (l.Material->RenderLayer > r.Material->RenderLayer) return false;

					// Sort by shader pointer next (so materials using the same shader run sequentially where possible)
					if (l.Material->Shader < r.Material->Shader) return true;
					if (l.Material->Shader > r.Material->Shader) return false;

					// Sort by material pointer last (so we can minimize switching between materials)
					if (l.Material < r.Material) return true;
					if (l.Material > r.Material) return false;

					return false;
				});
				

				// Start by assuming no shader or material is applied
				Shader::sptr current = nullptr;
				ShaderMaterial::sptr currentMat = nullptr;

				glViewport(0, 0, shadowWidth, shadowHeight);
				shadowBuffer->Bind();

				//renderGroup.each([&](entt::entity e, RendererComponent& renderer, Transform& transform)

				renderGroup.each([&](entt::entity e, RendererComponent& renderer, Transform& transform) {
					// Render the mesh
					if (renderer.CastShadows)
					{
						//BackendHandler::RenderVAO(simpleDepthShader, renderer.Mesh, viewProjection, transform, lightSpaceViewProj);
						BackendHandler::RenderVAO(simpleDepthShader, renderer.Mesh, viewProjection, transform, lightSpaceViewProj);
						
					}
				});

				player.get<MorphRenderer>().render(simpleDepthShader, viewProjection, player.get<Transform>(), view, viewProjection, lightSpaceViewProj);
				Wizard.get<MorphRenderer>().render(simpleDepthShader, viewProjection, Wizard.get<Transform>(), view, viewProjection, lightSpaceViewProj);
				Phantom.get<MorphRenderer>().render(simpleDepthShader, viewProjection, Phantom.get<Transform>(), view, viewProjection, lightSpaceViewProj);
				Phantom2.get<MorphRenderer>().render(simpleDepthShader, viewProjection, Phantom2.get<Transform>(), view, viewProjection, lightSpaceViewProj);
				Coin.get<MorphRenderer>().render(simpleDepthShader, viewProjection, Coin.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				shadowBuffer->Unbind();

				glfwGetWindowSize(BackendHandler::window, &width, &height);

				glViewport(0, 0, width, height);
				basicEffect->BindBuffer(0);

				// Iterate over the render group components and draw them
				renderGroup.each([&](entt::entity e, RendererComponent& renderer, Transform& transform) {
					// If the shader has changed, set up it's uniforms
					if (current != renderer.Material->Shader) {
						current = renderer.Material->Shader;
						current->Bind();
						BackendHandler::SetupShaderForFrame(current, view, projection);
					}
					// If the material has changed, apply it
					if (currentMat != renderer.Material) {
						currentMat = renderer.Material;
						currentMat->Apply();
					}

					shadowBuffer->BindDepthAsTexture(30);
					// Render the mesh
					BackendHandler::RenderVAO(renderer.Material->Shader, renderer.Mesh, viewProjection, transform, lightSpaceViewProj);
				});

				#pragma region Draw Physics Objects

				if (drawPhysics)
				{
					mydebugDrawer.SetMatrices(physicsShader, view, projection);
					dynamicsWorld->debugDrawWorld();
				}

				#pragma endregion
				

				#pragma region Rendering Animated Models

				
				//Animated Models
				if (moveWizard1 == false)
					Wizard.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				else
					Wizard.get<MorphRenderer>().nextFrame(time.DeltaTime, 1);
				shadowBuffer->BindDepthAsTexture(30);
				Wizard.get<MorphRenderer>().render(morphShader, viewProjection, Wizard.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				Phantom.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				shadowBuffer->BindDepthAsTexture(30);
				Phantom.get<MorphRenderer>().render(morphShader, viewProjection, Phantom.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				/*Portal.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				shadowBuffer->BindDepthAsTexture(30);
				Portal.get<MorphRenderer>().render(morphShader, viewProjection, Portal.get<Transform>(), view, viewProjection, lightSpaceViewProj);*/

				Phantom2.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				shadowBuffer->BindDepthAsTexture(30);
				Phantom2.get<MorphRenderer>().render(morphShader, viewProjection, Phantom2.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				Coin.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				shadowBuffer->BindDepthAsTexture(30);
				Coin.get<MorphRenderer>().render(morphShader, viewProjection, Coin.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				#pragma endregion
				

				#pragma region Render Player and Control Player

				//Update Animation
				if (glfwGetKey(BackendHandler::window, GLFW_KEY_W) == GLFW_PRESS && !playerAirborne ||
					glfwGetKey(BackendHandler::window, GLFW_KEY_A) == GLFW_PRESS && !playerAirborne ||
					glfwGetKey(BackendHandler::window, GLFW_KEY_S) == GLFW_PRESS && !playerAirborne ||
					glfwGetKey(BackendHandler::window, GLFW_KEY_D) == GLFW_PRESS && !playerAirborne)
				{
					player.get<MorphRenderer>().nextFrame(time.DeltaTime, 1); //Walk
				}
				else if (playerAirborne)
				{
					player.get<MorphRenderer>().nextFrame(time.DeltaTime, 2); //Jump
				}

				else
				{
					if (RenderGroupBool != 2)
						player.get<MorphRenderer>().nextFrame(time.DeltaTime, 0); //Idle
					else if (RenderGroupBool == 2)
						player.get<MorphRenderer>().nextFrame(time.DeltaTime, 3); //Shiver
				}
				shadowBuffer->BindDepthAsTexture(30);
				player.get<MorphRenderer>().render(morphShader, viewProjection, player.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				if(!playerControlLock)
					PlayerInput(player, time.DeltaTime, speed, playerBody);

				#pragma endregion


				#pragma region Scene Sounds

				//Background Music
				BG.Play();
				Level1Music == true;


				//Player Walking
				if (glfwGetKey(BackendHandler::window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(BackendHandler::window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(BackendHandler::window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(BackendHandler::window, GLFW_KEY_D) == GLFW_PRESS)
				{
					footsteps.Play();
				}
				else
				{
					footsteps.StopImmediately();
				}

				//Player Death
				if (PlayerHealth <= 0)
				{
					BG.StopImmediately();
					footsteps.StopImmediately();
				}

				//Player Walking on Bridge
				if (player.get<Transform>().GetLocalPosition().x < -15.0f && player.get<Transform>().GetLocalPosition().x > -18.0f)
					footsteps.SetParameter("isOnBridge", true);
				else
					footsteps.SetParameter("isOnBridge", false);

				//Birds Chirping
				Timer += time.DeltaTime;

				if (Timer > 10.0f && Timer < 20.0f)
					loon.Play();

				if (Timer >= 20.0)
					Timer = 0.0f;


				#pragma endregion

				
				#pragma region Updating Render Transforms with Physics Bodies

				LinkBody(Wizard, wizardBody);

				LinkBody(island1, island1Body);

				LinkBody(island2, island2Body);

				LinkBody(Bridge, bridgeBody);

				//LinkBody(Coin, coinBody);

				#pragma endregion

				CheckCoinCollision(player, Coin, 0.8f, 0.8f, 0.8f, 0.8f);


				/*btCollisionObject* playerObj = new btCollisionObject();
				playerObj->setCollisionShape(playerShape);

				btCollisionAlgorithm* pAlgorithm = dynamicsWorld->getDispatcher()->findAlgorithm(playerBody->upcast(btCollisionObject), coinBody);
				btManifoldResult oManifoldResult(pBulletObj1, pBulletObj2);
				pAlgorithm->processCollision(pBulletObj1, pBulletObj2, pBtWorld->getDispatchInfo(), &oManifoldResult);
				btPersistentManifold* pManifold = oManifoldResult.getPersistentManifold();*/


				#pragma region Rendering Sprites
				
				if (!playerControlLock)
				{
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					//spriteShader->Bind();
					BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
					heartMat->Apply();
					BackendHandler::RenderVAO(spriteShader, heartVao, orthoViewProjection, heart1Obj.get<Transform>());

					BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
					heartMat->Apply();
					BackendHandler::RenderVAO(spriteShader, heartVao, orthoViewProjection, heart2Obj.get<Transform>());

					BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
					heartMat->Apply();
					BackendHandler::RenderVAO(spriteShader, heartVao, orthoViewProjection, heart3Obj.get<Transform>());
				}

				if (startPanCamera)
				{
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					ShaderMaterial::sptr currentDialogueMat = nullptr;
					BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
					currentDialogueMat = dialogueL1D1.get<Sprite>().Material;
					currentDialogueMat->Apply();
					BackendHandler::RenderVAO(spriteShader, dialogueLevel1VAO, orthoViewProjection, dialogueL1D1.get<Transform>());
				}

				if (showInteraction)
				{
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					ShaderMaterial::sptr currentInteractionMat = nullptr;
					BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
					currentInteractionMat = interactionObj.get<Sprite>().Material;
					currentInteractionMat->Apply();
					BackendHandler::RenderVAO(spriteShader, interactionVAO, orthoViewProjection, interactionObj.get<Transform>());
				}
				
				#pragma endregion

				shadowBuffer->UnbindTexture(30);
				basicEffect->UnbindBuffer();

				/*if (!postEffectInit)
				{

					effects[activeEffect]->ApplyEffect(basicEffect0);

					effects[activeEffect]->DrawToScreen();

					postEffectInit = true;
				}
				else
				{
					effects[activeEffect]->ApplyEffect(basicEffect);

					effects[activeEffect]->DrawToScreen();
				}*/

				effects[activeEffect]->ApplyEffect(basicEffect);

				effects[activeEffect]->DrawToScreen();

			}

			#pragma endregion


			#pragma region Render Level 2 (Scene 2)

			if (RenderGroupBool == 2)
			{
				// Grab out camera info from the camera object
				Transform& camTransform = cameraObject2.get<Transform>();
				glm::mat4 view = glm::inverse(camTransform.LocalTransform());
				glm::mat4 projection = cameraObject2.get<Camera>().GetProjection();
				glm::mat4 viewProjection = projection * view;

				//Set up light space matrix (Increase ortho values to expand size for scene)
				glm::mat4 lightProjectionMatrix = glm::ortho(-120.0f, 120.0f, -120.0f, 120.0f, -64.0f, 64.0f);
				//glm::mat4 lightProjectionMatrix = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -30.0f, 30.0f);
				glm::mat4 lightViewMatrix = glm::lookAt(glm::vec3(-theSun._lightDirection), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f));
				glm::mat4 lightSpaceViewProj = lightProjectionMatrix * lightViewMatrix;

				// Grab out camera info from the camera object
				Transform& orthoCamTransform = orthoCameraObject2.get<Transform>();
				glm::mat4 orthoView = glm::inverse(orthoCamTransform.LocalTransform());
				glm::mat4 orthoProjection = orthoCameraObject2.get<Camera>().GetProjection();
				glm::mat4 orthoViewProjection = orthoProjection * orthoView;

				if (!scene2ShaderInit)
				{
					float scene2AmbientPow = 0.3f;
					theSun._ambientPow = scene2AmbientPow;
					directionalLightBuffer.SendData(reinterpret_cast<void*>(&theSun), sizeof(DirectionalLight));
					
					activeEffect = 2;

					scene2ShaderInit = true;
				}

				if (!destroyedScene1Objects)
				{
					//Removing the last scene's physics bodies
					dynamicsWorld->removeRigidBody(island1Body);
					dynamicsWorld->removeRigidBody(island2Body);
					dynamicsWorld->removeRigidBody(bridgeBody);
					destroyedScene1Objects = true;

					//Adding the current scene's physics bodies
					dynamicsWorld->addRigidBody(taigaIsland1Body, 1, 1);
					dynamicsWorld->addRigidBody(taigaIsland2Body, 1, 1);
					dynamicsWorld->addRigidBody(taigaIsland3Body, 1, 1);
					dynamicsWorld->addRigidBody(taigaIsland4Body, 1, 1);
					dynamicsWorld->addRigidBody(bridge2Body, 1, 1);
					dynamicsWorld->addRigidBody(bridge3Body, 1, 1);
					dynamicsWorld->addRigidBody(bridge4Body, 1, 1);
					dynamicsWorld->addRigidBody(SnowmanA1Body, 1, 1);
					dynamicsWorld->addRigidBody(SnowmanA2Body, 1, 1);
					dynamicsWorld->addRigidBody(SnowmanA3Body, 1, 1);
					dynamicsWorld->addRigidBody(SnowmanA4Body, 1, 1);
					dynamicsWorld->addRigidBody(SnowmanA5Body, 1, 1);
					dynamicsWorld->addRigidBody(SnowmanA6Body, 1, 1);
					dynamicsWorld->addRigidBody(SnowmanA7Body, 1, 1);
					dynamicsWorld->addRigidBody(SnowmanA8Body, 1, 1);
					dynamicsWorld->addRigidBody(SnowmanA9Body, 1, 1);
					dynamicsWorld->addRigidBody(parkingMeterBody, 1, 1);
					dynamicsWorld->addRigidBody(parkingMeterPoleBody, 1, 1);
					dynamicsWorld->addRigidBody(sketchyGuyBody, 1, 1);
					dynamicsWorld->addRigidBody(wallButtonBody, 1, 1);
					//dynamicsWorld->addRigidBody(Coin2Body, 1, 1);
					//dynamicsWorld->addRigidBody(SnowmanA10Body, 1, 1);
					//dynamicsWorld->addRigidBody(SnowmanA11Body, 1, 1);
					//dynamicsWorld->addRigidBody(SnowmanA12Body, 1, 1);

					

				}

				// Sort the renderers by shader and material, we will go for a minimizing context switches approach here,
				// but you could for instance sort front to back to optimize for fill rate if you have intensive fragment shaders
				renderGroup2.sort<RendererComponent>([](const RendererComponent& l, const RendererComponent& r) {
					// Sort by render layer first, higher numbers get drawn last
					if (l.Material->RenderLayer < r.Material->RenderLayer) return true;
					if (l.Material->RenderLayer > r.Material->RenderLayer) return false;

					// Sort by shader pointer next (so materials using the same shader run sequentially where possible)
					if (l.Material->Shader < r.Material->Shader) return true;
					if (l.Material->Shader > r.Material->Shader) return false;

					// Sort by material pointer last (so we can minimize switching between materials)
					if (l.Material < r.Material) return true;
					if (l.Material > r.Material) return false;

					return false;
				});

				// Start by assuming no shader or material is applied
				Shader::sptr current = nullptr;
				ShaderMaterial::sptr currentMat = nullptr;

				glViewport(0, 0, shadowWidth, shadowHeight);
				shadowBuffer->Bind();

				//renderGroup.each([&](entt::entity e, RendererComponent& renderer, Transform& transform)

				renderGroup2.each([&](entt::entity e, RendererComponent& renderer, Transform& transform) {
					// Render the mesh
					if (renderer.CastShadows)
					{
						BackendHandler::RenderVAO(simpleDepthShader, renderer.Mesh, viewProjection, transform, lightSpaceViewProj);
					}
				});

				player.get<MorphRenderer>().render(simpleDepthShader, viewProjection, player.get<Transform>(), view, viewProjection, lightSpaceViewProj);
				Wizard.get<MorphRenderer>().render(simpleDepthShader, viewProjection, Wizard.get<Transform>(), view, viewProjection, lightSpaceViewProj);
				parkingMeter.get<MorphRenderer>().render(simpleDepthShader, viewProjection, parkingMeter.get<Transform>(), view, viewProjection, lightSpaceViewProj);
				parkingMeterPole.get<MorphRenderer>().render(simpleDepthShader, viewProjection, parkingMeterPole.get<Transform>(), view, viewProjection, lightSpaceViewProj);
				sketchyGuy.get<MorphRenderer>().render(simpleDepthShader, viewProjection, sketchyGuy.get<Transform>(), view, viewProjection, lightSpaceViewProj);
				wallButton.get<MorphRenderer>().render(simpleDepthShader, viewProjection, wallButton.get<Transform>(), view, viewProjection, lightSpaceViewProj);
				sketchyGuy2.get<MorphRenderer>().render(simpleDepthShader, viewProjection, sketchyGuy2.get<Transform>(), view, viewProjection, lightSpaceViewProj);
				Coin2.get<MorphRenderer>().render(simpleDepthShader, viewProjection, Coin2.get<Transform>(), view, viewProjection, lightSpaceViewProj);


				/*PhantomLevel2.get<MorphRenderer>().render(simpleDepthShader, viewProjection, PhantomLevel2.get<Transform>(), view, viewProjection, lightSpaceViewProj);
				Phantom2Level2.get<MorphRenderer>().render(simpleDepthShader, viewProjection, Phantom2Level2.get<Transform>(), view, viewProjection, lightSpaceViewProj);*/

				shadowBuffer->Unbind();


				glfwGetWindowSize(BackendHandler::window, &width, &height);

				glViewport(0, 0, width, height);
				basicEffect->BindBuffer(0);

				// Iterate over the render group components and draw them
				renderGroup2.each([&](entt::entity e, RendererComponent& renderer, Transform& transform) {
					// If the shader has changed, set up it's uniforms
					if (current != renderer.Material->Shader) {
						current = renderer.Material->Shader;
						current->Bind();
						BackendHandler::SetupShaderForFrame(current, view, projection);
					}
					// If the material has changed, apply it
					if (currentMat != renderer.Material) {
						currentMat = renderer.Material;
						currentMat->Apply();
					}

					shadowBuffer->BindDepthAsTexture(30);
					// Render the mesh
					BackendHandler::RenderVAO(renderer.Material->Shader, renderer.Mesh, viewProjection, transform, lightSpaceViewProj);
				});

				#pragma region Draw Physics Objects

				if (drawPhysics)
				{
					mydebugDrawer.SetMatrices(physicsShader, view, projection);
					dynamicsWorld->debugDrawWorld();
				}

				#pragma endregion


				#pragma region Render Phantoms Level 2
				/*Phantom2Level2.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				Phantom2Level2.get<MorphRenderer>().render(morphShader, viewProjection, Phantom2Level2.get<Transform>(), view, viewProjection, lightSpaceViewProj);
				PhantomLevel2.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				PhantomLevel2.get<MorphRenderer>().render(morphShader, viewProjection, PhantomLevel2.get<Transform>(), view, viewProjection, lightSpaceViewProj);*/
				#pragma endregion


				//CheckPhantomCollision(player, playerBody, playerTransform, PhantomLevel2, 0.8f, 0.8f, 0.8f, 0.8f);
				//CheckPhantomCollision(player, playerBody, playerTransform, Phantom2Level2, 0.8f, 0.8f, 0.8f, 0.8f);

				
				
				#pragma region Render Player

				//Update Animation
				if (glfwGetKey(BackendHandler::window, GLFW_KEY_W) == GLFW_PRESS && !playerAirborne ||
					glfwGetKey(BackendHandler::window, GLFW_KEY_A) == GLFW_PRESS && !playerAirborne ||
					glfwGetKey(BackendHandler::window, GLFW_KEY_S) == GLFW_PRESS && !playerAirborne ||
					glfwGetKey(BackendHandler::window, GLFW_KEY_D) == GLFW_PRESS && !playerAirborne)
				{
					player.get<MorphRenderer>().nextFrame(time.DeltaTime, 1); //Walk
				}
				else if (playerAirborne)
				{
					player.get<MorphRenderer>().nextFrame(time.DeltaTime, 2); //Jump
				}
				else
				{
					player.get<MorphRenderer>().nextFrame(time.DeltaTime, 3); //Shiver
				}
				shadowBuffer->BindDepthAsTexture(30);
				player.get<MorphRenderer>().render(morphShader, viewProjection, player.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				if (!playerControlLock)
					PlayerInput(player, time.DeltaTime, speed, playerBody);

				#pragma endregion


				#pragma region Rendering Animated Models


				Wizard.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				shadowBuffer->BindDepthAsTexture(30);
				Wizard.get<MorphRenderer>().render(morphShader, viewProjection, Wizard.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				parkingMeter.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				shadowBuffer->BindDepthAsTexture(30);
				parkingMeter.get<MorphRenderer>().render(morphShader, viewProjection, parkingMeter.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				//Portal2.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				////shadowBuffer->BindDepthAsTexture(30);
				//Portal2.get<MorphRenderer>().render(morphShader, viewProjection, Portal2.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				parkingMeterPole.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				shadowBuffer->BindDepthAsTexture(30);
				parkingMeterPole.get<MorphRenderer>().render(morphShader, viewProjection, parkingMeterPole.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				sketchyGuy.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				shadowBuffer->BindDepthAsTexture(30);
				sketchyGuy.get<MorphRenderer>().render(morphShader, viewProjection, sketchyGuy.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				sketchyGuy2.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				shadowBuffer->BindDepthAsTexture(30);
				sketchyGuy2.get<MorphRenderer>().render(morphShader, viewProjection, sketchyGuy2.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				wallButton.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				shadowBuffer->BindDepthAsTexture(30);
				wallButton.get<MorphRenderer>().render(morphShader, viewProjection, wallButton.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				Coin2.get<MorphRenderer>().nextFrame(time.DeltaTime, 0);
				shadowBuffer->BindDepthAsTexture(30);
				Coin2.get<MorphRenderer>().render(morphShader, viewProjection, Coin2.get<Transform>(), view, viewProjection, lightSpaceViewProj);


				#pragma endregion

				CheckCoinCollision(player, Coin2, 0.8f, 0.8f, 0.8f, 0.8f);


				#pragma region Rendering Sprites

				if (!playerControlLock)
				{
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					//spriteShader->Bind();
					BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
					heartMat->Apply();
					BackendHandler::RenderVAO(spriteShader, heartVao2, orthoViewProjection, heart1Obj2.get<Transform>());

					BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
					heartMat->Apply();
					BackendHandler::RenderVAO(spriteShader, heartVao2, orthoViewProjection, heart2Obj2.get<Transform>());

					BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
					heartMat->Apply();
					BackendHandler::RenderVAO(spriteShader, heartVao2, orthoViewProjection, heart3Obj2.get<Transform>());
				}

				if (startPanCamera)
				{
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					ShaderMaterial::sptr currentDialogueMat = nullptr;
					BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
					currentDialogueMat = dialogueL2.get<Sprite>().Material;
					currentDialogueMat->Apply();
					BackendHandler::RenderVAO(spriteShader, dialogueLevel2VAO, orthoViewProjection, dialogueL2.get<Transform>());
				}

				if (showInteraction)
				{
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					ShaderMaterial::sptr currentInteractionMat = nullptr;
					BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
					currentInteractionMat = interactionObj2.get<Sprite>().Material;
					currentInteractionMat->Apply();
					BackendHandler::RenderVAO(spriteShader, interactionVAO2, orthoViewProjection, interactionObj2.get<Transform>());
				}

				#pragma endregion


				#pragma region Updating Render Transforms with Physics Bodies

				LinkBody(Wizard, wizardBody, 0.0f, 0.0f, -2.3f);

				LinkBody(TaigaGround, taigaIsland1Body, 0.0f, 0.0f, -7.0f);
				LinkBody(TaigaGround2, taigaIsland2Body, 0.0f, 0.0f, -7.0f);
				LinkBody(TaigaGround3, taigaIsland3Body, 0.0f, 0.0f, -7.0f);
				LinkBody(TaigaGround4, taigaIsland4Body, 0.0f, 0.0f, -7.0f);

				LinkBody(Bridge2, bridge2Body);
				LinkBody(Bridge3, bridge3Body);
				LinkBody(Bridge4, bridge4Body);

				LinkBody(SnowmanA1, SnowmanA1Body);
				LinkBody(SnowmanA2, SnowmanA2Body);
				LinkBody(SnowmanA3, SnowmanA3Body);
				LinkBody(SnowmanA4, SnowmanA4Body);
				LinkBody(SnowmanA5, SnowmanA5Body);
				LinkBody(SnowmanA6, SnowmanA6Body);
				LinkBody(SnowmanA7, SnowmanA7Body);
				LinkBody(SnowmanA8, SnowmanA8Body);
				LinkBody(SnowmanA9, SnowmanA9Body);

				LinkBody(parkingMeter, parkingMeterBody, 7.6f, 3.7f, -2.1f);
				LinkBody(parkingMeterPole, parkingMeterPoleBody, 0.0f, 2.8f, -2.1f);
				LinkBody(wallButton, wallButtonBody, 0.0f, 0.0f, -2.5f);

				LinkBody(sketchyGuy, sketchyGuyBody);
				LinkBody(sketchyGuy2, sketchyGuy2Body);
				//LinkBody(Coin2, Coin2Body);
				//LinkBody(SnowmanA10, SnowmanA10Body);
				//LinkBody(SnowmanA11, SnowmanA11Body);
				//LinkBody(SnowmanA12, SnowmanA12Body);



				//LinkBody(TaigaGround2, island2Body, 0.0f, 0.0f, -9.0f);

				//LinkBody(Bridge2, bridgeBody);

				#pragma endregion


				#pragma region Scene Sounds

				//Player Walking
				if (glfwGetKey(BackendHandler::window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(BackendHandler::window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(BackendHandler::window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(BackendHandler::window, GLFW_KEY_D) == GLFW_PRESS)
				{
					footsteps.Play();
				}
				else
				{
					footsteps.StopImmediately();
				}

				//Player Death
				if (PlayerHealth <= 0)
				{
					Islandmusic2.StopImmediately();
					footsteps.StopImmediately();
				}

				#pragma endregion

				shadowBuffer->UnbindTexture(30);
				basicEffect->UnbindBuffer();

				effects[activeEffect]->ApplyEffect(basicEffect);

				effects[4]->ApplyEffect(effects[activeEffect]);

				effects[activeEffect]->DrawToScreen();
			}

			#pragma endregion

			
			#pragma region Render Level 3 (Scene 4)

			if (RenderGroupBool == 4)
			{
				// Grab out camera info from the camera object
				Transform& camTransform = cameraObject4.get<Transform>();
				glm::mat4 view = glm::inverse(camTransform.LocalTransform());
				glm::mat4 projection = cameraObject4.get<Camera>().GetProjection();
				glm::mat4 viewProjection = projection * view;

				//Set up light space matrix (Increase ortho values to expand size for scene)
				glm::mat4 lightProjectionMatrix = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -30.0f, 30.0f);
				glm::mat4 lightViewMatrix = glm::lookAt(glm::vec3(-theSun._lightDirection), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f));
				glm::mat4 lightSpaceViewProj = lightProjectionMatrix * lightViewMatrix;

				// Grab out camera info from the camera object
				Transform& orthoCamTransform = orthoCameraObject4.get<Transform>();
				glm::mat4 orthoView = glm::inverse(orthoCamTransform.LocalTransform());
				glm::mat4 orthoProjection = orthoCameraObject4.get<Camera>().GetProjection();
				glm::mat4 orthoViewProjection = orthoProjection * orthoView;

				if (initScene4)
				{
					activeEffect = 1;

					PixelateEffect* temp = (PixelateEffect*)scene3Effects[6];
					float intensity = temp->GetIntensity();

					intensity = 8000.0f;

					temp->SetIntensity(intensity);

					initScene4 = false;
				}



				if (!destroyedScene2Objects)
				{
					//Removing the last scene's physics bodies
					dynamicsWorld->removeRigidBody(taigaIsland1Body);
					dynamicsWorld->removeRigidBody(taigaIsland2Body);
					dynamicsWorld->removeRigidBody(bridge2Body);
					dynamicsWorld->removeRigidBody(bridge3Body);
					dynamicsWorld->removeRigidBody(bridge4Body);
					dynamicsWorld->removeRigidBody(SnowmanA1Body);
					dynamicsWorld->removeRigidBody(SnowmanA2Body);
					dynamicsWorld->removeRigidBody(SnowmanA3Body);
					dynamicsWorld->removeRigidBody(SnowmanA4Body);
					dynamicsWorld->removeRigidBody(SnowmanA5Body);
					dynamicsWorld->removeRigidBody(SnowmanA6Body);
					dynamicsWorld->removeRigidBody(SnowmanA7Body);
					dynamicsWorld->removeRigidBody(SnowmanA8Body);
					dynamicsWorld->removeRigidBody(SnowmanA9Body);
					dynamicsWorld->removeRigidBody(parkingMeterBody);
					//dynamicsWorld->removeRigidBody(SnowmanA10Body);
					//dynamicsWorld->removeRigidBody(SnowmanA11Body);
					//dynamicsWorld->removeRigidBody(SnowmanA12Body);

					destroyedScene2Objects = true;

					//Adding the current scene's physics bodies
					dynamicsWorld->addRigidBody(Fireisland1Body, 1, 1);
					//dynamicsWorld->addRigidBody(Fireisland2Body, 1, 1);
					dynamicsWorld->addRigidBody(FirePlatform1Body, -1, -1);
					//FirePlatform1Body->setGravity(btVector3(0, 0, 0));
					FirePlatform1Body->setLinearFactor(btVector3(0, 0, 0));
					FirePlatform1Body->setAngularFactor(btVector3(0, 0, 0));
					//FirePlatform1Body->isKinematicObject();
					dynamicsWorld->addRigidBody(FirePlatform2Body, -1, -1);
					//FirePlatform2Body->setGravity(btVector3(0, 0, 0));
					FirePlatform2Body->setLinearFactor(btVector3(0, 0, 0));
					FirePlatform2Body->setAngularFactor(btVector3(0, 0, 0));

					dynamicsWorld->addRigidBody(FirePlatform3Body, -1, -1);
					FirePlatform3Body->setLinearFactor(btVector3(0, 0, 0));
					FirePlatform3Body->setAngularFactor(btVector3(0, 0, 0));

					dynamicsWorld->addRigidBody(FirePlatform4Body, -1, -1);
					FirePlatform4Body->setLinearFactor(btVector3(0, 0, 0));
					FirePlatform4Body->setAngularFactor(btVector3(0, 0, 0));

					dynamicsWorld->addRigidBody(FirePlatform5Body, -1, -1);
					FirePlatform5Body->setLinearFactor(btVector3(0, 0, 0));
					FirePlatform5Body->setAngularFactor(btVector3(0, 0, 0));

					dynamicsWorld->addRigidBody(FirePlatform6Body, -1, -1);
					FirePlatform6Body->setLinearFactor(btVector3(0, 0, 0));
					FirePlatform6Body->setAngularFactor(btVector3(0, 0, 0));
					//FirePlatform2Body->isKinematicObject();

					//dynamicsWorld->addCollisionObject(firePlatform1Obj, 41, 41);
				}

				// Sort the renderers by shader and material, we will go for a minimizing context switches approach here,
				// but you could for instance sort front to back to optimize for fill rate if you have intensive fragment shaders
				renderGroup4.sort<RendererComponent>([](const RendererComponent& l, const RendererComponent& r) {
					// Sort by render layer first, higher numbers get drawn last
					if (l.Material->RenderLayer < r.Material->RenderLayer) return true;
					if (l.Material->RenderLayer > r.Material->RenderLayer) return false;

					// Sort by shader pointer next (so materials using the same shader run sequentially where possible)
					if (l.Material->Shader < r.Material->Shader) return true;
					if (l.Material->Shader > r.Material->Shader) return false;

					// Sort by material pointer last (so we can minimize switching between materials)
					if (l.Material < r.Material) return true;
					if (l.Material > r.Material) return false;

					return false;
					});


				// Start by assuming no shader or material is applied
				Shader::sptr current = nullptr;
				ShaderMaterial::sptr currentMat = nullptr;

				glViewport(0, 0, shadowWidth, shadowHeight);
				shadowBuffer->Bind();

				//renderGroup.each([&](entt::entity e, RendererComponent& renderer, Transform& transform)

				renderGroup4.each([&](entt::entity e, RendererComponent& renderer, Transform& transform) {
				// Render the mesh
				if (renderer.CastShadows)
				{
					BackendHandler::RenderVAO(simpleDepthShader, renderer.Mesh, viewProjection, transform, lightSpaceViewProj);
				}
				});

				player.get<MorphRenderer>().render(simpleDepthShader, viewProjection, player.get<Transform>(), view, viewProjection, lightSpaceViewProj);

				shadowBuffer->Unbind();


				glfwGetWindowSize(BackendHandler::window, &width, &height);

				glViewport(0, 0, width, height);
				basicEffect3->BindBuffer(0);

				if (width > 900)
				{
					lastMenuObj.get<Transform>().SetLocalScale(1.14f, 1.0f, 1.5f);
				}
				else
				{
					lastMenuObj.get<Transform>().SetLocalScale(0.6f, 0.6f, 0.6f);
				}

				// Iterate over the render group components and draw them
				renderGroup4.each([&](entt::entity e, RendererComponent& renderer, Transform& transform) {
					// If the shader has changed, set up it's uniforms
					if (current != renderer.Material->Shader) {
						current = renderer.Material->Shader;
						current->Bind();
						BackendHandler::SetupShaderForFrame(current, view, projection);
					}
					// If the material has changed, apply it
					if (currentMat != renderer.Material) {
						currentMat = renderer.Material;
						currentMat->Apply();
					}

					shadowBuffer->BindDepthAsTexture(30);
					// Render the mesh
					BackendHandler::RenderVAO(renderer.Material->Shader, renderer.Mesh, viewProjection, transform);
				});

				#pragma region Draw Physics Objects

				if (drawPhysics)
				{
					mydebugDrawer.SetMatrices(physicsShader, view, projection);
					dynamicsWorld->debugDrawWorld();
				}

				#pragma endregion


				#pragma region Render Player

				//Update Animation
				if (glfwGetKey(BackendHandler::window, GLFW_KEY_W) == GLFW_PRESS && !playerAirborne ||
					glfwGetKey(BackendHandler::window, GLFW_KEY_A) == GLFW_PRESS && !playerAirborne ||
					glfwGetKey(BackendHandler::window, GLFW_KEY_S) == GLFW_PRESS && !playerAirborne ||
					glfwGetKey(BackendHandler::window, GLFW_KEY_D) == GLFW_PRESS && !playerAirborne)
				{
					player.get<MorphRenderer>().nextFrame(time.DeltaTime, 1); //Walk
				}
				else if (playerAirborne)
				{
					player.get<MorphRenderer>().nextFrame(time.DeltaTime, 2); //Jump
				}
				else
				{
					if (RenderGroupBool != 2)
						player.get<MorphRenderer>().nextFrame(time.DeltaTime, 0); //Idle
					else if (RenderGroupBool == 2)
						player.get<MorphRenderer>().nextFrame(time.DeltaTime, 3); //Idle
				}
				shadowBuffer->BindDepthAsTexture(30);
				player.get<MorphRenderer>().render(morphShader, viewProjection, player.get<Transform>(), view, viewProjection);

				if (!playerControlLock)
					PlayerInput(player, time.DeltaTime, speed, playerBody);

				#pragma endregion


				#pragma region Rendering Sprites

				if (!playerControlLock)
				{
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					//spriteShader->Bind();
					BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
					heartMat->Apply();
					BackendHandler::RenderVAO(spriteShader, heartVao3, orthoViewProjection, heart1Obj3.get<Transform>());

					BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
					heartMat->Apply();
					BackendHandler::RenderVAO(spriteShader, heartVao3, orthoViewProjection, heart2Obj3.get<Transform>());

					BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
					heartMat->Apply();
					BackendHandler::RenderVAO(spriteShader, heartVao3, orthoViewProjection, heart3Obj3.get<Transform>());
				}
				else
				{
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					ShaderMaterial::sptr currentMenuMat = nullptr;
					//spriteShader->Bind();
					BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
					currentMenuMat = lastMenuObj.get<Sprite>().Material;
					currentMenuMat->Apply();
					BackendHandler::RenderVAO(spriteShader, lastmenuVAO, orthoViewProjection, lastMenuObj.get<Transform>());
				}



				#pragma endregion


				#pragma region Updating Render Transforms with Physics Bodies

				//Example of how to link bodies is below. Have to create the physics bodies in the scene first
				//NOTE: If physics objects are incorrectly scaled or seem off with the rendered body then you will have to
				//      adjust the physics shape the object has
				// This is just a note in case you wanted to see how physics bodies are linked

				//LinkBody(Bridge2, bridgeBody);
				LinkBody(Fireisland1, Fireisland1Body);
				//LinkBody(Fireisland2, Fireisland2Body, 2.0f, 0.0f, 0.0f);
				LinkBody(FirePlatform, FirePlatform1Body, 0.0f, 0.0f, -1.5f);
				LinkBody(FirePlatform2, FirePlatform2Body, 0.0f, 0.0f, -1.5f);
				LinkBody(FirePlatform3, FirePlatform3Body, 0.0f, 0.0f, -1.5f);
				LinkBody(FirePlatform4, FirePlatform4Body, 0.0f, 0.0f, -1.5f);
				LinkBody(FirePlatform5, FirePlatform5Body, 0.0f, 0.0f, -1.5f);
				LinkBody(FirePlatform6, FirePlatform6Body, 0.0f, 0.0f, -1.5f);

				#pragma endregion


				#pragma region Scene Sounds

				//Player Walking
				if (glfwGetKey(BackendHandler::window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(BackendHandler::window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(BackendHandler::window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(BackendHandler::window, GLFW_KEY_D) == GLFW_PRESS)
				{
					footsteps.Play();
				}
				else
				{
					footsteps.StopImmediately();
				}

				//Player Death
				if (PlayerHealth <= 0)
				{
					Islandmusic3.StopImmediately();
					footsteps.StopImmediately();
				}

				#pragma endregion

				/*std::cout << "Platform Linear Velocity X: " << playerBody->getLinearVelocity().getX() << std::endl;
				std::cout << "Platform Linear Velocity Y: " << playerBody->getLinearVelocity().getY() << std::endl;
				std::cout << "Platform Linear Velocity Z: " << playerBody->getLinearVelocity().getZ() << std::endl;*/

				shadowBuffer->UnbindTexture(30);
				basicEffect3->UnbindBuffer();

				scene3Effects[activeEffect]->ApplyEffect(basicEffect3);

				scene3Effects[6]->ApplyEffect(scene3Effects[activeEffect]);
				

				scene3Effects[activeEffect]->DrawToScreen();

			}

			#pragma endregion


			#pragma region Render Death Scene (Scene 3)

			if (RenderGroupBool == 3)
			{
				// Grab out camera info from the camera object
				Transform& camTransform = cameraObject3.get<Transform>();
				glm::mat4 view = glm::inverse(camTransform.LocalTransform());
				glm::mat4 projection = cameraObject3.get<Camera>().GetProjection();
				glm::mat4 viewProjection = projection * view;

				// Grab out camera info from the camera object
				Transform& orthoCamTransform = orthoCameraObject3.get<Transform>();
				glm::mat4 orthoView = glm::inverse(orthoCamTransform.LocalTransform());
				glm::mat4 orthoProjection = orthoCameraObject3.get<Camera>().GetProjection();
				glm::mat4 orthoViewProjection = orthoProjection * orthoView;

				// Sort the renderers by shader and material, we will go for a minimizing context switches approach here,
				// but you could for instance sort front to back to optimize for fill rate if you have intensive fragment shaders
				renderGroup3.sort<RendererComponent>([](const RendererComponent& l, const RendererComponent& r) {
					// Sort by render layer first, higher numbers get drawn last
					if (l.Material->RenderLayer < r.Material->RenderLayer) return true;
					if (l.Material->RenderLayer > r.Material->RenderLayer) return false;

					// Sort by shader pointer next (so materials using the same shader run sequentially where possible)
					if (l.Material->Shader < r.Material->Shader) return true;
					if (l.Material->Shader > r.Material->Shader) return false;

					// Sort by material pointer last (so we can minimize switching between materials)
					if (l.Material < r.Material) return true;
					if (l.Material > r.Material) return false;

					return false;
					});

				// Start by assuming no shader or material is applied
				Shader::sptr current = nullptr;
				ShaderMaterial::sptr currentMat = nullptr;

				activeEffect = 0;

				basicEffect0->BindBuffer(0);

				glfwGetWindowSize(BackendHandler::window, &width, &height);

				if (width > 900)
				{
					deathMenuObj.get<Transform>().SetLocalScale(1.14f, 1.0f, 0.6f);
				}
				else
				{
					deathMenuObj.get<Transform>().SetLocalScale(0.6f, 0.6f, 0.6f);
				}

				// Iterate over the render group components and draw them
				renderGroup3.each([&](entt::entity e, RendererComponent& renderer, Transform& transform) {
					// If the shader has changed, set up it's uniforms
					if (current != renderer.Material->Shader) {
						current = renderer.Material->Shader;
						current->Bind();
						BackendHandler::SetupShaderForFrame(current, view, projection);
					}
					// If the material has changed, apply it
					if (currentMat != renderer.Material) {
						currentMat = renderer.Material;
						currentMat->Apply();
					}
					// Render the mesh
					BackendHandler::RenderVAO(renderer.Material->Shader, renderer.Mesh, viewProjection, transform);
				});

				#pragma region Rendering Sprites

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				//spriteShader->Bind();
				ShaderMaterial::sptr currentMenuMat = nullptr;
				BackendHandler::SetupShaderForFrame(spriteShader, orthoView, orthoProjection);
				currentMenuMat = deathMenuObj.get<Sprite>().Material;
				currentMenuMat->Apply();
				BackendHandler::RenderVAO(spriteShader, menuVAO, orthoViewProjection, deathMenuObj.get<Transform>());
				glDisable(GL_BLEND);

				#pragma endregion

				basicEffect0->UnbindBuffer();

				effects[activeEffect]->ApplyEffect(basicEffect0);

				effects[activeEffect]->DrawToScreen();
				
			}

			#pragma endregion


			

			//#pragma region Update Sound

			////Game.cpp Update & Render//
			//Update(time.DeltaTime);
			//Render();

			//#pragma endregion
			

			// Draw our ImGui content
			BackendHandler::RenderImGui();

			// Update all world matrices for this frame
			Application::Instance().ActiveScene->Poll();


			glfwSwapBuffers(BackendHandler::window);
			time.LastFrame = time.CurrentFrame;
		}

		#pragma region Physics Cleanup

		//Remove the rigidbodies from the dynamics world and delete them
		for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}

		//Delete collision shapes
		/*for (int j = 0; j < collisionShapes.size(); j++)
		{
			btCollisionShape* shape = collisionShapes[j];
			collisionShapes[j] = 0;
			delete shape;
		}*/

		//Delete dynamics world
		delete dynamicsWorld;

		//Delete solver
		delete solver;

		//Delete broadphase
		delete overlappingPairCache;

		//Delete dispatcher
		delete dispatcher;

		delete collisionConfiguration;

		#pragma endregion


		#pragma region Sound Cleanup

		// Shut down game.cpp//
		Shutdown();

		#pragma endregion

		directionalLightBuffer.Unbind(0);

		// Nullify scene so that we can release references
		Application::Instance().ActiveScene = nullptr;

		for (int i = 0; i < Application::Instance().scenes.size(); i++)
		{
			Application::Instance().scenes[i] = nullptr;
		}

		//Clean up the environment generator so we can release references
		EnvironmentGenerator::CleanUpPointers();
		BackendHandler::ShutdownImGui();
	}	

	// Clean up the toolkit logger so we don't leak memory
	Logger::Uninitialize();
	return 0;
}