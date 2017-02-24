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

#include "stdafx.h"

#include "sdl/SDLHandler.h"
#include "sdl/SDLWindow.h"
#include "graphics/gl/GLHandler.h"
#include "ICSE/TestWindow.h"
#include "wnd/WindowManager.h"

using ICSE::sdl::SDLHandler;


#if defined WINVER

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdline, int nCmdShow)

#else

	int main(int ,char**)

#endif

	{
		SDLHandler sdl;
		ICSE::Singleton<ICSE::graphics::gl::GLHandler>::create();
		ICSE::Singleton<ICSE::wnd::WindowManager>::create();

		ICSE::wnd::WindowManager *wman = ICSE::Singleton<ICSE::wnd::WindowManager>::getInstancePtr();
		
		auto testwin = wman->CreateAndRegisterWindow<ICSE::TestWindow>();
		
		while (wman->ProcessEvent());
		
		ICSE::Singleton<ICSE::wnd::WindowManager>::destroy();
		ICSE::Singleton<ICSE::graphics::gl::GLHandler>::destroy();

		testwin.reset();

		return 0;
	}

