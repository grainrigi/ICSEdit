#pragma once
#include "graphics/gl/GLShaderSet.h"

namespace ICSE{
namespace graphics{

	class DrawEnv {
		int m_width;
		int m_height;

		GLuint m_texture;
		GLuint m_program;
	public:
		DrawEnv(void);

		int width(void) const { return m_width; }
		int height(void) const { return m_height; }
		void setWidth(int w) { m_width = w; }
		void setHeight(int h) { m_height = h; }

		void setTexture(GLuint texture);
		void setShader(const ICSE::graphics::gl::GLShaderSet &shader);
	private:
		//DrawEnv(void);
	};

}
}