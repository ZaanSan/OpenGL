#include "FrameBuffer.h"

#include <iostream>

#include "Renderer.h"

FrameBuffer::FrameBuffer()
	: m_Attachment(Texture::Type::DEPTH_MAP),
	m_DepthStencil(RenderBuffer::Type::DEPTH24_STENCIL8)
{
	GLCall(glGenFramebuffers(1, &m_FrameBufferID));
}

FrameBuffer::FrameBuffer(Texture::Type typeT, unsigned int widthT, unsigned int heightT, RenderBuffer::Type typeR)
	: m_Attachment(typeT, widthT, heightT),
	m_DepthStencil(typeR)
{
	GLCall(glGenFramebuffers(1, &m_FrameBufferID));
	Bind();

	m_Attachment.Bind(0);
	switch (typeT)
	{
	case Texture::Type::POST_PROCESSING:
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Attachment.m_TextureID, 0));
		m_DepthStencil.Bind();
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencil.m_RenderBufferID));
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		break;
	case Texture::Type::DEPTH_MAP:
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Attachment.m_TextureID, 0));
		GLCall(glDrawBuffer(GL_NONE));
		GLCall(glReadBuffer(GL_NONE));
		break;
	}
	Unbind();
}


FrameBuffer::~FrameBuffer()
{
	GLCall(glDeleteFramebuffers(1, &m_FrameBufferID));
}

void FrameBuffer::Bind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID));
}

void FrameBuffer::Unbind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::BindTextureBuffer(unsigned int slot) const
{
	m_Attachment.Bind(slot);
}

void FrameBuffer::UnbindTextureBuffer() const
{
	m_Attachment.Unbind();
}
