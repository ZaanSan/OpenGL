#include "Renderer.h"
#include <glad/glad.h>
#include <iostream>
#include "TextureManager.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::DrawPoints(const VertexArray& va, unsigned nbPoints, const Shader& shader) const
{
	va.Bind();
	shader.Bind();
	GLCall(glDrawArrays(GL_POINTS, 0, nbPoints));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	va.Bind();
	shader.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const Model& model, Shader& shader) const
{
	TextureManager& ts = TextureManager::Instance();
	shader.Bind();
	for (auto& mesh : model.m_Meshes)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		unsigned int reflectionNr = 1;
		for (unsigned int i = 0; i < mesh.textures.size(); i++)
		{
			std::string number;
			std::string name = mesh.textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);
			else if (name == "texture_reflection")
				number = std::to_string(reflectionNr++);

			shader.SetUniform1i(("material." + name + number), i);
			ts.GetTexture(mesh.textures[i].path).Bind(i);
		}
		mesh.m_VAO->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, mesh.m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr));
		mesh.m_VAO->Unbind();
		glActiveTexture(GL_TEXTURE0);
		
	}
}

void Renderer::Draw(const Shape& shape, Shader& shader) const
{
	shape.m_VAO.Bind();
	shader.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, shape.m_IBO.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawInstanced(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, unsigned count) const
{
	va.Bind();
	shader.Bind();
	GLCall(glDrawElementsInstanced(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr, count));
}

void Renderer::DrawInstanced(const Model& model, Shader& shader, unsigned count) const
{
	TextureManager& ts = TextureManager::Instance();
	shader.Bind();
	for (auto& mesh : model.m_Meshes)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		unsigned int reflectionNr = 1;
		for (unsigned int i = 0; i < mesh.textures.size(); i++)
		{
			std::string number;
			std::string name = mesh.textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);
			else if (name == "texture_reflection")
				number = std::to_string(reflectionNr++);

			shader.SetUniform1i(("material." + name + number), i);
			ts.GetTexture(mesh.textures[i].path).Bind(i);
		}
		mesh.m_VAO->Bind();
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, mesh.m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr, count));
		mesh.m_VAO->Unbind();
		glActiveTexture(GL_TEXTURE0);
	}
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void Renderer::EnableBlending() const
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Renderer::EnableDepthTesting() const
{
	GLCall(glEnable(GL_DEPTH_TEST));
}

void Renderer::EnableStencilTesting() const
{
	GLCall(glEnable(GL_STENCIL_TEST));
}

void Renderer::EnableBackfaceCulling() const
{
	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glCullFace(GL_BACK)); // Default value
}

void Renderer::EnableDepthWriting() const
{
	GLCall(glDepthMask(GL_TRUE));
}

void Renderer::EnableMultiSampling() const
{
	GLCall(glEnable(GL_MULTISAMPLE));
}


void Renderer::DisableBackfaceCulling() const
{
	GLCall(glDisable(GL_CULL_FACE));
}

void Renderer::DisableDepthTesting() const
{
	GLCall(glDisable(GL_DEPTH_TEST));
}

void Renderer::DisableDepthWriting() const
{
	GLCall(glDepthMask(GL_FALSE));
}

void Renderer::DisableMultiSampling() const
{
	GLCall(glDisable(GL_MULTISAMPLE));
}

void Renderer::SetViewport(int x, int y, unsigned width, unsigned height)
{
	GLCall(glViewport(x, y, width, height));
}

void Renderer::SetCullFace(CullFace face)
{
	switch (face)
	{
	case CullFace::FRONT: GLCall(glCullFace(GL_FRONT)); break;
	case CullFace::BACK: GLCall(glCullFace(GL_BACK)); break;
	}
}

void Renderer::SetDepthFunction(DepthFunction function)
{
	switch (function)
	{
	case DepthFunction::LESS: GLCall(glDepthFunc(GL_LESS)); break;
	case DepthFunction::LEQUAL: GLCall(glDepthFunc(GL_LEQUAL)); break;
	}
}

