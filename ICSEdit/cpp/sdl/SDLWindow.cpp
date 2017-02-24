#include "sdl/SDLWindow.h"

using namespace ICSE::sdl;
using ICSE::graphics::gl::GLShaderSet;
using ICSE::graphics::gl::GLVBO;
using ICSE::graphics::gl::GLVBOTarget;

ICSE::sdl::SDLWindow::SDLWindow(void)
{
	
}

ICSE::sdl::SDLWindow::SDLWindow(const char *name, uint32_t width, uint32_t height, int flags)
{
	this->initWindow(name, width, height, flags);
	//this->OnInit();
}

SDL_Event ICSE::sdl::SDLWindow::WaitEvent(void)
{
	SDL_Event ev;
	SDL_Keycode key;
	
	SDL_PollEvent(&ev);

	return ev;
}

ICSE::sdl::SDLWindow::~SDLWindow(void)
{

}

void ICSE::sdl::SDLWindow::OnInit(void)
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

void ICSE::sdl::SDLWindow::initWindow(const char *name, uint32_t width, uint32_t height, int flags)
{
	
	m_window = SDL_CreateWindow(name,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, flags);

	//this->OnInit();
}

void ICSE::sdl::SDLWindow::Update(void)
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
	SDL_GL_SwapWindow(m_window);

	
}

SDL_Window * ICSE::sdl::SDLWindow::window(void)
{
	return m_window;
}


