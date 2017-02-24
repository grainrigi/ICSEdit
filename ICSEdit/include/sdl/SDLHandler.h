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