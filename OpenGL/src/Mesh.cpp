#include "Mesh.h"
#include <utility>
#include "TextureManager.h"
#include "VertexBufferLayout.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<TextureInformation> textures)
	: vertices(std::move(vertices)),
	indices(std::move(indices)),
	textures(std::move(textures))
{
	setupMesh();
}

void Mesh::setupMesh()
{
	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();
	m_VBO = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_IBO = std::make_unique<IndexBuffer>(&indices[0], indices.size());
	m_VAO->AddBuffer(*m_VBO, layout);
	m_VAO->Unbind();
}
