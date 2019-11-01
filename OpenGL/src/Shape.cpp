#include "Shape.h"

Shape::Shape(float* vertices, unsigned verticesSize, const VertexBufferLayout& layout, unsigned int* indices, unsigned indicesNb)
	: m_VBO(vertices, verticesSize),
	m_IBO(indices, indicesNb)
{
	m_VAO.Bind();
	m_VBO.Bind();
	m_VAO.AddBuffer(m_VBO, layout);
	m_IBO.Bind();
	m_VAO.Unbind();
	m_IBO.Unbind();
}

Shape::~Shape() = default;
