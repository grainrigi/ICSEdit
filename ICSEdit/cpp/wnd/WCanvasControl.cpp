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

#include "wnd/WCanvasControl.h"

using namespace ICSE::wnd;
using namespace ICSE::graphics;

ICSE::wnd::WCanvasControl::WCanvasControl(int w, int h)
	: AWSizableControl(w, h)
{
}

MemCanvasRGBA8 * ICSE::wnd::WCanvasControl::Lock(void)
{
	return m_canvas.Lock();
}

void ICSE::wnd::WCanvasControl::Unlock(void)
{
	m_canvas.Unlock();
}

/*void ICSE::wnd::WCanvasControl::Move(int x, int y)
{
	this->AWControl::Move(x, y);
}

void ICSE::wnd::WCanvasControl::Resize(int w, int h)
{
	this->AWControl::Resize(w, h);
}*/
