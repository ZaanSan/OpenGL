#include "Texture.h"

#include <utility>
#include <iostream>

#include "stb_image/stb_image.h"

#include "Renderer.h"

Texture::Texture(Type type, unsigned int width, unsigned int height)
	: m_TextureID(0), m_Width(0), m_Height(0), m_BPP(0), m_filePath(""), m_LocalBuffer(nullptr)
{
	GLCall(glGenTextures(1, &m_TextureID));
	Bind(0);

	switch (type)
	{
	case Type::POST_PROCESSING:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		break;
	case Type::DEPTH_MAP:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
		break;
	}
	Unbind();
}

Texture::Texture(std::string path, int wrapS, int wrapT, int minFilter, int magFilter)
	: m_TextureID(0), m_Width(0), m_Height(0), m_BPP(0), m_filePath(std::move(path)), m_LocalBuffer(nullptr)
{
	stbi_set_flip_vertically_on_load(true);
	m_LocalBuffer = stbi_load(m_filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLenum format = GL_RGBA;
	if (m_BPP == 1)
		format = GL_RED;
	else if (m_BPP == 3)
		format = GL_RGB;
	else if (m_BPP == 4)
		format = GL_RGBA;

	GLCall(glGenTextures(1, &m_TextureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
	else
		std::cout << "Failed to load texture : " << m_filePath << std::endl;
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_TextureID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

int Texture::GetWidth() const
{
	return m_Width;
}

int Texture::GetHeight() const
{
	return m_Height;
}

