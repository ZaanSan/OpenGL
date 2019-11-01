#pragma once

#include <glad/glad.h>
#include "Texture.h"
#include "RenderBuffer.h"

class FrameBuffer
{
private:
	unsigned int m_FrameBufferID;

	// Predefined FrameBuffer : texture attachment for color buffer and depth24_stencil8 as depth and stencil
	// Designed to use for Post Processing Effects
	Texture m_Attachment;
	RenderBuffer m_DepthStencil;

public:
	// Generate empty frame buffer
	FrameBuffer();
	FrameBuffer(Texture::Type typeT, unsigned int widthT, unsigned int heightT, RenderBuffer::Type typeR = RenderBuffer::Type::DEPTH24_STENCIL8);
	~FrameBuffer();

	void Bind() const;
	void Unbind() const;

	void BindTextureBuffer(unsigned int slot = 0) const;
	void UnbindTextureBuffer() const;
};

