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
#include "util/basic/Singleton.h"

namespace ICSE {
namespace sdl{

	class SDLHandler {
		/*
		//only for singleton
		friend class ICSE::Singleton<SDLHandler>;
		*/

		bool m_isInit;

	public:
		SDLHandler(void);
		~SDLHandler(void);

		SDLHandler(const SDLHandler&) = delete;
		SDLHandler& operator=(const SDLHandler&) = delete;
		SDLHandler(SDLHandler&&) = delete;
		SDLHandler& operator=(SDLHandler&&) = delete;
	private:

		bool initVideo(void);
	};

}
}