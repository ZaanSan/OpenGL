#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

#include "FrameBuffer.h"

class Cubemap
{
private:
	unsigned int m_CubemapID;
	std::vector<std::string> m_filenames;

public:
	// For depth cubemap (omnidirectional shadow map)
	Cubemap(unsigned int width, unsigned int height);
	// For skybox
	Cubemap(const std::vector<std::string>& faces);
	~Cubemap();

	void Bind(unsigned int slot) const;
	void Unbind() const;
	void BindAsDepthAttachment(const FrameBuffer& fbo) const;


	Cubemap(const Cubemap& other) = delete;
	Cubemap& operator=(const Cubemap& other) = delete;
};
