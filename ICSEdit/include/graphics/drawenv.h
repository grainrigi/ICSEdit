#pragma once
#include "graphics/gl/GLShaderSet.h"

namespace ICSE{
namespace graphics{

	class DrawEnv {

		GLuint m_texture;
		GLuint m_program;
	public:
		DrawEnv(void);

		void setTexture(GLuint texture);
		void setShader(const ICSE::graphics::gl::GLShaderSet &shader);
	private:
		//DrawEnv(void);
	};

}
}