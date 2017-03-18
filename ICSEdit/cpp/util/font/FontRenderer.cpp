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
#define SHORT_L(x) (((unsigned char*)&x)[0])
#define SHORT_A(x) (((unsigned char*)&x)[1])

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

void ICSE::font::FontRenderer::RenderText(ICSE::graphics::MemCanvasRGBA8 & canvas, const char * utf8txt, int topx, int topy, int opaqueness)
{
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
	int r_x = 0, r_y = 0, r_w = 0, r_h = 0;
	basex = r_x = topx + m_outline_width; basey = r_y = topy + m_outline_width;
	x = basex;

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
				binfo.bbox.bottom -= y + binfo.bbox.height() - b_h;
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
		r_w = r_w > x ? r_w : x;
		r_w = r_w > b_w ? b_w : r_w;
		x = basex;
		basey += m_lineheight + m_linespace;
		if (basey >= b_h)
			break;
	}
	r_w -= r_x;
	r_h = y + m_lineheight;
	r_h = r_h > b_h ? b_h : r_h;
	r_h -= r_y;


	//Draw the font
	uint8_t fnt_r = INT_R(m_font_color);
	uint8_t fnt_g = INT_G(m_font_color);
	uint8_t fnt_b = INT_B(m_font_color);
	uint8_t fnt_a = opaqueness;
	uint32_t fnt_pm_r = fnt_r * fnt_a;
	uint32_t fnt_pm_g = fnt_g * fnt_a;
	uint32_t fnt_pm_b = fnt_b * fnt_a;
	uint8_t ol_r = INT_R(m_outline_color);
	uint8_t ol_g = INT_G(m_outline_color);
	uint8_t ol_b = INT_B(m_outline_color);
	uint8_t ol_a = opaqueness;
	if(m_outline_width == 0)
	{
		
		uint32_t *pixels = reinterpret_cast<uint32_t*>(canvas.pixels());
		uint8_t *alpha = reinterpret_cast<uint8_t*>(alphamap.pixels());
		uint32_t out_a, dst_a, src_a;
		uint32_t pixel, pixfull = m_font_color;
		int stride = (canvas.stride() >> 2) - r_w;
		int astride = b_w - r_w;
		
		pixels += r_x + (canvas.stride() >> 2) * r_y;
		alpha += r_x + b_w * r_y;
		if (fnt_a == 255) {
			for (y = r_y; y < r_h + r_y; y++)
			{
				for (x = r_x; x < r_w + r_x; x++, alpha++, pixels++)
				{
					if (*alpha == 255)
						*pixels = pixfull;
					else {
						src_a = fnt_a * *alpha;
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
				}
				pixels += stride;
				alpha += astride;
			}
		}
		else {
			for (y = r_y; y < r_h; y++)
			{
				for (x = r_x; x < r_w; x++, alpha++, pixels++)
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
				alpha += astride;
			}
		}
	}
	else if(m_outline_width == 1)
	{
		//expand the edge if possible
		r_x--; r_y--; r_w++; r_h++;
		if (r_x <= 0) r_x = 1;
		if (r_y <= 0) r_y = 1;
		if (r_w + r_x >= b_w) r_w = b_w - r_x;
		if (r_h + r_y >= b_h) r_h = b_h - r_y;
		uint32_t *pixels = reinterpret_cast<uint32_t*>(canvas.pixels());
		uint8_t *alpha = reinterpret_cast<uint8_t*>(alphamap.pixels());

		std::unique_ptr<uint8_t> ucoltbl{ new uint8_t[256 * 3] };
		uint8_t *coltbl_r = ucoltbl.get();
		uint8_t *coltbl_g = ucoltbl.get() + 256;
		uint8_t *coltbl_b = ucoltbl.get() + 512;

		uint32_t out_a, dst_a, src_a;
		uint32_t pixel, alp_mean;
		uint32_t fnt_full = m_font_color | 0xff000000;
		int stride = (canvas.stride() >> 2) - r_w;
		int realstride = canvas.stride();

		alpha += r_x + (canvas.stride() >> 2) * r_y;
		
		
		for(int i = 0; i < 256; i++)
		{
			src_a = i;
			dst_a = 255 - i;
			coltbl_r[i] = (fnt_r * src_a + ol_r * dst_a) / 255;
			coltbl_g[i] = (fnt_g * src_a + ol_g * dst_a) / 255;
			coltbl_b[i] = (fnt_b * src_a + ol_b * dst_a) / 255;
		}

		pixels = (uint32_t*)canvas.pixels() + r_x + (canvas.stride() >> 2) * r_y;

		if(opaqueness == 255)
		{
			for (y = r_y; y < r_h + r_y; y++)
			{
				for (x = r_x; x < r_w + r_x; x++, pixels++, alpha++)
				{
					if (*alpha == 0)
					{
						alp_mean = alpha[-b_w];
						alp_mean = alp_mean >= alpha[-1] ? alp_mean : alpha[-1];
						alp_mean = alp_mean >= alpha[1] ? alp_mean : alpha[1];
						alp_mean = alp_mean >= alpha[b_w] ? alp_mean : alpha[b_w];
						if (alp_mean == 0)
							continue;
						src_a = alp_mean * 256;
						dst_a = INT_A(*pixels) * (65535 - src_a);
						src_a <<= 8;
						out_a = src_a + dst_a;
						if (out_a == 0)
							continue;
						INT_R(*pixels) = (ol_r * src_a + INT_R(*pixels) * dst_a) / out_a;
						INT_G(*pixels) = (ol_g * src_a + INT_G(*pixels) * dst_a) / out_a;
						INT_B(*pixels) = (ol_b * src_a + INT_B(*pixels) * dst_a) / out_a;
						INT_A(*pixels) = out_a >> 16;
					}
					else if (*alpha == 255)
					{
						*pixels = fnt_full;
					}
					else
					{
						INT_R(*pixels) = coltbl_r[*alpha];
						INT_G(*pixels) = coltbl_g[*alpha];
						INT_B(*pixels) = coltbl_b[*alpha];
						INT_A(*pixels) = 255;
					}
				}
				pixels += stride;
				alpha += stride;
			}
		}
		else
		{

			for (y = r_y; y < r_h + r_y; y++)
			{
				for (x = r_x; x < r_w + r_x; x++, pixels++, alpha++)
				{
					if (*alpha == 0)
					{
						alp_mean = alpha[-b_w];
						alp_mean = alp_mean >= alpha[-1] ? alp_mean : alpha[-1];
						alp_mean = alp_mean >= alpha[1] ? alp_mean : alpha[1];
						alp_mean = alp_mean >= alpha[b_w] ? alp_mean : alpha[b_w];
						if (alp_mean == 0)
							continue;
						src_a = alp_mean * ol_a;
						dst_a = INT_A(*pixels) * (65535 - src_a);
						src_a <<= 8;
						out_a = src_a + dst_a;
						if (out_a == 0)
							continue;
						INT_R(*pixels) = (ol_r * src_a + INT_R(*pixels) * dst_a) / out_a;
						INT_G(*pixels) = (ol_g * src_a + INT_G(*pixels) * dst_a) / out_a;
						INT_B(*pixels) = (ol_b * src_a + INT_B(*pixels) * dst_a) / out_a;
						INT_A(*pixels) = out_a >> 16;
					}
					else if (*alpha == 255)
					{
						src_a = 255 * fnt_a;
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
					else
					{
						src_a = 255 * opaqueness;
						dst_a = INT_A(*pixels) * (65535 - src_a);
						src_a <<= 8;
						out_a = src_a + dst_a;
						if (out_a == 0)
							continue;
						INT_R(*pixels) = (coltbl_r[*alpha] * src_a + INT_R(*pixels) * dst_a) / out_a;
						INT_G(*pixels) = (coltbl_g[*alpha] * src_a + INT_G(*pixels) * dst_a) / out_a;
						INT_B(*pixels) = (coltbl_b[*alpha] * src_a + INT_B(*pixels) * dst_a) / out_a;
						INT_A(*pixels) = out_a >> 16;
					}
				}
				pixels += stride;
				alpha += stride;
			}
		}
	}
	else
	{
		int edge = m_outline_width;
		//expand the edge if possible
		r_x-=edge; r_y-=edge; r_w+=edge + 1; r_h+=edge;
		if (r_x <= 0) r_x = 0;
		if (r_y <= 0) r_y = 0;
		if (r_w + r_x >= b_w) r_w = b_w - r_x;
		if (r_h + r_y >= b_h) r_h = b_h - r_y;
		edge--;

		//Build Edge arrays
		uint8_t *alp_tbl = new uint8_t[(edge * 2 + 1) * (edge * 2 + 1)];
		uint8_t *alp_true_tbl = alp_tbl + (edge * 2 + 1) * edge + edge;
		uint8_t alp_tbl_stride = edge * 2 + 1;
		uint8_t alp_mag = 255 / (edge * edge * 2);
		uint32_t fullcount = 0;
		uint32_t half_count = 0;
		float fradius = (float)edge + 0.5f;
		float fradius2 = fradius * fradius;
		float fradmin = (fradius - 1.0f) * (fradius - 1.0f);
		float fradmax = (fradius + 1.0f) * (fradius + 1.0f);
		for (int jy = -edge; jy <= edge; jy++)
		{
			for (int jx = -edge; jx <= edge; jx++)
			{
				int dist = jx * jx + jy * jy;
				if (dist < fradmin) {
					alp_true_tbl[jx + jy * alp_tbl_stride] = 0xff;
					fullcount++;
				}
				else if (dist > fradmax)
					alp_true_tbl[jx + jy * alp_tbl_stride] = 0x00;
				else {
					alp_true_tbl[jx + jy * alp_tbl_stride] = 255.1f * (1.0f - ((float)dist - fradmin) / (fradmax - fradmin));
					half_count++;
				}
			}
		}

		uint32_t alp_mean;
		uint32_t out_a, dst_a, src_a;

		uint8_t *alp_tbl_ptr = alp_true_tbl;
		std::unique_ptr<uint8_t> ualp_sub_tbl{ new uint8_t[half_count] };
		uint8_t *alp_sub_tbl = ualp_sub_tbl.get();
		std::unique_ptr<int32_t> ualp_diff_tbl{ new int32_t[(edge * 2 + 1) * (edge * 2 + 1)] };
		int32_t *alp_diff_tbl = ualp_diff_tbl.get();
		std::unique_ptr<uint16_t> uoutline{ new uint16_t[r_w * r_h] };
		uint16_t *outline = uoutline.get();

		int stride = (canvas.stride() >> 2) - r_w;
		int realstride = canvas.stride() >> 2;

		uint32_t *pixels_sub_ptr = 0;
		uint32_t edge_stride = realstride - (edge * 2 + 1);
		uint32_t outline_edge_stride = r_w - (edge * 2 + 1);

		{
			int32_t diff = -edge * r_w - edge, sub_diff = -edge * r_w - edge;
			int32_t *alp_diff_sub_tbl = alp_diff_tbl + fullcount;

			for(int jy = -edge; jy <= edge; jy++)
			{
				for(int jx = -edge; jx <= edge; jx++)
				{
					if (alp_true_tbl[jx + jy * alp_tbl_stride] == 0xff)
					{
						*alp_diff_tbl = diff;
						diff = 0;
						alp_diff_tbl++;
						//sub_diff++;
					}
					else if(alp_true_tbl[jx + jy*alp_tbl_stride])
					{
						*alp_diff_sub_tbl = sub_diff;
						*alp_sub_tbl = alp_true_tbl[jx + jy*alp_tbl_stride];
						sub_diff = 0;
						alp_sub_tbl++;
						alp_diff_sub_tbl++;
						//diff++;
					}
					diff++;
					sub_diff++;
					
				}
				diff += outline_edge_stride;
				sub_diff += outline_edge_stride;
			}

			assert(ualp_diff_tbl.get() + fullcount == alp_diff_tbl);
			assert(ualp_diff_tbl.get() + fullcount + half_count == alp_diff_sub_tbl);
		}

		uint32_t *pixels = reinterpret_cast<uint32_t*>(canvas.pixels());
		uint8_t *alpha = reinterpret_cast<uint8_t*>(alphamap.pixels());
		alpha += r_x + edge + b_w * (r_y + edge);


		uint16_t *outline_ptr = outline;
		outline += edge + r_w * (edge);
		alp_diff_tbl = ualp_diff_tbl.get();
		int32_t *alp_diff_tbl_ptr = alp_diff_tbl;
		alp_sub_tbl = ualp_sub_tbl.get();
		uint8_t *alp_sub_tbl_ptr = alp_sub_tbl;
		memset(uoutline.get(), 0, r_w * r_h * 2);
		int32_t *alp_diff_tbl_limit_full = alp_diff_tbl + fullcount;
		int32_t *alp_diff_tbl_limit_half = alp_diff_tbl_limit_full + half_count;
		

		for (y = r_y + edge; y < r_h + r_y - edge; y++)
		{
			for (x = r_x + edge; x < r_w + r_x - edge; x++, alpha++, outline++)
			{
				if (*alpha == 0)
				{
					alp_mean = alpha[-b_w] + alpha[-1] + alpha[1] + alpha[b_w];
					if (alp_mean == 0) continue;
					//if (alp_mean > 255)
						//alp_mean = 255;
					if(alp_mean > 1)
					{
						outline_ptr = outline;
						alp_diff_tbl_ptr = alp_diff_tbl;
						for(;alp_diff_tbl_ptr < alp_diff_tbl_limit_full;alp_diff_tbl_ptr++)
						{
							outline_ptr += *alp_diff_tbl_ptr;
							SHORT_A(*outline_ptr) = 0xff;
						}
						outline_ptr = outline;
						alp_sub_tbl_ptr = alp_sub_tbl;
						for (;alp_diff_tbl_ptr < alp_diff_tbl_limit_half; alp_diff_tbl_ptr++, alp_sub_tbl_ptr++)
						{
							outline_ptr += *alp_diff_tbl_ptr;
							if (SHORT_A(*outline_ptr) != 0xff)
							{
								SHORT_A(*outline_ptr) = ((*alp_sub_tbl_ptr << 8) + SHORT_A(*outline_ptr) * (255 - *alp_sub_tbl_ptr)) >> 8;
							}
						}
					}
				}
				else {
					*outline = 0xff00 | *alpha;
				}
			}
			alpha += stride + edge * 2;
			outline += edge * 2;
		}
		std::unique_ptr<uint8_t> ucoltbl{ new uint8_t[256 * 3] };
		uint8_t *coltbl_r = ucoltbl.get();
		uint8_t *coltbl_g = ucoltbl.get() + 256;
		uint8_t *coltbl_b = ucoltbl.get() + 512;

		for (int i = 0; i < 256; i++)
		{
			src_a = i;
			dst_a = 255 - i;
			coltbl_r[i] = (fnt_r * src_a + ol_r * dst_a) / 255;
			coltbl_g[i] = (fnt_g * src_a + ol_g * dst_a) / 255;
			coltbl_b[i] = (fnt_b * src_a + ol_b * dst_a) / 255;
		}

		outline = uoutline.get();
		pixels = (uint32_t*)canvas.pixels() + r_x + (canvas.stride() >> 2) * r_y;
		stride = (canvas.stride() >> 2) - r_w;

		for (y = r_y; y < r_h + r_y; y++)
		{
			for (x = r_x; x < r_w + r_x; x++, pixels++, outline++)
			{
				if (SHORT_L(*outline) == 255)
				{
					src_a = 255 * fnt_a;
					dst_a = INT_A(*pixels) * (65535 - src_a);
					src_a <<= 8;
					out_a = src_a + dst_a;
					if (out_a == 0)
						continue;
					INT_R(*pixels) = (fnt_r * src_a + INT_R(*pixels) * dst_a) >> 24;
					INT_G(*pixels) = (fnt_g * src_a + INT_G(*pixels) * dst_a) >> 24;
					INT_B(*pixels) = (fnt_b * src_a + INT_B(*pixels) * dst_a) >> 24;
					INT_A(*pixels) = out_a >> 16;

				}
				else if (SHORT_L(*outline) == 0)
				{
					src_a = SHORT_A(*outline) * ol_a;
					dst_a = INT_A(*pixels) * (65535 - src_a);
					src_a <<= 8;
					out_a = src_a + dst_a;
					if (out_a == 0)
						continue;
					INT_R(*pixels) = (ol_r * src_a + INT_R(*pixels) * dst_a) / out_a;
					INT_G(*pixels) = (ol_g * src_a + INT_G(*pixels) * dst_a) / out_a;
					INT_B(*pixels) = (ol_b * src_a + INT_B(*pixels) * dst_a) / out_a;
					INT_A(*pixels) = out_a >> 16;

				}
				else
				{
					src_a = 255 * opaqueness;
					dst_a = INT_A(*pixels) * (65535 - src_a);
					src_a <<= 8;
					out_a = src_a + dst_a;
					if (out_a == 0)
						continue;
					INT_R(*pixels) = (coltbl_r[SHORT_L(*outline)] * src_a + INT_R(*pixels) * dst_a) >> 24;
					INT_G(*pixels) = (coltbl_g[SHORT_L(*outline)] * src_a + INT_G(*pixels) * dst_a) >> 24;
					INT_B(*pixels) = (coltbl_b[SHORT_L(*outline)] * src_a + INT_B(*pixels) * dst_a) >> 24;
					INT_A(*pixels) = out_a >> 16;
				}
			}
			pixels += stride;
			//outline += r_x;
		}
	}
}

void ICSE::font::FontRenderer::BakeText(ICSE::graphics::MemCanvasRGBA8 & canvas, const char * utf8txt, int topx, int topy, int opaqueness)
{
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
	int r_x = 0, r_y = 0, r_w = 0, r_h = 0;
	basex = r_x = topx + m_outline_width; basey = r_y = topy + m_outline_width;
	x = basex;

	MemCanvasA8 alphamap(b_w, b_h);
	alphamap.ClearWithColor(0);

	//Get Font alpha
	while (true)
	{
		//No space to render
		if (y > canvas.height())
			break;

		//draw one line text
		while (true)
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
				binfo.bbox.bottom -= y + binfo.bbox.height() - b_h;
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
		r_w = r_w > x ? r_w : x;
		r_w = r_w > b_w ? b_w : r_w;
		x = basex;
		basey += m_lineheight + m_linespace;
		if (basey >= b_h)
			break;
	}
	r_w -= r_x;
	r_h = y + m_lineheight;
	r_h = r_h > b_h ? b_h : r_h;
	r_h -= r_y;


	//Draw the font
	uint8_t fnt_r = INT_R(m_font_color);
	uint8_t fnt_g = INT_G(m_font_color);
	uint8_t fnt_b = INT_B(m_font_color);
	uint8_t fnt_a = opaqueness;
	uint32_t fnt_pm_r = fnt_r * fnt_a;
	uint32_t fnt_pm_g = fnt_g * fnt_a;
	uint32_t fnt_pm_b = fnt_b * fnt_a;
	uint8_t ol_r = INT_R(m_outline_color);
	uint8_t ol_g = INT_G(m_outline_color);
	uint8_t ol_b = INT_B(m_outline_color);
	uint8_t ol_a = opaqueness;
	if (m_outline_width == 0)
	{

		uint32_t *pixels = reinterpret_cast<uint32_t*>(canvas.pixels());
		uint8_t *alpha = reinterpret_cast<uint8_t*>(alphamap.pixels());
		uint32_t out_a, dst_a, src_a;
		uint32_t pixel, pixfull = m_font_color;
		int stride = (canvas.stride() >> 2) - r_w;
		int astride = b_w - r_w;

		pixels += r_x + (canvas.stride() >> 2) * r_y;
		alpha += r_x + b_w * r_y;
		if (fnt_a == 255) {
			for (y = r_y; y < r_h + r_y; y++)
			{
				for (x = r_x; x < r_w + r_x; x++, alpha++, pixels++)
				{
					if (*alpha == 255)
						*pixels = pixfull;
					else {
						if (*alpha == 0)
							continue;
						src_a = fnt_a * *alpha >> 8;
						INT_R(*pixels) = fnt_r;
						INT_G(*pixels) = fnt_g;
						INT_B(*pixels) = fnt_b;
						INT_A(*pixels) = src_a;
					}
				}
				pixels += stride;
				alpha += astride;
			}
		}
		else {
			for (y = r_y; y < r_h; y++)
			{
				for (x = r_x; x < r_w; x++, alpha++, pixels++)
				{
					if (*alpha == 0)
						continue;
					src_a = *alpha * fnt_a >> 8;
					INT_R(*pixels) = fnt_r;
					INT_G(*pixels) = fnt_g;
					INT_B(*pixels) = fnt_b;
					INT_A(*pixels) = src_a;
				}
				pixels += stride;
				alpha += astride;
			}
		}
	}
	else if (m_outline_width == 1)
	{
		//expand the edge if possible
		r_x--; r_y--; r_w++; r_h++;
		if (r_x <= 0) r_x = 0;
		if (r_y <= 0) r_y = 0;
		if (r_w + r_x >= b_w) r_w = b_w - r_x;
		if (r_h + r_y >= b_h) r_h = b_h - r_y;
		uint32_t *pixels = reinterpret_cast<uint32_t*>(canvas.pixels());
		uint8_t *alpha = reinterpret_cast<uint8_t*>(alphamap.pixels());

		std::unique_ptr<uint8_t> ucoltbl{ new uint8_t[256 * 3] };
		uint8_t *coltbl_r = ucoltbl.get();
		uint8_t *coltbl_g = ucoltbl.get() + 256;
		uint8_t *coltbl_b = ucoltbl.get() + 512;

		uint32_t out_a, dst_a, src_a;
		uint32_t pixel, alp_mean;
		uint32_t fnt_full = m_font_color | 0xff000000;
		int stride = (canvas.stride() >> 2) - r_w;
		int realstride = canvas.stride();

		alpha += r_x + (canvas.stride() >> 2) * r_y;


		for (int i = 0; i < 256; i++)
		{
			src_a = i;
			dst_a = 255 - i;
			coltbl_r[i] = (fnt_r * src_a + ol_r * dst_a) / 255;
			coltbl_g[i] = (fnt_g * src_a + ol_g * dst_a) / 255;
			coltbl_b[i] = (fnt_b * src_a + ol_b * dst_a) / 255;
		}

		pixels = (uint32_t*)canvas.pixels() + r_x + (canvas.stride() >> 2) * r_y;

		if (opaqueness == 255)
		{
			for (y = r_y; y < r_h + r_y; y++)
			{
				for (x = r_x; x < r_w + r_x; x++, pixels++, alpha++)
				{
					if (*alpha == 0)
					{
						alp_mean = alpha[-b_w];
						alp_mean = alp_mean >= alpha[-1] ? alp_mean : alpha[-1];
						alp_mean = alp_mean >= alpha[1] ? alp_mean : alpha[1];
						alp_mean = alp_mean >= alpha[b_w] ? alp_mean : alpha[b_w];
						if (alp_mean == 0)
							continue;
						src_a = alp_mean;
						INT_R(*pixels) = ol_r;
						INT_G(*pixels) = ol_g;
						INT_B(*pixels) = ol_b;
						INT_A(*pixels) = alp_mean;
					}
					else if (*alpha == 255)
					{
						*pixels = fnt_full;
					}
					else
					{
						INT_R(*pixels) = coltbl_r[*alpha];
						INT_G(*pixels) = coltbl_g[*alpha];
						INT_B(*pixels) = coltbl_b[*alpha];
						INT_A(*pixels) = 255;
					}
				}
				pixels += stride;
				alpha += stride;
			}
		}
		else
		{

			for (y = r_y; y < r_h + r_y; y++)
			{
				for (x = r_x; x < r_w + r_x; x++, pixels++, alpha++)
				{
					if (*alpha == 0)
					{
						alp_mean = alpha[-b_w];
						alp_mean = alp_mean >= alpha[-1] ? alp_mean : alpha[-1];
						alp_mean = alp_mean >= alpha[1] ? alp_mean : alpha[1];
						alp_mean = alp_mean >= alpha[b_w] ? alp_mean : alpha[b_w];
						if (alp_mean == 0)
							continue;
						src_a = alp_mean * ol_a >> 8;
						INT_R(*pixels) = ol_r;
						INT_G(*pixels) = ol_g;
						INT_B(*pixels) = ol_b;
						INT_A(*pixels) = src_a;
					}
					else if (*alpha == 255)
					{
						INT_R(*pixels) = fnt_r;
						INT_G(*pixels) = fnt_g;
						INT_B(*pixels) = fnt_b;
						INT_A(*pixels) = fnt_a;
					}
					else
					{
						INT_R(*pixels) = coltbl_r[*alpha];
						INT_G(*pixels) = coltbl_g[*alpha];
						INT_B(*pixels) = coltbl_b[*alpha];
						INT_A(*pixels) = opaqueness;
					}
				}
				pixels += stride;
				alpha += stride;
			}
		}
	}
	else
	{
		int edge = m_outline_width;
		//expand the edge if possible
		r_x -= edge; r_y -= edge; r_w += edge + 1; r_h += edge;
		if (r_x <= 0) r_x = 0;
		if (r_y <= 0) r_y = 0;
		if (r_w + r_x >= b_w) r_w = b_w - r_x;
		if (r_h + r_y >= b_h) r_h = b_h - r_y;
		edge--;

		//Build Edge arrays
		uint8_t *alp_tbl = new uint8_t[(edge * 2 + 1) * (edge * 2 + 1)];
		uint8_t *alp_true_tbl = alp_tbl + (edge * 2 + 1) * edge + edge;
		uint8_t alp_tbl_stride = edge * 2 + 1;
		uint8_t alp_mag = 255 / (edge * edge * 2);
		uint32_t fullcount = 0;
		uint32_t half_count = 0;
		float fradius = (float)edge + 0.5f;
		float fradius2 = fradius * fradius;
		float fradmin = (fradius - 1.0f) * (fradius - 1.0f);
		float fradmax = (fradius + 1.0f) * (fradius + 1.0f);
		for (int jy = -edge; jy <= edge; jy++)
		{
			for (int jx = -edge; jx <= edge; jx++)
			{
				int dist = jx * jx + jy * jy;
				if (dist < fradmin) {
					alp_true_tbl[jx + jy * alp_tbl_stride] = 0xff;
					fullcount++;
				}
				else if (dist > fradmax)
					alp_true_tbl[jx + jy * alp_tbl_stride] = 0x00;
				else {
					alp_true_tbl[jx + jy * alp_tbl_stride] = 255.1f * (1.0f - ((float)dist - fradmin) / (fradmax - fradmin));
					half_count++;
				}
			}
		}

		uint32_t alp_mean;
		uint32_t out_a, dst_a, src_a;

		uint8_t *alp_tbl_ptr = alp_true_tbl;
		std::unique_ptr<uint8_t> ualp_sub_tbl{ new uint8_t[half_count] };
		uint8_t *alp_sub_tbl = ualp_sub_tbl.get();
		std::unique_ptr<int32_t> ualp_diff_tbl{ new int32_t[(edge * 2 + 1) * (edge * 2 + 1)] };
		int32_t *alp_diff_tbl = ualp_diff_tbl.get();
		std::unique_ptr<uint16_t> uoutline{ new uint16_t[r_w * r_h] };
		uint16_t *outline = uoutline.get();

		int stride = (canvas.stride() >> 2) - r_w;
		int realstride = canvas.stride() >> 2;

		uint32_t *pixels_sub_ptr = 0;
		uint32_t edge_stride = realstride - (edge * 2 + 1);
		uint32_t outline_edge_stride = r_w - (edge * 2 + 1);

		{
			int32_t diff = -edge * r_w - edge, sub_diff = -edge * r_w - edge;
			int32_t *alp_diff_sub_tbl = alp_diff_tbl + fullcount;

			for (int jy = -edge; jy <= edge; jy++)
			{
				for (int jx = -edge; jx <= edge; jx++)
				{
					if (alp_true_tbl[jx + jy * alp_tbl_stride] == 0xff)
					{
						*alp_diff_tbl = diff;
						diff = 0;
						alp_diff_tbl++;
						//sub_diff++;
					}
					else if (alp_true_tbl[jx + jy*alp_tbl_stride])
					{
						*alp_diff_sub_tbl = sub_diff;
						*alp_sub_tbl = alp_true_tbl[jx + jy*alp_tbl_stride];
						sub_diff = 0;
						alp_sub_tbl++;
						alp_diff_sub_tbl++;
						//diff++;
					}
					diff++;
					sub_diff++;

				}
				diff += outline_edge_stride;
				sub_diff += outline_edge_stride;
			}

			assert(ualp_diff_tbl.get() + fullcount == alp_diff_tbl);
			assert(ualp_diff_tbl.get() + fullcount + half_count == alp_diff_sub_tbl);
		}

		uint32_t *pixels = reinterpret_cast<uint32_t*>(canvas.pixels());
		uint8_t *alpha = reinterpret_cast<uint8_t*>(alphamap.pixels());
		alpha += r_x + edge + b_w * (r_y + edge);


		uint16_t *outline_ptr = outline;
		outline += edge + r_w * (edge);
		alp_diff_tbl = ualp_diff_tbl.get();
		int32_t *alp_diff_tbl_ptr = alp_diff_tbl;
		alp_sub_tbl = ualp_sub_tbl.get();
		uint8_t *alp_sub_tbl_ptr = alp_sub_tbl;
		memset(uoutline.get(), 0, r_w * r_h * 2);
		int32_t *alp_diff_tbl_limit_full = alp_diff_tbl + fullcount;
		int32_t *alp_diff_tbl_limit_half = alp_diff_tbl_limit_full + half_count;


		for (y = r_y + edge; y < r_h + r_y - edge; y++)
		{
			for (x = r_x + edge; x < r_w + r_x - edge; x++, alpha++, outline++)
			{
				if (*alpha == 0)
				{
					alp_mean = alpha[-b_w] + alpha[-1] + alpha[1] + alpha[b_w];
					if (alp_mean == 0) continue;
					//if (alp_mean > 255)
					//alp_mean = 255;
					if (alp_mean > 1)
					{
						outline_ptr = outline;
						alp_diff_tbl_ptr = alp_diff_tbl;
						for (; alp_diff_tbl_ptr < alp_diff_tbl_limit_full; alp_diff_tbl_ptr++)
						{
							outline_ptr += *alp_diff_tbl_ptr;
							SHORT_A(*outline_ptr) = 0xff;
						}
						outline_ptr = outline;
						alp_sub_tbl_ptr = alp_sub_tbl;
						for (; alp_diff_tbl_ptr < alp_diff_tbl_limit_half; alp_diff_tbl_ptr++, alp_sub_tbl_ptr++)
						{
							outline_ptr += *alp_diff_tbl_ptr;
							if (SHORT_A(*outline_ptr) != 0xff)
							{
								SHORT_A(*outline_ptr) = ((*alp_sub_tbl_ptr << 8) + SHORT_A(*outline_ptr) * (255 - *alp_sub_tbl_ptr)) >> 8;
							}
						}
					}
				}
				else {
					*outline = 0xff00 | *alpha;
				}
			}
			alpha += stride + edge * 2;
			outline += edge * 2;
		}
		std::unique_ptr<uint8_t> ucoltbl{ new uint8_t[256 * 3] };
		uint8_t *coltbl_r = ucoltbl.get();
		uint8_t *coltbl_g = ucoltbl.get() + 256;
		uint8_t *coltbl_b = ucoltbl.get() + 512;

		for (int i = 0; i < 256; i++)
		{
			src_a = i;
			dst_a = 255 - i;
			coltbl_r[i] = (fnt_r * src_a + ol_r * dst_a) / 255;
			coltbl_g[i] = (fnt_g * src_a + ol_g * dst_a) / 255;
			coltbl_b[i] = (fnt_b * src_a + ol_b * dst_a) / 255;
		}

		outline = uoutline.get();
		pixels = (uint32_t*)canvas.pixels() + r_x + (canvas.stride() >> 2) * r_y;
		stride = (canvas.stride() >> 2) - r_w;

		for (y = r_y; y < r_h + r_y; y++)
		{
			for (x = r_x; x < r_w + r_x; x++, pixels++, outline++)
			{
				if (SHORT_L(*outline) == 255)
				{
					INT_R(*pixels) = fnt_r;
					INT_G(*pixels) = fnt_g;
					INT_B(*pixels) = fnt_b;
					INT_A(*pixels) = fnt_a;

				}
				else if (SHORT_L(*outline) == 0)
				{
					src_a = SHORT_A(*outline) * ol_a >> 8;
					INT_R(*pixels) = ol_r;
					INT_G(*pixels) = ol_g;
					INT_B(*pixels) = ol_b;
					INT_A(*pixels) = src_a;

				}
				else
				{
					INT_R(*pixels) = coltbl_r[SHORT_L(*outline)];
					INT_G(*pixels) = coltbl_g[SHORT_L(*outline)];
					INT_B(*pixels) = coltbl_b[SHORT_L(*outline)];
					INT_A(*pixels) = opaqueness;
				}
			}
			pixels += stride;
			//outline += r_x;
		}
	}
}

ICSE::graphics::RECT ICSE::font::FontRenderer::GetRenderRect(const char * utf8txt)
{
	UTF8Reader ur(utf8txt);
	int codepoint1, codepoint2;
	codepoint2 = ur.readCodePoint();

	Font::BakeInfo binfo = m_bakeinfo;
	int x = 0, y = 0, basex = 0, basey = 0;
	int ascent = std::round((float)m_font->ascent() * binfo.scaleY),
		descent = std::round((float)m_font->descent() * binfo.scaleY);
	int r_w = 0, r_h = 0,
		l_h = m_lineheight;
	int r_x = 0, r_y = 0;
	basex = r_x = m_outline_width; basey = r_y = m_outline_width;
	x = basex;

	//Get Font alpha
	while (true)
	{
		//draw one line text
		while (true)
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

			//Prepare for the next character
			x += m_font->GetCodepointAdvance(codepoint1) * binfo.scaleX;
			x += m_font->GetKernelAdvance(codepoint1, codepoint2) * binfo.scaleX;
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
		if (r_w < x)
			r_w = x;
		x = basex;
		basey += m_lineheight + m_linespace;
	}
	r_w += m_outline_width;
	r_h = basey + m_outline_width;

	return ICSE::graphics::RECT{ 0, 0, r_h, r_w };
}

void ICSE::font::FontRenderer::SetFontHeight(int height)
{
	m_lineheight = height; 
	m_font->GetScaleBakeInfo(height, &m_bakeinfo);
}

void ICSE::font::FontRenderer::SetFontFamily(std::shared_ptr<Font> font)
{
	m_font = font;
}
