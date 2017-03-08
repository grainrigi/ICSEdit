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

#pragma once
#include "graphics/gl/GLShaderSet.h"
#include "graphics/gl/GLVBO.h"

namespace ICSE{
namespace sdl{

	class SDLWindow {
		SDL_Window *m_window;
		

	public:
		SDLWindow(const char *name = "", uint32_t width = 640, uint32_t height = 480, int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

		SDL_Event WaitEvent(void);

		~SDLWindow(void);

		void OnInit(void);
		virtual void Update(void);
		virtual void OnEvent(SDL_WindowEvent &evt) {};

		SDL_Window *window(void);
		uint32_t windowID(void);

		int width(void);
		int height(void);
		void SetWidth(int w);
		void SetHeight(int h);

	private:
		void initWindow(const char *name, uint32_t width, uint32_t height, int flags);
	};

}
}