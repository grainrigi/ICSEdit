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

#include "sdl/SDLHandler.h"

using ICSE::sdl::SDLHandler;

SDLHandler::SDLHandler(void)
	: m_isInit(false)
{
	m_isInit = initVideo();
}

ICSE::sdl::SDLHandler::~SDLHandler(void)
{
	if(m_isInit)
	{
		SDL_Quit();
		m_isInit = false;
	}
}

bool ICSE::sdl::SDLHandler::initVideo(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		THROW(std::runtime_error, "SDL_Init Error: ", SDL_GetError());
		return false;
	}

	return true;
}
