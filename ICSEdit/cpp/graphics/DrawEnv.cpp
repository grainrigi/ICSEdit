/*
(c) 2016,2017 Grain

This file is part of ICSEdit.

ICSEdit is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ICSEdit is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ICSEdit.  If not, see <http://www.gnu.org/licenses/>.
*/

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
