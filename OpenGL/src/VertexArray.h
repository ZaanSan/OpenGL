#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_VertexArrayID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, unsigned int position = 0);
	void DefineInstancedAttribute(unsigned int index, unsigned int divisor);

	void Bind() const;
	void Unbind() const;
};

