#pragma once
#include "graphics/gl/GLShaderSet.h"
#include "graphics/gl/GLVBO.h"

namespace ICSE{
namespace sdl{

	class SDLWindow {
		SDL_Window *m_window;
		SDL_GLContext m_context;
		ICSE::graphics::gl::GLShaderSet m_shader;
		ICSE::graphics::gl::GLVBO m_vbo;

	public:
		SDLWindow(void);
		SDLWindow(const char *name, uint32_t width, uint32_t height, int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

		SDL_Event WaitEvent(void);

		~SDLWindow(void);

		virtual void OnInit(void);
		virtual void Update(void);

		SDL_Window *window(void);

	private:
		void initWindow(const char *name, uint32_t width, uint32_t height, int flags);
	};

}
}