#include "ColorCorrectionEffect.h"

void ColorCorrectionEffect::Init(unsigned width, unsigned height)
{
	int index = int(_buffers.size());
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->AddDepthTarget();
	_buffers[index]->Init(width, height);

	//Set up shaders
	index = int(_shaders.size());
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/color_correction_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();

	_cube.loadFromFile("cubes/TwilightCorrection.cube");

	//Call base class Init
	PostEffect::Init(width, height);
}

void ColorCorrectionEffect::ApplyEffect(PostEffect* buffer)
{
	BindShader(0);
	_cube.bind(30);

	buffer->BindColorAsTexture(0, 0, 0);

	_buffers[0]->RenderToFSQ();

	buffer->UnbindTexture(0);
	_cube.unbind(30);

	UnbindShader();
}

LUT3D ColorCorrectionEffect::GetCube() const
{
	return _cube;
}

void ColorCorrectionEffect::SetCube(LUT3D cube)
{
	_cube = cube;
}


