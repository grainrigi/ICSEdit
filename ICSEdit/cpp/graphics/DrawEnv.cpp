#include "graphics/DrawEnv.h"

using namespace ICSE::graphics;

DrawEnv::DrawEnv(void)
	: m_texture(0),
	m_program(0)
{
}

void ICSE::graphics::DrawEnv::setShader(const ICSE::graphics::gl::GLShaderSet & shader)
{
	if(m_program != shader.program())
		glUseProgram(shader.program());
	m_program = shader.program();
}
