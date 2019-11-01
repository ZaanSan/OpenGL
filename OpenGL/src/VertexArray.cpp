#include "VertexArray.h"

#include <glad/glad.h>

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_VertexArrayID));
}


VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_VertexArrayID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, unsigned position)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glVertexAttribPointer(i + position, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		GLCall(glEnableVertexAttribArray(i + position));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::DefineInstancedAttribute(unsigned index, unsigned divisor)
{
	GLCall(glVertexAttribDivisor(index, divisor));
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_VertexArrayID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
