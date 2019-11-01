#pragma once
#include <string>
#include <glad/glad.h>

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

class Texture
{
	friend class FrameBuffer;

public:
	enum class Type : char
	{
		POST_PROCESSING,
		DEPTH_MAP
	};

private:
	unsigned int	m_TextureID;
	int				m_Width;
	int				m_Height;
	int				m_BPP;
	std::string		m_filePath;
	unsigned char*	m_LocalBuffer;

public:

	/**
	 * Constructor for empty texture (attach to framebuffer)
	 */
	Texture(Type type, unsigned int width = SCR_WIDTH, unsigned int height = SCR_HEIGHT);

	/**
	 * Constructor for normal texture (from file)
	 */
	Texture(std::string path, int wrapS = GL_REPEAT, int wrapT = GL_REPEAT, int minFilter = GL_LINEAR, int magFilter = GL_LINEAR);
	~Texture();

	void Bind(unsigned int slot) const;
	void Unbind() const;

	int GetWidth() const;
	int GetHeight() const;
};

