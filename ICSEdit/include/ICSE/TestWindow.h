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
#include "sdl/SDLWindow.h"
#include "graphics/Mesh2D.h"
#include "graphics/Mesh2DRenderer.h"
#include "graphics/gl/GLTexture.h"

namespace ICSE {

	class TestWindow : public sdl::SDLWindow {
		SDL_GLContext m_context;
		ICSE::graphics::Mesh2D m_mesh;
		ICSE::graphics::gl::GLShaderSet m_shader;
		std::unique_ptr<ICSE::graphics::Mesh2DRenderer> m_renderer;
		GLuint m_unif_matmv;
		ICSE::graphics::gl::GLTexture m_texture;
		int m_width;
		int m_height;
	 public:
		TestWindow(void);

		void OnInit(void);
		virtual void Update(void) override;
		virtual void OnEvent(SDL_WindowEvent &evt) override;
	 private:
		 void checkFPS(void);
	};

}
