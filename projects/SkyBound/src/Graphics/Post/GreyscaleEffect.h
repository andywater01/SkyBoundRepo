#pragma once

#include "Graphics/Post/PostEffect.h"

class GreyscaleEffect : public PostEffect
{
public:
	//Initializes framebuffer
	//Overrides post effect init
	void Init(unsigned width, unsigned height) override;

	//Applies the effect to this buffer
	//passes the previous framebuffer with the texture to apply as parameter
	void ApplyEffect(PostEffect* buffer) override;

	//Getters
	float GetIntensity() const;

	//Setters
	void SetIntensity(float intensity);
private:
	float _intensity = 0.0f;

};