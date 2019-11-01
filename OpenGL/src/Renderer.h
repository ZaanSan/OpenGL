#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "Model.h"
#include "Shape.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

// Non-static methods because may store state to avoid some calls..
class Renderer
{
public:

	enum class DepthFunction : char
	{
		LESS,
		LEQUAL
	};

	enum class CullFace : char
	{
		FRONT,
		BACK
	};

public:
	void DrawPoints(const VertexArray& va, unsigned int nbPoints, const Shader& shader) const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Draw(const Model& model, Shader& shader) const;
	void Draw(const Shape& shape, Shader& shader) const;
	void DrawInstanced(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, unsigned int count) const;
	void DrawInstanced(const Model& model, Shader& shader, unsigned int count) const;
	void Clear() const;
	
	void EnableBlending() const;
	void EnableDepthTesting() const;
	void EnableStencilTesting() const;
	void EnableBackfaceCulling() const;
	void EnableDepthWriting() const;
	void EnableMultiSampling() const;

	void DisableDepthTesting() const;
	void DisableBackfaceCulling() const;
	void DisableDepthWriting() const;
	void DisableMultiSampling() const;

	void SetViewport(int x, int y, unsigned int width, unsigned int height);
	void SetCullFace(CullFace face);
	void SetDepthFunction(DepthFunction function);
};
