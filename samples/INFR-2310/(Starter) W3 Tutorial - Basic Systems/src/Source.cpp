/*
Week 3 Tutorial Starter - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "Sprites/CSpriteRenderer.h"
#include "Sprites/CSpriteAnimator.h"
#include "CKnightFSM.h"
#include "CSkullFSM.h"

#include "imgui.h"

#include <memory>

using namespace nou;

int main() 
{
	App::Init("Week 3 Tutorial - Sprites", 800, 800);
	App::SetClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	App::InitImgui();

	//Load in some shaders.
	auto v_sprite = std::make_unique<Shader>("shaders/texturedunlit.vert", GL_VERTEX_SHADER);
	auto f_sprite = std::make_unique<Shader>("shaders/texturedunlit.frag", GL_FRAGMENT_SHADER);

	auto prog_sprite = ShaderProgram({ v_sprite.get(), f_sprite.get() });

	//  Skull  //

	//Load in sprites.
	Texture2D skullTex = Texture2D("SkullSprite.png", true);

	Material skullMat(prog_sprite);
	skullMat.AddTexture("albedo", skullTex);


	//Load in spritesheets.
	auto skullSheet = std::make_unique<Spritesheet>(skullTex, glm::vec2(125.0f, 125.0f));
	skullSheet->AddAnimation(CSkullFSM::idleClip, 0, 5, 12.0f);
	skullSheet->AddAnimation(CSkullFSM::attackClip, 16, 21, 12.0f);
	skullSheet->AddAnimation(CSkullFSM::emoteClip, 32, 37, 12.0f);


	//Create the knight entity.
	Entity skullEntity = Entity::Create();
	skullEntity.transform.m_scale = glm::vec3(2.0f, 2.0f, 2.0f);
	skullEntity.Add<CSpriteRenderer>(skullEntity, *skullSheet, skullMat);
	auto& skullAnim = skullEntity.Add<CSpriteAnimator>(skullEntity, *skullSheet);
	skullEntity.Add<CSkullFSM>(skullEntity);

	//////////////////////////////////////////////

	//Load in sprites.
	Texture2D boomTex = Texture2D("explosion.png", true);
	Texture2D knightTex = Texture2D("knight.png", true);

	Material boomMat(prog_sprite);
	boomMat.AddTexture("albedo", boomTex);

	Material knightMat(prog_sprite);
	knightMat.AddTexture("albedo", knightTex);
	
	//TODO: Load in explosion spritesheet.
	auto boomSheet = std::make_unique<Spritesheet>(boomTex, glm::vec2(222.0f, 222.0f));
	boomSheet->AddAnimation("boom", 0, 27, 30.0f);
	boomSheet->SetDefaultFrame(27);

	//Load in knight spritesheet, add animations.
	auto knightSheet = std::make_unique<Spritesheet>(knightTex, glm::vec2(64.0f, 64.0f));
	knightSheet->AddAnimation(CKnightFSM::idleClip, 0, 4, 12.0f);
	knightSheet->AddAnimation(CKnightFSM::runClip, 5, 12, 12.0f);
	knightSheet->AddAnimation(CKnightFSM::attackClip, 19, 21, 12.0f);

	//Set up our camera.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Ortho(-400.0f, 400.0f, -400.0f, 400.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 2.0f);

	//TODO: Create explosion entity.
	Entity boomEntity = Entity::Create();
	boomEntity.Add<CSpriteRenderer>(boomEntity, *boomSheet, boomMat);
	auto& boomAnim = boomEntity.Add<CSpriteAnimator>(boomEntity, *boomSheet);

	//Create the knight entity.
	Entity knightEntity = Entity::Create();
	knightEntity.transform.m_scale = glm::vec3(2.0f, 2.0f, 2.0f);
	knightEntity.Add<CSpriteRenderer>(knightEntity, *knightSheet, knightMat);
	auto& knightAnim = knightEntity.Add<CSpriteAnimator>(knightEntity, *knightSheet);
	knightEntity.Add<CKnightFSM>(knightEntity);

	App::Tick();

	//Disabling the depth buffer.
	//(The reason we do this is so we can naively draw sprites on top
	//of each other in the order we call Draw().)
	glDisable(GL_DEPTH_TEST);

	//Disable backface culling.
	//(The reason we disable culling is to make it simpler for us to 
	//flip sprites without making OpenGL think we're drawing polygons
	//that shouldn't be visible.)
	glDisable(GL_CULL_FACE);
	
	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		//TODO: Control our knight.
		bool moving = Input::GetKey(GLFW_KEY_RIGHT) || Input::GetKey(GLFW_KEY_LEFT);
		knightEntity.Get<CKnightFSM>().SetVariable("moving", moving);


		if (moving)
		{
			bool flip = Input::GetKey(GLFW_KEY_LEFT);

			knightEntity.transform.m_scale.x = (flip) ? -2.0f : 2.0f;
			knightEntity.transform.m_pos.x += (flip) ? -100.0f * deltaTime : 100.0f * deltaTime;

		}


		//Control our skull.
		bool skullMoving = Input::GetKey(GLFW_KEY_D) || Input::GetKey(GLFW_KEY_A);
		skullEntity.Get<CSkullFSM>().SetVariable("skullMoving", skullMoving);


		if (skullMoving)
		{
			bool skullFlip = Input::GetKey(GLFW_KEY_A);

			skullEntity.transform.m_scale.x = (skullFlip) ? -2.0f : 2.0f;
			skullEntity.transform.m_pos.x += (skullFlip) ? -100.0f * deltaTime : 100.0f * deltaTime;

		}

		if (Input::GetKey(GLFW_KEY_SPACE))
		{
			skullEntity.Get<CSkullFSM>().SetTrigger("skullAttack");
		}

		if (Input::GetKey(GLFW_KEY_E))
		{
			skullEntity.Get<CSkullFSM>().SetTrigger("skullEmote");
		}



		//Updates all the entities.
		camEntity.Get<CCamera>().Update();
		//TODO: Update explosion entity.
		knightEntity.Get<CKnightFSM>().Update();
		boomEntity.Get<CSpriteAnimator>().Update(deltaTime);
		knightEntity.Get<CSpriteAnimator>().Update(deltaTime);

		skullEntity.Get<CSkullFSM>().Update();
		skullEntity.Get<CSpriteAnimator>().Update(deltaTime);
		
		//Recomputes global matrices.
		//TODO: Update explosion entity.
		boomEntity.transform.RecomputeGlobal();
		knightEntity.transform.RecomputeGlobal();
		skullEntity.transform.RecomputeGlobal();

		//Draws the sprites.
		//TODO: Draw explosion entity.
		boomEntity.Get<CSpriteRenderer>().Draw();
		knightEntity.Get<CSpriteRenderer>().Draw();
		skullEntity.Get<CSpriteRenderer>().Draw();

		//For Imgui stuff...
		App::StartImgui();

		//TODO: Create Imgui panel.
		//TODO: Create Imgui button and play explosion on press.
		static bool panelOpen = true;
		ImGui::Begin("Panels are cool", &panelOpen, ImVec2(300, 100));

		if (ImGui::Button("Boom!"))
		{
			boomEntity.Get<CSpriteAnimator>().PlayOnce("boom");
		}

		if (ImGui::Button("Knight Attack!"))
		{
			knightEntity.Get<CKnightFSM>().SetTrigger("attacking");
		}

		if (ImGui::Button("Skull Attack!"))
		{
			skullEntity.Get<CSkullFSM>().SetTrigger("skullAttack");
		}

		if (ImGui::Button("Skull Celebration!"))
		{
			skullEntity.Get<CSkullFSM>().SetTrigger("skullEmote");
		}

		ImGui::End();
		App::EndImgui();


		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0; 
} 
