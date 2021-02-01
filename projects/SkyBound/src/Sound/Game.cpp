//------------------------------------------------------------------------
// Game.cpp
//------------------------------------------------------------------------

#include "Sound/AudioEngine.h"

float gameTime;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------

void Init()
{
	AudioEngine& engine = AudioEngine::Instance();
	engine.Init();
	engine.LoadBank("Master");
	engine.LoadBus("MusicBus", "{a5b53ded-d7b3-4e6b-a920-0b241ef6f268}");

	AudioEvent& music = engine.CreateEvent("music", "{b56cb9d2-1d47-4099-b80e-7d257b99a823}");

	music.Play();

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

	//Get ref to bus
	AudioBus& musicBus = engine.GetBus("MusicBus");

	AudioListener& listener = engine.GetListener();
	
	listener.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	
	// After 5 seconds go underwater
	if (gameTime > 4.0f)
	{
		// Do something
		music.SetParameter("Underwater", 1.0f);

		musicBus.SetPaused(true);
	}

	if (gameTime > 10.0f)
	{
		engine.SetGlobalParameter("Timewarp", 1.0f);

		musicBus.SetPaused(false);
	}


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