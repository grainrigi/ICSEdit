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

#include "wnd/WindowManager.h"

using namespace ICSE::wnd;

ICSE::wnd::WindowManager::WindowManager(void)
{
}

void ICSE::wnd::WindowManager::RegisterWindow(std::shared_ptr<ICSE::sdl::SDLWindow> window, bool isMain)
{
	m_windows.insert(std::make_pair(window->windowID(), window));
	if (isMain)
		m_main = window.get();
}

bool ICSE::wnd::WindowManager::ProcessEvent(void)
{
	constexpr std::chrono::nanoseconds frame(16000000);

	SDL_Event ev;
	SDL_Keycode key;

	auto start = std::chrono::high_resolution_clock::now();

	while (SDL_PollEvent(&ev))
	{
		if (ev.type == SDL_QUIT)
			return false;
		else if (ev.type == SDL_WINDOWEVENT)
		{
			auto it = m_windows.find(ev.window.windowID);
			if (it != m_windows.end())
				it->second->OnEvent(ev.window);
		}
	}

	

	for (auto wnd : m_windows)
		wnd.second->Update();

	if (m_quitting)
		return false;

	auto end = std::chrono::high_resolution_clock::now();

	auto diff = end - start;
	auto waitms = std::chrono::duration_cast<std::chrono::milliseconds>(frame - diff).count();

	//if (waitms > 20)
	//	__asm { int 3};

	if (waitms < 0)
		waitms = 0;

	SDL_Delay(waitms);
	//SDL_Delay(16);

	return true;
}

void ICSE::wnd::WindowManager::PostQuitMessage(void)
{
	m_quitting = true;
}
