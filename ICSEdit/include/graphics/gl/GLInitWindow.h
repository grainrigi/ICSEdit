#pragma once
#include "sdl/SDLWindow.h"

namespace ICSE {
namespace graphics {
namespace gl {

	class GLInitWindow : public ICSE::sdl::SDLWindow {		
		SDL_GLContext m_context;

	 public:
		GLInitWindow(void);
		
		virtual void OnInit(void) override;
		virtual void Update(void) override {};
	 private:
		
	};

}
}
}
