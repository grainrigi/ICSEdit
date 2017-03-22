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
#include "util/basic/Singleton.h"

namespace ICSE {
namespace wnd {

	class WindowManager {
		friend class ICSE::Singleton<WindowManager>;

		std::unordered_map<uint32_t, std::shared_ptr<ICSE::sdl::SDLWindow>> m_windows;
		ICSE::sdl::SDLWindow *m_main = nullptr;
		bool m_quitting = false;
	 public:
		 ~WindowManager(void);

		void RegisterWindow(std::shared_ptr<ICSE::sdl::SDLWindow> window, bool isMain = false);

		template<typename Wnd, typename ...Args>
		std::shared_ptr<Wnd> CreateAndRegisterWindow(Args ...args);

		bool ProcessEvent(void);

		void PostQuitMessage(void);
		
	 private:
		WindowManager(void);
	};

	template<typename Wnd, typename ...Args>
	inline std::shared_ptr<Wnd> WindowManager::CreateAndRegisterWindow(Args ...args)
	{
		std::shared_ptr<Wnd> ptr(new Wnd(args...));
		this->m_windows.insert(std::make_pair(ptr->windowID(), ptr));

		return ptr;
	}

}
}
