/*
Week 2 Tutorial Starter - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "NOU/CMeshRenderer.h"
#include "NOU/GLTFLoader.h"

#include "Logging.h"

#include <memory>

using namespace nou;

//TODO: Write templated LERP function.
template<typename T>
T LERP(const T& p0, const T& p1, float t)
{
	return (1.0f - t) * p0 + t * p1;
}

int main() 
{
	App::Init("Week 1 Tutorial - LERP", 800, 800);
	App::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//Load in some shaders.
	//Smart pointers will automatically deallocate memory when they go out of scope.
	auto v_texLit = std::make_unique<Shader>("shaders/texturedlit.vert", GL_VERTEX_SHADER);
	auto f_texLit = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);

	auto prog_texLit = ShaderProgram({ v_texLit.get(), f_texLit.get() });

	//TODO: Load in duck model and texture.
	Mesh duckMesh;
	GLTF::LoadMesh("duck/Duck.gltf", duckMesh);
	Texture2D duckTex = Texture2D("duck/DuckCM.png");

	Material duckMat(prog_texLit);
	duckMat.AddTexture("albedo", duckTex);

	//Load in sword model and texture.
	Mesh swordMesh;
	GLTF::LoadMesh("sword/Sword.gltf", swordMesh);
	Texture2D swordTex = Texture2D("sword/SwordCM.png");

	Material swordMat(prog_texLit);
	swordMat.AddTexture("albedo", swordTex);

	//TODO: Set up camera entity and transform.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Perspective(60.0f, 1.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 4.0f);

	//TODO: Create our duck entity.
	Entity duckEntity = Entity::Create();
	duckEntity.Add<CMeshRenderer>(duckEntity, duckMesh, duckMat);
	duckEntity.transform.m_scale = glm::vec3(0.0005f, 0.0005f, 0.0005f);
	duckEntity.transform.m_pos = glm::vec3(-1.5f, -2.0f, 0.0f);
	duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Create sword entity.
	Entity swordEntity = Entity::Create();
	swordEntity.Add<CMeshRenderer>(swordEntity, swordMesh, swordMat);
	swordEntity.transform.m_scale = glm::vec3(0.001f, 0.001f, 0.001f);
	swordEntity.transform.m_pos = glm::vec3(1.2f, 0.5f, 0.0f);
	swordEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Variables for lerping position of duck
	glm::vec3 origPos = glm::vec3(-1.5f, -2.0f, 0.0f), newPos = glm::vec3(1.2f, -2.0f, 0.0f);
	float posTimer = 0.0f, posTimeLimit = 4.0f;
	bool duckyGoPlaces = true;

	//Variables for lerping position of sword
	glm::vec3 swordPos = glm::vec3(1.2f, 0.5f, 0.0f), newSwordPos = glm::vec3(-1.1f, 0.5f, 0.0f);
	float swordPosTimer = 0.0f, swordPosTimeLimit = 4.0f;
	bool swordSwinging = true;

	//Variables for lerping colour
	glm::vec3 origCol = glm::vec3(1.0f, 1.0f, 1.0f), newCol = glm::vec3(5.0f, 0.0f, 0.0f);
	float colTimer = 0.0f, colTimeLimit = 5.0f;
	bool duckyGoColor = true;

	//Variables for lerping scale
	glm::vec3 origScale = glm::vec3(0.0005f, 0.0005f, 0.0005f), newScale = glm::vec3(0.01f, 0.01f, 0.01f);
	float scaleTimer = 0.0f, scaleTimeLimit = 5.0f;
	bool scalingUp = true;

	//Variables for lerping rotation
	glm::quat origRot = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat newRot = glm::angleAxis(glm::radians(120.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	float rotTimer = 0.0f, rotTimeLimit = 2.0f;
	bool firstRotation = true;

	App::Tick();

	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		//TODO: LERP stuff!
		posTimer += deltaTime;
		colTimer += deltaTime;
		scaleTimer += deltaTime;
		rotTimer += deltaTime;
		swordPosTimer += deltaTime;

		//Position
		if (posTimer >= posTimeLimit)
		{
			posTimer = 0.0f;
			duckyGoPlaces = !duckyGoPlaces;
		}

		//Colour
		if (colTimer >= colTimeLimit)
		{
			colTimer = 0.0f;
			duckyGoColor = !duckyGoColor;
		}

		//Scale
		if (scaleTimer >= scaleTimeLimit)
		{
			scaleTimer = 0.0f;
			scalingUp = !scalingUp;
		}

		//Rotation
		if (rotTimer >= rotTimeLimit)
		{
			rotTimer = 0.0f;
			firstRotation = !firstRotation;
		}

		//Sword Position
		if (swordPosTimer >= swordPosTimeLimit)
		{
			swordPosTimer = 0.0f;
			swordSwinging = !swordSwinging;
		}


		//Calculate t
		float tPos = posTimer / posTimeLimit;
		float tCol = colTimer / colTimeLimit;
		float tScale = scaleTimer / scaleTimeLimit;
		float tRot = rotTimer / rotTimeLimit;
		float tSwordPos = swordPosTimer / swordPosTimeLimit;

		//Position
		if (duckyGoPlaces == true)
		{
			duckEntity.transform.m_pos = LERP(origPos, newPos, tPos);
		}
		else if (duckyGoPlaces == false)
		{
			duckEntity.transform.m_pos = LERP(newPos, origPos, tPos);
		}


		//Colour
		if (duckyGoColor == true)
		{
			duckMat.m_color = LERP(origCol, newCol, tCol);
		}
		else if (duckyGoColor == false)
		{
			duckMat.m_color = LERP(newCol, origCol, tCol);
		}


		//Scale
		if (scalingUp == true)
		{
			duckEntity.transform.m_scale = LERP(origScale, newScale, tScale);
		}
		else if (scalingUp == false)
		{
			duckEntity.transform.m_scale = LERP(newScale, origScale, tScale);
		}

		//Rotation
		if (firstRotation == true)
		{
			swordEntity.transform.m_rotation = LERP(origRot, newRot, tRot);
		}
		else if (firstRotation == false)
		{
			swordEntity.transform.m_rotation = LERP(newRot, origRot, tRot);
		}

		//SwordPosition
		if (swordSwinging == true)
		{
			swordEntity.transform.m_pos = LERP(swordPos, newSwordPos, tSwordPos);
		}
		else if (swordSwinging == false)
		{
			swordEntity.transform.m_pos = LERP(newSwordPos, swordPos, tSwordPos);
		}


		//TODO: Update camera, update duck transform, draw duck...
		camEntity.Get<CCamera>().Update();

		duckEntity.transform.RecomputeGlobal();
		duckEntity.Get<CMeshRenderer>().Draw();

		swordEntity.transform.RecomputeGlobal();
		swordEntity.Get<CMeshRenderer>().Draw();

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0; 
} 
