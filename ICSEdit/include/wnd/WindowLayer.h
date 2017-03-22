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
#include "graphics/drawenv.h"
#include "wnd/AWSizableControl.h"
#include "wnd/ControlRenderer.h"

namespace ICSE {
namespace wnd {
	class ControlWindow;

	class WindowLayer {
		ControlWindow *m_parent;
		uint32_t m_id;

		ControlRenderer m_ascent;
		ControlRenderer m_descent;

		std::unordered_map<uint32_t, std::shared_ptr<AWControl>> m_controls;
		std::unordered_map<uint32_t, std::shared_ptr<AWCanvasControl>> m_ascentcontrols;
		std::unordered_map<uint32_t, std::shared_ptr<AWCanvasControl>> m_descentcontrols;
	public:
		WindowLayer(ControlWindow * parent, ControlRendererShader *shader);

		template<typename Control, typename ...Args>
		std::shared_ptr<Control> CreateControl(Args ...args);

		template<typename Control, typename ...Args>
		std::shared_ptr<Control> CreateAscentCanvasControl(Args ...args);
		template<typename Control, typename ...Args>
		std::shared_ptr<Control> CreateDescentCanvasControl(Args ...args);

		void RemoveControl(uint32_t id);

		void UpdateAll(void);
		void RenderAll(graphics::DrawEnv *env);

		bool ProcessEvent(SDL_Event &evt);

		uint32_t id(void) const { return m_id; }
		
	private:
		static uint32_t ObtainID(void);
	};

#include "wnd/WindowLayerImpl.h"

}
}
