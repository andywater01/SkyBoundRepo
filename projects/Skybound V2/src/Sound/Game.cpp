//------------------------------------------------------------------------
// Game.cpp
//------------------------------------------------------------------------
#include <iostream>

#include "AudioEngine.h"


float gameTime;

class SoundObject
{
public:
	SoundObject(AudioEvent& soundRef)
		: _sound(soundRef)
	{}

	void CheckInput(bool input)
	{
		if (input)
		{
			_sound.Play();
		}
	}

	AudioEvent& GetSound()
	{
		return _sound;
	}

private:
	AudioEvent& _sound;

};

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------

void Init()
{
	AudioEngine& engine = AudioEngine::Instance();
	engine.Init();
	engine.LoadBank("Master");
	engine.LoadBus("MusicBus", "{172fce18-b855-4394-a303-1faa708a7d7b}");

	AudioEvent& menuMusic = engine.CreateEvent("menuMusic", "{98cff329-b764-4ba5-b304-52d39179b942}");
	AudioEvent& music = engine.CreateEvent("music", "{f7cd671d-25bb-4e3b-a17f-8265822fa5ed}");
	AudioEvent& music2 = engine.CreateEvent("music2", "{6f3d37f4-30e0-4173-a2d5-350f8210b26e}");
	AudioEvent& music3 = engine.CreateEvent("music3", "{375f76ec-0686-45b1-996e-56e908a17a5c}");


	AudioEvent& popSound = engine.CreateEvent("popSound", "{b2dc5970-89f7-4849-a07e-f6f6bebb4baf}");
	AudioEvent& loonSound = engine.CreateEvent("loonSound", "{b2900161-916e-40d1-942e-e1d35089ab81}");
	AudioEvent& footstepsSound = engine.CreateEvent("footstepsSound", "{f6976b06-1be7-48cb-b195-e34cba50f9db}");


	//Narration
	AudioEvent& Narration1 = engine.CreateEvent("Narration1", "{185de26a-67ae-46cf-9006-c3a82135e68c}");
	AudioEvent& Narration2 = engine.CreateEvent("Narration2", "{669cc823-0d96-4813-993e-9c75a1aebcff}");
	AudioEvent& Narration3 = engine.CreateEvent("Narration3", "{d81616ea-6cc6-4bdf-b11f-599f96594c1a}");


	//SoundObject BG(music);

	//BG.CheckInput(false);


	//music.Play();
	
 
	

}

//------------------------------------------------------------------------
// Update your game here. 
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	// Increment game time
	gameTime += deltaTime;

	AudioEngine& engine = AudioEngine::Instance();

	//Get ref to music

	AudioEvent& music = engine.GetEvent("music");
	AudioEvent& popSound = engine.GetEvent("popSound");
	AudioEvent& loon = engine.GetEvent("loonSound");
	AudioEvent& footsteps = engine.GetEvent("footstepsSound");

	AudioEvent& menu = engine.GetEvent("menuMusic");
	AudioEvent& Islandmusic2 = engine.GetEvent("music2");
	AudioEvent& Islandmusic3 = engine.GetEvent("music3");


	AudioEvent& Prelude1 = engine.GetEvent("Narration1");
	AudioEvent& Prelude2 = engine.GetEvent("Narration2");
	AudioEvent& Prelude3 = engine.GetEvent("Narration3");


	//Get ref to bus
	AudioBus& musicBus = engine.GetBus("MusicBus");

	AudioListener& listener = engine.GetListener();
	
	
	listener.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		
	
	
	// After 5 seconds go underwater
	//if (gameTime > 4.0f)
	//{
	//	// Do something
	//	music.SetParameter("Underwater", 1.0f);

	//	musicBus.SetPaused(true);
	//}

	/*if (gameTime > 10.0f)
	{
		engine.SetGlobalParameter("Timewarp", 1.0f);

		musicBus.SetPaused(false);
	}*/

	


	//std::cout << "AUDIO IS WORKING" << std::endl;

	engine.Update();
}


//------------------------------------------------------------------------
// Add your display calls here
//------------------------------------------------------------------------
void Render()
{
	// Who needs graphics when you have audio?
}


//------------------------------------------------------------------------
// Add your shutdown code here.
//------------------------------------------------------------------------
void Shutdown()
{
	AudioEngine::Instance().Shutdown();
}


