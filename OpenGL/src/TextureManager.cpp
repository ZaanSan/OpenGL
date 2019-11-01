#include "TextureManager.h"
#include <iostream>

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager() = default;


TextureManager& TextureManager::Instance()
{
	static TextureManager instance;
	return instance;
}

const Texture& TextureManager::GetTexture(const std::string& path)
{
	if (m_Textures.find(path) != m_Textures.end())
		return *m_Textures[path];
	m_Textures[path] = std::make_unique<Texture>(path);
	return *m_Textures[path];
}