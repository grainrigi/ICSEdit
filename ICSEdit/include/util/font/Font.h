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
#include "util/file/BinaryFile.h"
#include "util/font/stb_truetype.h"
#include "graphics/MemCanvasA8.h"
#include "graphics/Geometry.h"

namespace ICSE {
namespace font {

	class Font {		
		stbtt_fontinfo m_info;
		std::unique_ptr<uint8_t> m_data;
		FT_Face m_face;

		struct VMetrics{
			int32_t ascent;
			int32_t descent;
			int32_t lineGap;
		};

		VMetrics m_vmetrics;
	public:
		struct BakeInfo {
			ICSE::graphics::RECT bbox;
			int LineHeight;
			float scaleX;
			float scaleY;
		};


		Font(void);
		~Font(void) = default;

		static Font LoafFromFile(const std::string &filepath);
		static Font LoadFromFile(const ICSE::file::BinaryFile &file);
		static Font LoadFromMemory(const void *buffer, int size);

		Font(Font &&font) = default;
		Font &operator=(Font &&font) = default;
		
		int32_t descent(void) const { return m_vmetrics.descent; }
		int32_t ascent(void) const { return m_vmetrics.ascent; }
		int32_t lineGap(void) const { return m_vmetrics.lineGap; }

		void MakeCodepointBitmap(ICSE::graphics::MemCanvasA8 *dest, int codepoint, int cbx, int cby, int cbWidth, int cbHeight, float scaleX, float scaleY) const;
		void MakeCodepointBitmap(ICSE::graphics::MemCanvasA8 *dest, int codepoint, int cbWidth, int cbHeight, float scaleX, float scaleY) const;
		void MakeCodepointBitmap(ICSE::graphics::MemCanvasA8 *dest, int codepoint, int cbx, int cby, const BakeInfo &info) const;
		void GetScaleBakeInfo(int lineHeight, BakeInfo *info) const;
		void GetCodepointBakeInfo(int codepoint, BakeInfo *info) const;
		int GetCodepointAdvance(int codepoint) const;
		int GetKernelAdvance(int codepoint1, int codepoint2) const;
	 private:
		 void init(void);
	};

}
}
