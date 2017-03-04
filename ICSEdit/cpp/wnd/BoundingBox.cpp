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

#include "wnd/BoundingBox.h"

using namespace ICSE::wnd;

ICSE::wnd::BoundingBox::BoundingBox(void)
	: x{0}, y{0}, w{0}, h{0}
{
}

ICSE::wnd::BoundingBox::BoundingBox(int nx, int ny, int nw, int nh)
	: x{ nx }, y{ ny }, w{ nw }, h{ nh }
{
}

void ICSE::wnd::BoundingBox::SetWidth(int nw)
{
	if (nw < 0)
		THROW(std::invalid_argument, "the specified width is below 0.");
	w = nw;
}

void ICSE::wnd::BoundingBox::SetHeight(int nh)
{
	if (nh < 0)
		THROW(std::invalid_argument, "the specified height is below 0.");
	h = nh;
}