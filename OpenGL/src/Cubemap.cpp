#include "Cubemap.h"

#include <iostream>

#include "stb_image/stb_image.h"

#include "Renderer.h"

Cubemap::Cubemap(unsigned int width, unsigned int height)
{
	GLCall(glGenTextures(1, &m_CubemapID));
	Bind(0);
	for (unsigned int i = 0; i < 6; ++i)
	{
		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	}

	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	Unbind();
}


Cubemap::Cubemap(const std::vector<std::string>& faces)
{
	m_filenames = faces;

	GLCall(glGenTextures(1, &m_CubemapID));
	Bind(0);

	int width, height, nrChannels;
	unsigned char *data;
	
	for (GLuint i = 0; i < m_filenames.size(); i++)
	{
		data = stbi_load(m_filenames[i].c_str(), &width, &height, &nrChannels, 0);

		if (data == nullptr) {
			std::cout << "CubeMap(Texture) : Failed to load Texture from file" << std::endl;
			assert(0);
		}

		GLenum format = GL_RGBA;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		
		GLCall(glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data
		));
		stbi_image_free(data);
	}

	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	Unbind();
}

Cubemap::~Cubemap()
{
	if (m_CubemapID != 0)
		GLCall(glDeleteTextures(1, &m_CubemapID));
}

void Cubemap::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapID));
}

void Cubemap::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void Cubemap::BindAsDepthAttachment(const FrameBuffer& fbo) const
{
	fbo.Bind();
	GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_CubemapID, 0));
	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));
	fbo.Unbind();
}
