//------------------------------------------------------------------------
// Game.cpp
//------------------------------------------------------------------------
#include <iostream>

#include "AudioEngine.h"
#include "SharedVar.h"


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

	
	AudioEvent& music = engine.CreateEvent("music", "{f7cd671d-25bb-4e3b-a17f-8265822fa5ed}");
	AudioEvent& popSound = engine.CreateEvent("popSound", "{b2dc5970-89f7-4849-a07e-f6f6bebb4baf}");
	AudioEvent& loonSound = engine.CreateEvent("loonSound", "{b2900161-916e-40d1-942e-e1d35089ab81}");
	AudioEvent& footstepsSound = engine.CreateEvent("footstepsSound", "{f6976b06-1be7-48cb-b195-e34cba50f9db}");



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


