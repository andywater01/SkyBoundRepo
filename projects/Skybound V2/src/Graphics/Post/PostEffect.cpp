#include "PostEffect.h"

void PostEffect::Init(unsigned width, unsigned height)
{
	//Set up framebuffers
	if (!_shaders.size() > 0)
	{

		int index = int(_buffers.size());
		_buffers.push_back(new Framebuffer());
		_buffers[index]->AddColorTarget(GL_RGBA8);
		_buffers[index]->AddDepthTarget();
		_buffers[index]->Init(width, height);
	}

	_shaders.push_back(Shader::Create());
	_shaders[_shaders.size() - 1]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[_shaders.size() - 1]->LoadShaderPartFromFile("shaders/passthrough_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[_shaders.size() - 1]->Link();
}

void PostEffect::ApplyEffect(PostEffect* previousBuffer)
{
	BindShader(_shaders.size() - 1);

	previousBuffer->BindColorAsTexture(0, 0, 0);

	_buffers[0]->RenderToFSQ();

	previousBuffer->UnbindTexture(0);


	UnbindShader();
}

void PostEffect::DrawToScreen()
{
	BindShader(_shaders.size() - 1);

	BindColorAsTexture(0, 0, 0);

	_buffers[0]->DrawFullscreenQuad();

	UnbindTexture(0);

	UnbindShader();
}

void PostEffect::Reshape(unsigned width, unsigned height)
{
	for (unsigned int i = 0; i < _buffers.size(); i++)
	{
		_buffers[i]->Reshape(width, height);
	}
}

void PostEffect::Clear()
{
	for (unsigned int i = 0; i < _buffers.size(); i++)
	{
		_buffers[i]->Clear();
	}
}

void PostEffect::Unload()
{
	for (unsigned int i = 0; i < _buffers.size(); i++)
	{
		if (_buffers[i] != nullptr)
		{
			_buffers[i]->Unload();
			delete _buffers[i];
			_buffers[i] = nullptr;
		}
	}

	_shaders.clear();
}

void PostEffect::BindBuffer(int index)
{
	_buffers[index]->Bind();
}

void PostEffect::UnbindBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void PostEffect::BindColorAsTexture(int index, int colorBuffer, int textureSlot)
{
	_buffers[index]->BindColorAsTexture(colorBuffer, textureSlot);
}

void PostEffect::BindDepthAsTexture(int index, int textureSlot)
{
	_buffers[index]->BindDepthAsTexture(textureSlot);
}

void PostEffect::UnbindTexture(int textureSlot)
{
	//Binds texture at slot to GL_NONE
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void PostEffect::BindShader(int index)
{
	_shaders[index]->Bind();
}

void PostEffect::UnbindShader()
{
	glUseProgram(GL_NONE);
}
