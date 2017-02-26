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

#include "graphics/MemCanvasRGBA8.h"
#include "graphics/MemSubCanvasRGBA8.h"

using namespace ICSE::graphics;

ICSE::graphics::MemCanvasRGBA8::MemCanvasRGBA8(int width, int height)
	: MemCanvas(4, width, height)
{
}

MemSubCanvasRGBA8 ICSE::graphics::MemCanvasRGBA8::GetSubPixels(int x, int y, int width, int height)
{
	return MemSubCanvasRGBA8(*this, x, y, width, height);
}

void ICSE::graphics::MemCanvasRGBA8::ClearWithColor(uint32_t color)
{
	int x, y, w = width(), h = height(), s = (stride() >> 2) - w;
	uint32_t *pix = reinterpret_cast<uint32_t*>(pixels());
	for(y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++, pix++)
			*pix = color;
		pix += s;
	}
}

ICSE::graphics::MemCanvasRGBA8::MemCanvasRGBA8(int width, int height, void * mem)
	: MemCanvas(4, width, height, mem)
{
}
