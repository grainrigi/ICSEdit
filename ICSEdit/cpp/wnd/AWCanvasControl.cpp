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

#include "wnd/AWCanvasControl.h"

using namespace ICSE::wnd;

ICSE::wnd::AWCanvasControl::AWCanvasControl(int width, int height)
	: m_canvas(width, height)
{
	Resize(width, height);

	Resize += [&](int w, int h) { this->OnResize(w, h); };
}

void ICSE::wnd::AWCanvasControl::OnResize(int w, int h)
{
	if (m_canvas.width() != w || m_canvas.height() != h)
		m_canvas = graphics::MemCanvasRGBA8(w, h);
}

