#include "Graphics/Post/VignetteEffect.h"

void VignetteEffect::Init(unsigned width, unsigned height)
{
	//Load the buffers
	int index = int(_buffers.size());
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(width, height);

	//Load the shaders
	index = int(_shaders.size());
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/VignettePost_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
}

void VignetteEffect::ApplyEffect(PostEffect * buffer)
{
	BindShader(0);
	_shaders[0]->SetUniform("uInnerRadius", _innerRadius);
	_shaders[0]->SetUniform("uOuterRadius", _outerRadius);
	_shaders[0]->SetUniform("uOpacity", _opacity);

	buffer->BindColorAsTexture(0, 0, 0);

	_buffers[0]->RenderToFSQ();

	buffer->UnbindTexture(0);

	UnbindShader();
}

float VignetteEffect::GetInnerRadius() const
{
	return _innerRadius;
}

float VignetteEffect::GetOuterRadius() const
{
	return _outerRadius;
}

float VignetteEffect::GetOpacity() const
{
	return _opacity;
}

void VignetteEffect::SetInnerRadius(float innerRadius)
{
	_innerRadius = innerRadius;
}

void VignetteEffect::SetOuterRadius(float outerRadius)
{
	_outerRadius = outerRadius;
}

void VignetteEffect::SetOpacity(float opacity)
{
	_opacity = opacity;
}
