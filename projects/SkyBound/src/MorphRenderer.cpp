#include "MorphRenderer.h"

MorphRenderer::AnimData::AnimData()
{
	frameIndex = 0;
	frameTime = 0.2f;
}


MorphRenderer::MorphRenderer()
{
	m_anim = std::make_unique<AnimData>();
	m_t = 0;
	m_timer = 0;
}

void MorphRenderer::addFrame(std::shared_ptr<MeshBuilder<VertexPosNormTexCol>> mesh)
{
	if (m_anim->frame.size() == 0)
	{
		vao->SetIndexBuffer(mesh->returnEBO());
	}
	
	m_anim->frame.push_back(mesh->returnVBO());
	
}


void MorphRenderer::nextFrame(float dt)
{
	if (!&m_anim->frame)
		return;

	float t;

	if (m_anim->frameTime > 0.0f)
	{
		m_timer += dt;

		if (m_timer > m_anim->frameTime)
		{
			m_timer -= m_anim->frameTime;

			m_anim->frameIndex += 1;

			if (m_anim->frameIndex >= m_anim->frame.size())
				m_anim->frameIndex = 0;

		}

		m_timer = fmod(m_timer, m_anim->frameTime);

		t = m_timer / m_anim->frameTime;
	}
	else
	{
		t = 0.0f;
	}

	size_t f0Index = 0;
	size_t f1Index = 0;

	f1Index = m_anim->frameIndex;

	if (f1Index == 0)
	{
		f0Index = m_anim->frame.size() - 1;
	}
	else
	{
		f0Index = f1Index - 1;
	}


	UpdateData(m_anim->frame[f0Index], m_anim->frame[f1Index], t);
}


void MorphRenderer::UpdateData(VertexBuffer::sptr frame0, VertexBuffer::sptr frame1, float t)
{
	VertexPosNormTexCol* VPNTC = nullptr;

	vao->AddVertexBuffer(frame0, { //Colour
								   BufferAttribute(1, 3, GL_FLOAT, false, sizeof(VertexPosNormTexCol), (size_t)&VPNTC->Color, AttribUsage::Color),
								   //UV
								   BufferAttribute(3, 2, GL_FLOAT, false, sizeof(VertexPosNormTexCol), (size_t)&VPNTC->UV, AttribUsage::Texture),
								   //Position 1
								   BufferAttribute(0, 3, GL_FLOAT, false, sizeof(VertexPosNormTexCol), (size_t)&VPNTC->Position, AttribUsage::Position),
								   //Normal 1
								   BufferAttribute(2, 3, GL_FLOAT, false, sizeof(VertexPosNormTexCol), (size_t)&VPNTC->Normal, AttribUsage::Normal) });

	

	vao->AddVertexBuffer(frame1, { //Position 2
								   BufferAttribute(4, 3, GL_FLOAT, false, sizeof(VertexPosNormTexCol), (size_t)&VPNTC->Position, AttribUsage::Position),
								   //Normal 2
								   BufferAttribute(5, 3, GL_FLOAT, false, sizeof(VertexPosNormTexCol), (size_t)&VPNTC->Normal, AttribUsage::Normal) });

	

	m_t = t;
}



void MorphRenderer::SetFrameTime(float frameTime)
{
	m_anim->frameTime = frameTime;
}



void MorphRenderer::render(
	const Shader::sptr& shader,
	const glm::mat4& viewProjection,
	Transform& transform)
{
	
	shader->Bind();
	Material->Apply();
	shader->SetUniformMatrix("u_ModelViewProjection", viewProjection * transform.LocalTransform());
	shader->SetUniformMatrix("u_Model", transform.LocalTransform());
	shader->SetUniformMatrix("u_NormalMatrix", transform.NormalMatrix());
	shader->SetUniform("t", m_t);
	vao->Render();
}



