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
#include "graphics/Mesh2DRenderer.h"
#include "graphics/MemCanvasRenderer.h"
#include "wnd/WindowLayer.h"
#include "util/basic/Delegate.h"

namespace ICSE {
namespace wnd {
	class WindowLayer;

	class ControlWindow : public sdl::SDLWindow {
		//Renderers
		graphics::Mesh2DRenderer m_m2drend;
		graphics::MemCanvasRenderer m_cvsrend;
		ControlRendererShader m_ctlshader;

		SDL_GLContext m_context;
		std::unordered_map<uint32_t, std::shared_ptr<WindowLayer>> m_layers;

	protected:
		
		std::shared_ptr<WindowLayer> createLayer(void);
		void removeLayer(uint32_t id);

		Delegate<> dUpdate;

	public:
		ControlWindow(const char *name = "", uint32_t width = 640, uint32_t height = 480, int flags = SDL_WINDOW_SHOWN);
		
		graphics::Mesh2DRenderer &GetM2DRenderer(void) { return m_m2drend; }
		graphics::MemCanvasRenderer &GetCVSRenderer(void) { return m_cvsrend; }

		virtual void Update(void) override;

		void MakeCurrent(void);
	private:
		void initContextAndRenderer(void);
		void OnUpdate(void);
	};

}
}
