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

#include "ICSE/TestWindow.h"
#include "wnd/WindowManager.h"
#include "util/file/BinaryFile.h"
#include "util/file/ImageFile.h"
#include "util/font/stb_truetype.h"

#include "util/font/UTF8Reader.h"
#include "util/font/FontRenderer.h"

using namespace ICSE;
using ICSE::graphics::gl::GLShaderSet;
using ICSE::graphics::gl::GLVBO;
using ICSE::graphics::gl::GLVBOTarget;
using ICSE::graphics::Mesh2D;
using namespace ICSE::file;
using namespace ICSE::graphics;
using namespace ICSE::graphics::gl;
using namespace ICSE::font;
using namespace ICSE::wnd;

ICSE::TestWindow::TestWindow(void)
	: m_width(1024)
	, m_height(768)
	, ICSE::sdl::SDLWindow(u8"ウィンドウ", 1024, 768)
	, m_baked{nullptr}
	, crend()
	, ctlrend(nullptr)
{
	this->OnInit();
	//crend.initShader();
	crend.LateInitialize(ICSE::Singleton<Mesh2DRenderer>::getInstancePtr());
	ctlrend.LateInitialize();
}




#define INT_R(x) (((const unsigned char*)&x)[0])
#define INT_G(x) (((const unsigned char*)&x)[1])
#define INT_B(x) (((const unsigned char*)&x)[2])
#define INT_A(x) (((const unsigned char*)&x)[3])

/*
#define INT_R(x) (x & 0xFF)
#define INT_G(x) (x >> 8 & 0xFF)
#define INT_B(x) (x >> 16 & 0xFF)
#define INT_A(x) (x >> 24 & 0xFF)
*/
uint32_t blend(uint32_t b, uint32_t a)
{
	int out_a = INT_A(a) + (INT_A(b) * (255 - INT_A(a)) >> 8);
	//if (((unsigned char*)&b)[3] > 0)
	//__asm {int 3};
	if (out_a == 0)
		return 0;
	return (INT_R(a) * INT_A(a) + INT_R(b) * (INT_A(b) * (255 - INT_A(a)) >> 8)) / out_a & 0x000000ff
		| (INT_G(a) * INT_A(a) + INT_G(b) * (INT_A(b) * (255 - INT_A(a)) >> 8)) / out_a << 8 & 0x0000ff00
		| (INT_B(a) * INT_A(a) + INT_B(b) * (INT_A(b) * (255 - INT_A(a)) >> 8)) / out_a << 16 & 0x00ff0000
		| out_a << 24;
}

inline uint32_t blend_exp(uint32_t b, uint32_t a, uint32_t a_alpha)
{
	int a_b = (INT_A(b) * (255 - a_alpha) >> 8);
	int out_a = a_alpha + (INT_A(b) * (255 - a_alpha) >> 8);
	//if (((unsigned char*)&b)[3] > 0)
	//__asm {int 3};
	if (out_a == 0)
		return 0;
	return (INT_R(a) * a_alpha + INT_R(b) * a_b) / out_a & 0x000000ff
		| (INT_G(a) * a_alpha + INT_G(b) * a_b) / out_a << 8 & 0x0000ff00
		| (INT_B(a) * a_alpha + INT_B(b) * a_b) / out_a << 16 & 0x00ff0000
		| out_a << 24;
}

inline uint32_t blend_direct(uint8_t *dest, uint32_t src, uint32_t src_alpha)
{

	//__m64 msrc, mdst, masrc, madst, mao;

	
	int a_b = (dest[3] * (255 - src_alpha) >> 8);
	int out_a = src_alpha + a_b;
	if (out_a == 0)
		return 0;
	//mdst = _mm_setr_pi16(dest[0], dest[1], dest[2], 0);
	//msrc = _mm_setr_pi16(INT_R(src), INT_G(src), INT_B(src), 0);
	//masrc = _mm_setr_pi16(src_alpha, src_alpha, src_alpha, 0);
	//madst = _mm_setr_pi16(a_b, a_b, a_b, 0);
	//mao = _mm_setr_pi16(out_a, out_a, out_a, 0);
	//msrc = _mm_mullo_pi16(msrc, masrc);
	//mdst = _mm_mullo_pi16(mdst, madst);
	//mdst = _mm_add_pi16(mdst, msrc);
	
	dest[0] = (INT_R(src) * src_alpha + dest[0] * a_b) / out_a;
	dest[1] = (INT_G(src) * src_alpha + dest[1] * a_b) / out_a;
	dest[2] = (INT_B(src) * src_alpha + dest[2] * a_b) / out_a;
	dest[3] = out_a;
	
	//dest[0] = mdst.m64_u16[0] / out_a;
	//dest[1] = mdst.m64_u16[1] / out_a;
	//dest[2] = mdst.m64_u16[2] / out_a;
	//dest[3] = out_a;
}



void ICSE::TestWindow::OnInit(void)
{
	std::string vShader{
		//"#version 140\n"
		"attribute vec4 attr_pos;"
		//"in vec4 attr_col;"
		"attribute vec2 attr_uv;"
		"uniform mat4 mat_mv;"
		"varying vec2 vary_uv;"
		"void main() { "
		"  gl_Position = attr_pos * mat_mv;"
		//"  vary_col = attr_col;"
		"  vary_uv = attr_uv;"
		"}" };
	std::string fShader{
		//"#version 140\n"
		"uniform sampler2D unif_tex;"
		"varying vec2 vary_uv;"
		"void main() { "
		//"  gl_FragColor = vary_col;"
		"  gl_FragColor = texture2D(unif_tex, vary_uv);"
		"}" };
	float position[]{
		1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
	};
	float color[]{
		1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
	};
	float uv[]{
		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
	};


	m_context = SDL_GL_CreateContext(window());

	m_shader = GLShaderSet::createFromString(vShader, fShader, { {0, "attr_pos"}, { 1, "attr_uv" } });
	m_unif_matmv = m_shader.getUniformLocation("mat_mv");
	glUniform1i(m_shader.getUniformLocation("unif_tex"), 0);

	glViewport(0, 0, m_width, m_height);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	m_mesh = ICSE::graphics::Mesh2D(4, Mesh2D::ATTR_POSITION | Mesh2D::ATTR_COORD, GL_STATIC_DRAW);
	memcpy(&(*m_mesh.positions())[0], position, sizeof(position));
	//memcpy(&(*m_mesh.colors())[0], color, sizeof(color));
	memcpy(&(*m_mesh.coordinates())[0], uv, sizeof(uv));

	m_renderer.reset(new ICSE::graphics::Mesh2DRenderer());

	ICSE::file::BinaryFile png = ICSE::file::BinaryFile::LoadFromFile("live_atlas_liveui.png");
	ICSE::file::ImageFile img = ICSE::file::ImageFile::LoadFromFile(png, ICSE::file::ImageFile::FORMAT_RGBA);
	BinaryFile bfnt = BinaryFile::LoadFromFile("fnt_b.otf");
	FontRenderer frender(std::shared_ptr<Font>(new Font(Font::LoadFromFile(bfnt))));

	m_baked.reset(new MemCanvasRGBA8(1024, 768));
	//m_canvas.reset(new WCanvasControl(1024, 768, *ctlrend.GetPool()));
	for(int i = 0; i < 100; i++)
		ctlrend.AddCanvasControl(m_canvas);
	MemCanvasRGBA8 *baked = m_canvas->Lock();
	baked->ClearWithColor(0xffc53cfe);
	frender.SetFontHeight(70);
	frender.SetFontColor(0xffffffff);
	frender.SetOutlineWidth(8);
	auto start = std::chrono::high_resolution_clock::now();

	
	frender.RenderText(*baked, u8R"(お願いシンデレラ！
夢は夢で終われない
動き始めてる 輝く日のために
エブリデイどんな時もキュートハート持ってたい
ピンチもサバイバルもクールに超えたい
アップデイト無敵なパッションくじけ心更新
私にできることだけを重ねて
魔法が解けないように
リアルなスキルめぐるミラクル信じてる
お願い！シンデレラ 夢は夢で終われない
叶えるよ星に願いをかけたなら
見つけようMy Only Star)", 20, 20, 255);
	//frender.RenderText(baked, u8"決定\n", 20, 20, 255);


	auto end = std::chrono::high_resolution_clock::now();

	m_shader.use();
	
	memcpy(m_baked->pixels(), baked->pixels(), 1024 * 768 * 4);

	std::ofstream ofs("result.txt");
	ofs << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	ofs << "ns";
	ofs.close();

	//delete bitmap;
	//delete baked;
	//delete alp_tbl;

	m_canvas->Unlock();

	std::shared_ptr<int> ptr{ new int(0) };
	std::weak_ptr<int> wptr = ptr;
	wptr.reset();
}

void ICSE::TestWindow::Update(void)
{
	static float arg = 0.0f;

	checkFPS();

	//arg += 0.02f;
	Matrix4<float> mat_mv, mat_id;

	m_texture.bind();

	mat_mv = Matrix4<float>::createRotationAroundAxis(0.0f, arg * 57.3f, 0.0f);
	mat_id.identity();

	glUniformMatrix4fv(m_unif_matmv, 1, false, mat_mv.data);

	SDL_GL_MakeCurrent(window(), m_context);
	ICSE::graphics::DrawEnv env;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	wnd::BoundingBox bb;
	bb.SetX(mouse_x - 125);
	bb.SetY(mouse_y - 125);
	bb.SetWidth(1024);
	bb.SetHeight(768);
	m_canvas->Resize(mouse_x + 10, mouse_y + 10);
	auto cvs = m_canvas->Lock();
	for (int i = 0; i < mouse_y; i++)
		memcpy(cvs->pixelAt(0, i), m_baked->pixelAt(0, i), mouse_x * 4);
	m_canvas->Unlock();
	env.setWidth(m_width);
	env.setHeight(m_height);

	//if(!m_mesh.isUploaded())
		//m_mesh.upload();
	//m_renderer->draw(&env, m_mesh);
	auto start = std::chrono::high_resolution_clock::now();
	ctlrend.RenderAll(&env);

	auto end = std::chrono::high_resolution_clock::now();
	volatile int cnt = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	std::cout << cnt << std::endl;
	SDL_GL_SwapWindow(window());
}

void ICSE::TestWindow::OnEvent(SDL_Event &evt)
{
	if (evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE)
		ICSE::Singleton<ICSE::wnd::WindowManager>::getInstance().PostQuitMessage();
}

void ICSE::TestWindow::checkFPS(void)
{
	using namespace std::chrono;
	static time_point<steady_clock> base = high_resolution_clock::now();
	char buf[32];

	static int frame = 0;

	frame++;

	auto now = high_resolution_clock::now();
	if(duration_cast<milliseconds>(now - base).count() > 200)
	{
		float fps = ((float)frame * 1000000000.0f) / (float)(duration_cast<nanoseconds>(now - base).count());
		sprintf(buf, "FPS:%.2lf", std::round(fps * 100.0f) / 100.0f);
		SDL_SetWindowTitle(window(), buf);

		frame = 0;
		base = now;
	}
}


