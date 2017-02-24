#include "stdafx.h"

#include "sdl/SDLHandler.h"
#include "sdl/SDLWindow.h"
#include "graphics/gl/GLHandler.h"

using ICSE::sdl::SDLHandler;


#if defined WINVER

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdline, int nCmdShow)

#else

	int main(int ,char**)

#endif

	{
		SDLHandler sdl;
		ICSE::Singleton<ICSE::graphics::gl::GLHandler>::create();
		ICSE::sdl::SDLWindow wnd("Window", 256, 256);
		wnd.OnInit();

		while (wnd.WaitEvent().window.event != SDL_WINDOWEVENT_CLOSE)
		{
			wnd.Update();
			//SDL_Delay(16);
		}
		ICSE::Singleton<ICSE::graphics::gl::GLHandler>::destroy();

		return 0;
	}

