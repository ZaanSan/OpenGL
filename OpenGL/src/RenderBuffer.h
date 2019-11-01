#pragma once

class RenderBuffer
{
	friend class FrameBuffer;

private:
	unsigned int m_RenderBufferID;

public:
	enum class Type : char
	{
		DEPTH32,
		STENCIL32,
		DEPTH24_STENCIL8
	};

public:
	RenderBuffer(Type type);
	~RenderBuffer();

	void Bind() const;
	void Unbind() const;
};

