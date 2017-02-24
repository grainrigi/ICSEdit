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
