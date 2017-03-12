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
using namespace ICSE::graphics;

ICSE::wnd::AWCanvasControl::AWCanvasControl()
	: m_canvas{}
{
	
}

ICSE::wnd::AWCanvasControl::~AWCanvasControl(void)
{
	if(!m_observer.expired())
	{
		m_observer.lock()->NotifyDeletion(this->m_canvas.txunitid(),this->id());
	}
}

void ICSE::wnd::AWCanvasControl::InitCanvas(int width, int height, graphics::MemCanvasRenderTexturePool & pool)
{
	this->m_canvas = pool.ObtainCanvas(width, height);

	Resize(width, height);

	Resize += [&](int w, int h) { this->OnResize(w, h); };
	Move += [&](int x, int y) { this->OnMove(x, y); };
}

void ICSE::wnd::AWCanvasControl::OnResize(int w, int h)
{
	m_canvas.Resize(w, h);
	if (!m_observer.expired())
		m_observer.lock()->NotifyResize(this);
		
}

void ICSE::wnd::AWCanvasControl::OnMove(int x, int y)
{
	if (!m_observer.expired())
		m_observer.lock()->NotifyMove(this);
}

