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

#include "wnd/AWControl.h"

using namespace ICSE::wnd;

ICSE::wnd::AWControl::AWControl(void)
{
	Move += [&](int x, int y) { this->move(x, y); };
	Resize += [&](int w, int h) { this->resize(w, h); };
}

void ICSE::wnd::AWControl::move(int x, int y)
{
	m_bb.SetX(x);
	m_bb.SetY(y);
}

void ICSE::wnd::AWControl::resize(int w, int h)
{
	m_bb.SetWidth(w);
	m_bb.SetHeight(h);
}
