#pragma once

#include "Texture.h"
#include <unordered_map>
#include <memory>

class TextureManager
{
private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_Textures;

public:
	static TextureManager &Instance();

	const Texture& GetTexture(const std::string& path);

private:
	TextureManager();
	~TextureManager();
};

