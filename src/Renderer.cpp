// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Renderer.h"

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& t_Vao, const IndexBuffer& t_Ibo, const Shader& t_Program) const
{
	t_Program.Bind();
	t_Vao.Bind();
	t_Ibo.Bind();
	glDrawElements(GL_TRIANGLES, t_Ibo.getCount(), GL_UNSIGNED_INT, nullptr);
}