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

#include "ICSE/TestWindow.h"
#include "wnd/WindowManager.h"

using namespace ICSE;
using ICSE::graphics::gl::GLShaderSet;
using ICSE::graphics::gl::GLVBO;
using ICSE::graphics::gl::GLVBOTarget;

ICSE::TestWindow::TestWindow(void)
	: ICSE::sdl::SDLWindow(u8"ウィンドウ", 256, 256)
{
	this->OnInit();
}

void ICSE::TestWindow::OnInit(void)
{
	std::string vShader{
		"#version 140\n"
		"in vec4 attr_pos;"
		"in vec4 attr_col;"
		"out vec4 vary_col;"
		"void main() { "
		"  gl_Position = attr_pos;"
		"  vary_col = attr_col;"
		"}" };
	std::string fShader{
		"#version 120\n"
		"in vec4 vary_col;"
		"void main() { "
		"  gl_FragColor = vary_col;"
		"}" };
	float position[] = {
		0.0f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	};
	float color[] ={
		1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
	};
	

	m_context = SDL_GL_CreateContext(window());
	


	glViewport(0, 0, 256, 256);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	m_shader = GLShaderSet::createFromString(
	vShader, fShader,
	{ { 0, "attr_pos" }, { 1, "attr_col" } }
	);

	m_vbo = GLVBO::GLVBO(GLVBOTarget::ARRAY_BUFFER, GL_STATIC_DRAW);
	m_vbo.extend(sizeof(position) + sizeof(color));
	m_vbo.uploadDataRange(sizeof(position), position, 0);
	m_vbo.uploadDataRange(sizeof(color), color, sizeof(position));
}

void ICSE::TestWindow::Update(void)
{
	m_shader.use();
	m_vbo.bind();

	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)36);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	SDL_GL_SwapWindow(window());
}

void ICSE::TestWindow::OnEvent(SDL_WindowEvent &evt)
{
	if (evt.event == SDL_WINDOWEVENT_CLOSE)
		ICSE::Singleton<ICSE::wnd::WindowManager>::getInstance().PostQuitMessage();
}
