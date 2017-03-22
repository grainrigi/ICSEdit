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
#include "wnd/ControlWindow.h"
#include "wnd/WCanvasControl.h"
#include "wnd/WStaticText.h"
#include "wnd/WDereButton.h"
#include "graphics/MemCanvasRGBA8.h"

namespace ICSE {

	class TestControlWindow : public wnd::ControlWindow {
		std::shared_ptr<wnd::WCanvasControl> m_canvas;
		std::shared_ptr<wnd::WindowLayer> m_mainlayer;
		std::shared_ptr<wnd::WStaticText> m_static, m_static2;
		std::shared_ptr<wnd::WDereButton> m_button;

		int m_mousex, m_mousey;

		graphics::MemCanvasRGBA8 m_memcanvas;

	public:
		TestControlWindow(void);

		void OnEvt(SDL_Event &evt);
		
	private:
		void OnInit(void);
		void OnUpdate(void);
	};

}
