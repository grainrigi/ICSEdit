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
#include "util/font/Font.h"
#include "graphics/MemCanvasRGBA8.h"

namespace ICSE {
namespace font {
	
	class FontRenderer {
		std::shared_ptr<Font> m_font;
		Font::BakeInfo m_bakeinfo;
		uint32_t m_outline_color;
		int m_outline_width;
		int m_lineheight;
		int m_linespace;
		uint32_t m_font_color;
	 public:
		FontRenderer(std::shared_ptr<Font> font);
		
		void RenderText(ICSE::graphics::MemCanvasRGBA8 &canvas, const char *utf8txt, int topx = 0, int topy = 0, int opaqueness = 255);
		//this function does not blend the text
		void BakeText(ICSE::graphics::MemCanvasRGBA8 &canvas, const char *utf8txt, int topx = 0, int topy = 0, int opaqueness = 255);

		graphics::RECT GetRenderRect(const char *utf8txt);

		//Set the outline
		//If thickness is zero, the outline won't be drawn.
		void SetOutlineColor(uint32_t color) { m_outline_color = color; }
		void SetOutlineWidth(int width) { m_outline_width = width; }

		void SetFontHeight(int height);
		void SetLineSpace(int space) { m_linespace = space; }
		void SetFontColor(uint32_t color) { m_font_color = color; }

		void SetFontFamily(std::shared_ptr<Font> font);
	 private:
		
	};

}
}
