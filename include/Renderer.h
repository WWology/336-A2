#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

class Renderer
{
private:
public:
	Renderer();

	void Clear() const;
	void Draw(const VertexArray& t_Vao, const IndexBuffer& t_Ibo, const Shader& t_Program) const;
	void Draw(const VertexArray& t_Vao, const Shader& t_Program) const;
};
