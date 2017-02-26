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
#include "graphics/MemCanvasRGBA8.h"
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

ICSE::TestWindow::TestWindow(void)
	: m_width(1024)
	, m_height(768)
	, ICSE::sdl::SDLWindow(u8"ウィンドウ", 1024, 768)
{
	this->OnInit();
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
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

	MemCanvasRGBA8 baked(m_width, m_height);
	baked.ClearWithColor(0x00000000);
	frender.SetFontHeight(56);
	frender.SetFontColor(0xffffffff);
	auto start = std::chrono::high_resolution_clock::now();
	frender.RenderText(baked, u8R"(お願いシンデレラ！
夢は夢で終われない
動き始めてる 輝く日のために
エブリデイどんな時もキュートハート持ってたい
ピンチもサバイバルもクールに超えたい
アップデイト無敵なパッションくじけ心更新
私にできることだけを重ねて
魔法が解けないように
リアルなスキルめぐるミラクル信じてる
お願い！シンデレラ 夢は夢で終われない
叶えるよ星に願いをかけたなら)");

	/*
	stbtt_fontinfo info;
	if (!stbtt_InitFont(&info, fnt.getData(), 0))
		THROW(std::runtime_error, "Font Initialization failed.");

	int b_w = m_width;
	int b_h = m_height;
	int l_h = 36;
	int l_h_s = 10;

	uint8_t *bitmap = new uint8_t[b_w * b_h];
	memset(bitmap, 0, b_w * b_h);

	float scale = stbtt_ScaleForPixelHeight(&info, l_h);
	char* word = u8"AVAW";
	int x = 10, y, ascent, descent, lineGap;




	UTF8Reader ur(word);
	int codepoint1, codepoint2;
	int c_x1, c_y1, c_x2, c_y2, byteOffset, ax, kern;
	int basey = 10;
	int basex = 10;



	while (true) {
		//ur = UTF8Reader(word);
		codepoint2 = ur.readCodePoint();
		scale = stbtt_ScaleForPixelHeight(&info, l_h);
		stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
		ascent = (int)ascent * scale;
		descent = (int)descent * scale;
		while (true)
		{
			codepoint1 = codepoint2;
			if (codepoint1 == 0 || codepoint1 == '\n')
				break;
			codepoint2 = ur.readCodePoint();
			stbtt_GetCodepointBitmapBox(&info, codepoint1, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
			y = basey + ascent + c_y1;
			byteOffset = x + (y * b_w);
			if (x + c_x2 > b_w)
				c_x2 = b_w - x + c_x1;
			if (y + l_h > b_h)
				c_y2 = b_h - y + c_y1;
			stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, codepoint1);
			stbtt_GetCodepointHMetrics(&info, codepoint1, &ax, 0);
			x += ax * scale;
			kern = stbtt_GetCodepointKernAdvance(&info, codepoint1, codepoint2);
			x += kern * scale;
			if (x > b_w)
				break;
		}
		if (codepoint1 == 0)
			break;
		//l_h += l_h_s;
		basey += l_h + l_h_s;

		if (basey > b_h)
			break;
		x = basex;
		//break;
	}

	MemCanvasRGBA8 cbaked(b_w, b_h);
	uint32_t *baked = reinterpret_cast<uint32_t*>(cbaked.pixels());
	for (int i = 0; i < b_w * b_h; i++)
		baked[i] = 0xffc53cfe;
	uint32_t pix = 0x00000000;
	float pixb = 255.0f, pixg = 166.0f, pixr = 166.0f;
	uint32_t pixi = 0x00ffffff;
	//for (int i = 0; i < b_w * b_h; i++)
		//baked[i] = pix | (bitmap[i] << 24);
	int edge = 5;
	int eds = b_w * edge;
	uint32_t ret = 0;
	
	/*for(y = edge; y < b_h - edge; y++)
	{
		for(x = edge; x < b_w - edge; x++)
		{
			ret = pix | bitmap[y * b_w + x] << 24;
			baked[(y - edge) * b_w + (x - edge)] = blend(baked[(y - edge) * b_w + (x - edge)], ret);
			baked[(y - edge) * b_w + (x + edge)] = blend(baked[(y - edge) * b_w + (x + edge)], ret);
			baked[(y + edge) * b_w + (x - edge)] = blend(baked[(y + edge) * b_w + (x - edge)], ret);
			baked[(y + edge) * b_w + (x + edge)] = blend(baked[(y + edge) * b_w + (x + edge)], ret);
		}
	}*/
	//for (int i = 0; i < b_w * b_h; i++)
		//baked[i] = blend(baked[i], pixi | (bitmap[i] << 24));	
		/*
	

	if (edge > 1)
	{
		edge = edge - 1;
		uint8_t alpha = 0;
		float alf = 0.0f;
		uint16_t alp_mean = 0;

		uint8_t *alp_tbl = new uint8_t[(edge * 2 + 1) * (edge * 2 + 1)];
		uint8_t *alp_true_tbl = alp_tbl + (edge * 2 + 1) * edge + edge;
		uint8_t alp_tbl_stride = edge * 2 + 1;
		uint8_t alp_mag = 255 / (edge * edge * 2);
		float fradius = (float)edge + 0.5f;
		float fradius2 = fradius * fradius;
		float fradmin = (fradius - 1.0f) * (fradius - 1.0f);
		float fradmax = (fradius + 1.0f) * (fradius + 1.0f);
		for (int jy = -edge; jy <= edge; jy++)
		{
			for (int jx = -edge; jx <= edge; jx++)
			{
				int dist = jx * jx + jy * jy;
				if (dist < fradmin)
					alp_true_tbl[jx + jy * alp_tbl_stride] = 0xff;
				else if (dist > fradmax)
					alp_true_tbl[jx + jy * alp_tbl_stride] = 0x00;
				else
					alp_true_tbl[jx + jy * alp_tbl_stride] = 255.1f * (1.0f - ((float)dist - fradmin) / (fradmax - fradmin));
			}
		}

		uint32_t pixfull = pix | 0xff000000;
		uint32_t *bakedptr = baked;
		uint8_t *bitmapptr = bitmap;
		uint8_t *alptblptr = alp_true_tbl;
		for (y = edge; y < b_h - edge; y++)
		{
			bakedptr = baked + y * b_w;
			bitmapptr = bitmap + y * b_w;
			for (x = edge; x < b_w - edge; x++)
			{
				alpha = bitmapptr[x];


				//if(alp_mean < 64)
				//alp_mean = alp_mean << 2;
				//alp_mean /= 8;
				/*if(alp_mean < 0.0001f)
					baked[y * b_w + x] = 0x440000f;
				else*/ /*if (alpha == 255)
				{
					//bakedptr[x] = pixi | 0xff000000;
				}/*
				else if (alpha < 255 && alpha > 0)
				{
					blend_direct((uint8_t*)(baked +y * b_w + x), pix, 255 - alpha);

					for (int jy = -edge; jy <= edge; jy++)
					for (int jx = -edge; jx <= edge; jx++)
					baked[(y + jy) * b_w + x + jx] = blend(baked[(y + jy) * b_w + x + jx], pix | (0xff - (jx*jx + jy*jy) * 31) << 24);

				}
			*//*
				else {
					alp_mean =
						//bitmap[(y - 1) * b_w + x - 1] +
						bitmapptr[-b_w + x] +
						//bitmapptr[-b_w + x + 1] +
						bitmapptr[x - 1] +
						bitmapptr[x + 1] +
						//bitmapptr[b_w + x - 1] +
						bitmapptr[b_w + x];
					//bitmap[(y + 1) * b_w + x + 1];

					if (alp_mean == 0) continue;
					if (alp_mean > 255)
						alp_mean = 255;
					if (alp_mean > 1)
					{
						bakedptr = baked + (y - edge) * b_w;
						alptblptr = alp_tbl + edge;
						for (int jy = -edge; jy <= edge; jy++)
						{
							for (int jx = -edge; jx <= edge; jx++)
							{
								//baked[(y + jy) * b_w + x + jx] = blend_exp(baked[(y + jy) * b_w + x + jx], pix, (alp_true_tbl[jx + jy * alp_tbl_stride] * alp_mean >> 8));
								/*if(bitmap[(y + jy) * b_w + jx + x] != 255)
									blend_direct((uint8_t*)(baked +(y + jy) * b_w + x + jx), pix, (alp_true_tbl[jx + jy * alp_tbl_stride] * alp_mean >> 8));*//*
								if (bakedptr[x + jx] != pixfull) {
									if (alptblptr[jx] == 255)
										bakedptr[x + jx] = pixfull;
									else if (alptblptr[jx] > 20)
										blend_direct((uint8_t*)(bakedptr + x + jx), pix, (alptblptr[jx] * alp_mean >> 8));
								}
							}
							bakedptr += b_w;
							alptblptr += alp_tbl_stride;
						}


						//baked[y * b_w + x] = pix | ((int)alp_mean << 24);

					}
				}


				/*
				else
					baked[y * b_w + x] = 0x440000ff;
					*/
					/*
			}
		}
		for (int i = 0; i < b_w * b_h; i++)
		{
			if (bitmap[i] != 0)
				blend_direct((uint8_t*)(baked + i), pixi, (bitmap[i]));
		}
		delete alp_tbl;
	}
	else{
		uint8_t alpha;
		uint8_t *bitmapptr = bitmap;
		uint32_t alp_mean;
		for(y = 1; y < b_h - 1; y++)
		{
			bitmapptr = bitmap + y * b_w;
			for(x = 1; x < b_w - 1; x++)
			{
				alpha = bitmapptr[x];
				if (alpha == 0)
				{
					alp_mean = bitmapptr[-b_w + x];
					alp_mean = alp_mean >= bitmapptr[x - 1] ? alp_mean : bitmapptr[x - 1];
					alp_mean = alp_mean >= bitmapptr[x + 1] ? alp_mean : bitmapptr[x + 1];
					alp_mean = alp_mean >= bitmapptr[b_w + x] ? alp_mean : bitmapptr[b_w + x];
					if (alp_mean > 0)
						blend_direct((uint8_t*)(baked + y * b_w + x), pix, alp_mean);
				}
				else if (alpha == 255)
					baked[y * b_w + x] = pixi | 0xff000000;
				else
				{
					baked[y * b_w + x] = blend_exp(pixi | 0xff000000, pix, 255 - alpha);
				}
			}
		}
	}
	*/
	auto end = std::chrono::high_resolution_clock::now();

	m_shader.use();
	m_texture.init();
	m_texture.uploadImage((char*)baked.pixels(), m_width, m_height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);

	std::ofstream ofs("result.txt");
	ofs << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	ofs << "ns";
	ofs.close();

	//delete bitmap;
	//delete baked;
	//delete alp_tbl;
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
	glClear(GL_COLOR_BUFFER_BIT);

	m_mesh.draw(&env, m_shader, { 0, 1, -1 });
	SDL_GL_SwapWindow(window());
}

void ICSE::TestWindow::OnEvent(SDL_WindowEvent &evt)
{
	if (evt.event == SDL_WINDOWEVENT_CLOSE)
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


