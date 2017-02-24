#include "graphics/gl/GLInitWindow.h"

using namespace ICSE::graphics::gl;

ICSE::graphics::gl::GLInitWindow::GLInitWindow(void)
	: SDLWindow("GLInit", 1, 1, SDL_WINDOW_OPENGL)
{
	//__asm {int 3};
	OnInit();
}

void ICSE::graphics::gl::GLInitWindow::OnInit(void)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	m_context = SDL_GL_CreateContext(window());
	GLenum result = glewInit();
	if (result != GLEW_OK)
		THROW(std::runtime_error, "GLEW Init failed.");
		//__asm { int 3 };
	if (SDL_GL_SetSwapInterval(1) < 0)
		THROW(std::runtime_error, "Unable to set VSync");
		//__asm { int 3 };

	SDL_HideWindow(window());
}
