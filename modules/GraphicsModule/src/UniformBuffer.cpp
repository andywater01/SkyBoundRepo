#include "UniformBuffer.h"

UniformBuffer::UniformBuffer()
{

}

UniformBuffer::UniformBuffer(unsigned int bytes)
{
	AllocateMemory(bytes);
}

UniformBuffer::~UniformBuffer()
{
	Unload();
}

void UniformBuffer::Unload()
{
	if (_Handle)
	{
		glDeleteBuffers(1, &_Handle);
	}
}

bool UniformBuffer::IsInit()
{
	return _IsInit;
}

void UniformBuffer::AllocateMemory(unsigned int bytes)
{
	if (_Handle)
	{
		Unload();
	}
	glGenBuffers(1, &_Handle);
	_IsInit = true;
	Bind();
	// This allocates 'n' bytes on the GPU, after which, data can now be sent to the Buffer
	glBufferData(GL_UNIFORM_BUFFER, bytes, GL_NONE, GL_DYNAMIC_DRAW);
	Unbind();
}

void UniformBuffer::SendMatrix(const glm::mat4& matrix, int offset) const
{
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), &matrix);
	Unbind();
}

void UniformBuffer::SendVector(const glm::vec3& vector, const int offset) const
{
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), &vector);
	Unbind();
}

void UniformBuffer::SendVector(const glm::vec4& vector, int offset) const
{
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), &vector);
	Unbind();
}

void UniformBuffer::SendFloat(float scalar, int offset) const
{
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &scalar);
	Unbind();
}

void UniformBuffer::SendInt(int number, int offset) const
{
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &number);
	Unbind();
}

void UniformBuffer::SendUInt(unsigned int number, int offset) const
{
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(unsigned int), &number);
	Unbind();
}

void UniformBuffer::SendBool(bool boolean, int offset) const
{
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(bool), &boolean);
	Unbind();
}

void UniformBuffer::SendData(void* data, int size, int offset) const
{
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	Unbind();
}

void UniformBuffer::Bind(GLuint slot)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, _Handle);
	_BindLocation = slot;
}

void UniformBuffer::Unbind(GLuint slot)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, GL_NONE);
	_BindLocation = -1;
}

void UniformBuffer::Bind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, _Handle);
}

void UniformBuffer::Unbind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}