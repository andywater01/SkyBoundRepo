#pragma once

#include "Graphics/Post/PostEffect.h"
#include "Graphics/LUT.h"

class WarmEffect : public PostEffect
{
public:
	//Initializes framebuffer
	void Init(unsigned width, unsigned height) override;

	//Applies effect to this buffer
	void ApplyEffect(PostEffect* buffer) override;

	//Getters
	LUT3D GetCube() const;

	//Setters
	void SetCube(LUT3D cube);

private:
	LUT3D _cube;
};