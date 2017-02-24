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

#include "sdl/SDLWindow.h"

using namespace ICSE::sdl;


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
	if (m_window != nullptr)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
}

void ICSE::sdl::SDLWindow::OnInit(void)
{
	
	
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
	

	
}

SDL_Window * ICSE::sdl::SDLWindow::window(void)
{
	return m_window;
}

uint32_t ICSE::sdl::SDLWindow::windowID(void)
{
	return SDL_GetWindowID(m_window);
}


