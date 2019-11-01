
#include "RenderBuffer.h"
#include "Renderer.h"


extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;


RenderBuffer::RenderBuffer(Type type)
{
	glGenRenderbuffers(1, &m_RenderBufferID);
	Bind();
	switch (type)
	{
	case Type::DEPTH24_STENCIL8:
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT));
		break;
	case Type::DEPTH32:
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH, SCR_WIDTH, SCR_HEIGHT));
		break;
	case Type::STENCIL32:
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL, SCR_WIDTH, SCR_HEIGHT));
		break;
	}
	Unbind();
}

RenderBuffer::~RenderBuffer()
{
	GLCall(glDeleteRenderbuffers(1, &m_RenderBufferID));
}

void RenderBuffer::Bind() const
{
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
}

void RenderBuffer::Unbind() const
{
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}
