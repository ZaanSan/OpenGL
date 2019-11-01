#pragma once

#include <vector>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct TextureInformation
{
	std::string		type;
	std::string		path;
};

class Mesh
{
	friend class Renderer;
	friend class Model;

public:
	std::vector<Vertex>						vertices;
	std::vector<unsigned int>				indices;
	std::vector<TextureInformation>			textures;

private:
	std::unique_ptr<VertexArray>			m_VAO;
	std::unique_ptr<VertexBuffer>			m_VBO;
	std::unique_ptr<IndexBuffer>			m_IBO;

public:
	Mesh(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<TextureInformation> textures);
private:
	void setupMesh();
};
