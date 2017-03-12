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

#include "wnd/WindowLayer.h"


using namespace ICSE::wnd;
using namespace ICSE::graphics;


ICSE::wnd::WindowLayer::WindowLayer(ControlWindow * parent)
	: m_parent{ parent }
{
}

void ICSE::wnd::WindowLayer::UpdateAll(void)
{
	for (auto &control : m_controls)
		control.second->Update();
}

void ICSE::wnd::WindowLayer::RenderAll(DrawEnv *env)
{
	m_ascent.RenderAll(env);
	m_descent.RenderAll(env);
}
