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

#include "wnd/AWButton.h"

using namespace ICSE::wnd;

ICSE::wnd::AWButton::AWButton(void)
	: m_pushing(false)
{
	this->AWControl::dOnEvent += [&](SDL_Event &e) { this->OnEvent(e); };
}

void ICSE::wnd::AWButton::OnEvent(SDL_Event & evt)
{
	switch (evt.type)
	{
		case SDL_MOUSEBUTTONDOWN:
		{
			if (GetBB().isIncludePoint(evt.button.x, evt.button.y))
			{
				m_pushing = true;
				setPushing(true);
			}
		}break;
		case SDL_MOUSEBUTTONUP:
		{
			if (m_pushing)
			{
				m_pushing = false;
				setPushing(false);
				if (GetBB().isIncludePoint(evt.button.x, evt.button.y))
				{
					OnClick();
				}
			}
		}break;
	}
}