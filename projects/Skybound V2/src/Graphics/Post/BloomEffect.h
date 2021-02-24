#pragma once

#include "Graphics/Post/PostEffect.h"
#include "Graphics/LUT.h"

class BloomEffect : public PostEffect
{
public:
	//Initializes framebuffer
	void Init(unsigned width, unsigned height) override;

	//Applies effect to this buffer
	void ApplyEffect(PostEffect* buffer) override;

	//Getters
	float GetDownscale() const;
	float GetThreshold() const;
	unsigned GetPasses() const;
	float GetPixelX() const;
	float GetPixelY() const;

	//Setters
	void SetDownscale(float downscale);
	void SetThreshold(float threshold);
	void SetPasses(unsigned passes);
	void SetPixelX(float pixelX);
	void SetPixelY(float pixelY);

private:
	float _downscale = 2.f;
	float _threshold = 0.616f;
	unsigned _passes = 8;
	glm::vec2 _pixelSize;
};