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

#include "util/font/FontRenderer.h"
#include "util/font/UTF8Reader.h"

using namespace ICSE::font;
using namespace ICSE::graphics;

#define INT_R(x) (((unsigned char*)&x)[0])
#define INT_G(x) (((unsigned char*)&x)[1])
#define INT_B(x) (((unsigned char*)&x)[2])
#define INT_A(x) (((unsigned char*)&x)[3])

ICSE::font::FontRenderer::FontRenderer(std::shared_ptr<Font> font)
	: m_font{ font },
	m_bakeinfo { 0 },
	m_outline_color { 0xff000000 },
	m_outline_width { 0 },
	m_lineheight { 10 },
	m_linespace { 0 },
	m_font_color { 0xff000000 }
{
	font->GetScaleBakeInfo(m_lineheight, &m_bakeinfo);
}

void ICSE::font::FontRenderer::RenderText(ICSE::graphics::MemCanvasRGBA8 & canvas, const char * utf8txt)
{
	/*BinaryFile fnt = BinaryFile::LoadFromFile("fnt_b.otf");
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
	char* word = u8"お願い！シンデレラ\n夢は夢で終われない\n動き始めてる 輝く日のために";
	int x = 10, y, ascent, descent, lineGap;




	//UTF8Reader ur(word);
	int codepoint1, codepoint2;
	int c_x1, c_y1, c_x2, c_y2, byteOffset, ax, kern;
	int basey = 10;
	int basex = 10;



	while (true) {
		//ur = UTF8Reader(word);
		codepoint2 = ur.readCodePoint();
		scale = stbtt_ScaleForPixelHeight(&info, l_h);
		stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
		ascent *= scale;
		descent *= scale;
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
	}*/

	UTF8Reader ur(utf8txt);
	int codepoint1, codepoint2;
	codepoint2 = ur.readCodePoint();

	Font::BakeInfo binfo = m_bakeinfo;
	int x = 0, y = 0, basex = 0, basey = 0;
	int ascent = std::round((float)m_font->ascent() * binfo.scaleY),
		descent = std::round((float)m_font->descent() * binfo.scaleY);
	int b_w = canvas.width(),
		b_h = canvas.height(),
		l_h = m_lineheight;

	MemCanvasA8 alphamap(b_w, b_h);
	alphamap.ClearWithColor(0);

	//Get Font alpha
	while(true)
	{
		//No space to render
		if (y > canvas.height())
			break;

		//draw one line text
		while(true) 
		{
			codepoint1 = codepoint2;
			codepoint2 = ur.readCodePoint();

			//The line is end
			if (codepoint1 == '\n' || codepoint1 == '\0')
				break;
			//Get bake info
			m_font->GetCodepointBakeInfo(codepoint1, &binfo);

			//Determine the render position and the clipping size
			y = basey + ascent + binfo.bbox.top;
			if (y + binfo.bbox.height() > b_h)
				y = b_h - binfo.bbox.height();
			if (x + binfo.bbox.width() > b_w)
				binfo.bbox.right -= x + binfo.bbox.width() - b_w;

			//Render the font
			m_font->MakeCodepointBitmap(&alphamap, codepoint1, x, y, binfo);

			//Prepare for the next character
			x += m_font->GetCodepointAdvance(codepoint1) * binfo.scaleX;
			x += m_font->GetKernelAdvance(codepoint1, codepoint2) * binfo.scaleX;
			if (x > b_w)
				break;
		}
		//The text is end
		
		if (codepoint1 != '\n')
		{
			while (codepoint1 != '\n' && codepoint1 != '\0')
			{
				codepoint1 = ur.readCodePoint();
			}
			codepoint2 = ur.readCodePoint();
		}
		if (codepoint1 == '\0')
			break;
		

		//Prepare for the nextline
		x = basex;
		basey += m_lineheight + m_linespace;
	}

	//Draw the font
	if(m_outline_width == 0)
	{
		uint8_t fnt_r = INT_R(m_font_color);
		uint8_t fnt_g = INT_G(m_font_color);
		uint8_t fnt_b = INT_B(m_font_color);
		uint8_t fnt_a = INT_A(m_font_color);
		uint32_t *pixels = reinterpret_cast<uint32_t*>(canvas.pixels());
		uint8_t *alpha = reinterpret_cast<uint8_t*>(alphamap.pixels());
		uint32_t out_a, dst_a, src_a;
		uint32_t pixel;
		int stride = canvas.stride() - b_w * 4;
		
		for(int y = 0; y < b_h; y++)
		{
			for(int x = 0; x < b_w; x++, alpha++, pixels++)
			{
				src_a = *alpha * fnt_a;
				dst_a = INT_A(*pixels) * (65535 - src_a);
				src_a <<= 8;
				out_a = src_a + dst_a;
				if (out_a == 0)
					continue;
				INT_R(*pixels) = (fnt_r * src_a + INT_R(*pixels) * dst_a) / out_a;
				INT_G(*pixels) = (fnt_g * src_a + INT_G(*pixels) * dst_a) / out_a;
				INT_B(*pixels) = (fnt_b * src_a + INT_B(*pixels) * dst_a) / out_a;
				INT_A(*pixels) = out_a >> 16;
			}
			pixels += stride;
		}
	}
}

void ICSE::font::FontRenderer::SetFontHeight(int height)
{
	m_lineheight = height; 
	m_font->GetScaleBakeInfo(height, &m_bakeinfo);
}
