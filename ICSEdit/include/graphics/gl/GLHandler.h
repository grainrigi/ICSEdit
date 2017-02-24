#pragma once
#include "util/basic/Singleton.h"
#include "graphics/gl/GLInitWindow.h"

namespace ICSE {
namespace graphics {
namespace gl {

	class GLHandler {
		friend class Singleton<GLHandler>;

		GLInitWindow m_window;

	 public:
		
		
	 private:
		 GLHandler(void);
		 void initGL(void);
	};

}
}
}
