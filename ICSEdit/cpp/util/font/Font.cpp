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

#include "util/basic/Singleton.h"
#include "util/font/FreeType.h"
#include "util/font/Font.h"

using namespace ICSE;
using namespace ICSE::font;
using ICSE::file::BinaryFile;

ICSE::font::Font::Font(void)
	: m_info { 0 },
	m_data{ nullptr }
{
}



Font ICSE::font::Font::LoafFromFile(const std::string & filepath)
{
	BinaryFile file = BinaryFile::LoadFromFile(filepath);
	return LoadFromFile(file);
}

Font ICSE::font::Font::LoadFromFile(const ICSE::file::BinaryFile & file)
{
	Font fnt;
	fnt.m_data.reset(new uint8_t[file.getSize()]);
	memcpy(fnt.m_data.get(), file.getData(), file.getSize());

	stbtt_InitFont(&fnt.m_info, fnt.m_data.get(), 0);
	fnt.init();

	return fnt;
}

Font ICSE::font::Font::LoadFromMemory(const void * buffer, int size)
{
	Font fnt;
	fnt.m_data.reset(new uint8_t[size]);
	memcpy(fnt.m_data.get(), buffer, size);
	FT_Library lib = Singleton<FreeType>::getInstance().library();

	FT_New_Memory_Face(lib, fnt.m_data.get(), size, 0, &fnt.m_face);

	return fnt;
}

void ICSE::font::Font::MakeCodepointBitmap(ICSE::graphics::MemCanvasA8 * dest, int codepoint, int cbx, int cby, const BakeInfo & info) const
{
	stbtt_MakeCodepointBitmap(
		&m_info,
		reinterpret_cast<unsigned char*>(dest->pixelAt(cbx, cby)),
		info.bbox.width(), info.bbox.height(),
		dest->stride(),
		info.scaleX, info.scaleY, codepoint);
}

void ICSE::font::Font::GetScaleBakeInfo(int lineHeight, BakeInfo * info) const
{
	info->scaleX = info->scaleY = stbtt_ScaleForPixelHeight(&m_info, (float)lineHeight);
}

void ICSE::font::Font::GetCodepointBakeInfo(int codepoint, BakeInfo * info) const
{
	stbtt_GetCodepointBitmapBox(&m_info, codepoint,
		info->scaleX, info->scaleY,
		&info->bbox.left, &info->bbox.top,
		&info->bbox.right, &info->bbox.bottom);
}

int ICSE::font::Font::GetCodepointAdvance(int codepoint) const
{
	int adv;
	stbtt_GetCodepointHMetrics(&m_info, codepoint, &adv, 0);
	return adv;
}

int ICSE::font::Font::GetKernelAdvance(int codepoint1, int codepoint2) const
{
	return stbtt_GetGlyphKernAdvance(&m_info, codepoint1, codepoint2);
}

void ICSE::font::Font::init(void)
{
	stbtt_GetFontVMetrics(&m_info, &m_vmetrics.ascent, &m_vmetrics.descent, &m_vmetrics.lineGap);
}


