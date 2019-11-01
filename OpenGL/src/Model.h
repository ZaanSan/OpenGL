#pragma once

#include <vector>

#include <assimp/scene.h>

#include "Mesh.h"

class Model
{
	friend class Renderer;

private:
	std::vector<Mesh>	m_Meshes;
	std::string			m_Directory;

public:
	explicit Model(const char* path);
	~Model();

	void AddInstancedArray(const VertexBuffer& vbo, const VertexBufferLayout& layout, unsigned position);

private:
	void	loadModel(const std::string& path);
	void	processNode(aiNode* node, const aiScene* scene);
	Mesh	processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<TextureInformation> loadMaterialTextures(aiMaterial* mat,
														 aiTextureType type,
														 std::string typeName);
};

