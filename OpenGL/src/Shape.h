#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"

class Shape
{
	friend class Renderer;

private:
	VertexArray		m_VAO;
	VertexBuffer	m_VBO;
	IndexBuffer		m_IBO;

public:
	Shape(float* vertices, unsigned int verticesSize, const VertexBufferLayout& layout, unsigned int* indices, unsigned int indicesNb);
	~Shape();
};

