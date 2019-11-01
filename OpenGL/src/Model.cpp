#include "Model.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

Model::Model(const char* path)
{
	loadModel(path);
}

Model::~Model()
{
}

void Model::AddInstancedArray(const VertexBuffer& vbo, const VertexBufferLayout& layout, unsigned position)
{
	unsigned int nbElems = layout.GetElements().size();
	for (auto& mesh : m_Meshes)
	{
		auto& vao = mesh.m_VAO;
		vao->Bind();
		vao->AddBuffer(vbo, layout, position);
		for (unsigned int j = 0; j < nbElems; ++j)
		{
			vao->DefineInstancedAttribute(position + j, 1);
		}
		vao->Unbind();
	}
}

void Model::loadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Error : Model loading : " << importer.GetErrorString() << std::endl;
		return;
	}
	m_Directory = path.substr(0, path.find_last_of('/')) + "/";
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex>				vertices;
	std::vector<unsigned int>		indices;
	std::vector<TextureInformation>	textures;

	// Vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.Position = glm::vec3(mesh->mVertices[i].x,
									mesh->mVertices[i].y,
									mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x,
								  mesh->mNormals[i].y,
								  mesh->mNormals[i].z);
		if (mesh->HasTextureCoords(0))
		{
			vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
										 mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	// Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	// Textures
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<TextureInformation> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		std::vector<TextureInformation> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		std::vector<TextureInformation> reflectionMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_reflection");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		textures.insert(textures.end(), reflectionMaps.begin(), reflectionMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<TextureInformation> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<TextureInformation> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		(void)mat->GetTexture(type, i, &str);
		TextureInformation texture;
		texture.type = typeName;
		texture.path = m_Directory + str.C_Str();
		textures.push_back(texture);
	}
	return textures;
}