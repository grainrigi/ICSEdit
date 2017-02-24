#pragma once
#include "util/basic/Singleton.h"
#include "graphics/Mesh2D.h"
#include "graphics/gl/GLShaderSet.h"

namespace ICSE{
namespace graphics {

	class Mesh2DRenderer {
		friend class ICSE::Singleton<Mesh2DRenderer>;

		gl::GLShaderSet m_shader_tex;
		gl::GLShaderSet m_shader_vc;
		gl::GLShaderSet m_shader_ins_gl2_tex;
		gl::GLShaderSet m_shader_ins_gl2_vc;
		gl::GLShaderSet m_shader_ins_gl3_tex;
		gl::GLShaderSet m_shader_ins_gl3_vc;
	public:
		~Mesh2DRenderer(void);

		

		void draw(DrawEnv *env, Mesh2D &mesh);
		void draw(DrawEnv *env, Mesh2D &mesh, const gl::GLShaderSet &shader, const Mesh2D::ShaderAttributes &attr);
	private:
		Mesh2DRenderer(void);

		void initShader(void);

		Mesh2DRenderer(const Mesh2DRenderer&) = delete;
		Mesh2DRenderer& operator=(const Mesh2DRenderer&) = delete;
		Mesh2DRenderer(Mesh2DRenderer&&) = delete;
		Mesh2DRenderer& operator=(Mesh2DRenderer&&) = delete;
	};

}
}