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

#include "graphics/MemTexturedCanvasRGBA8.h"

using namespace ICSE::graphics;


ICSE::graphics::MemTexturedCanvasRGBA8::~MemTexturedCanvasRGBA8(void)
{
	if(!m_txunit.expired())
		m_txunit.lock()->ReleaseSpace(m_spaceid);
}

MemCanvasRGBA8 * ICSE::graphics::MemTexturedCanvasRGBA8::Lock(void)
{
	if (!m_locking)
	{
		m_locking = true;
		return static_cast<MemCanvasRGBA8*>(this);
	}
	else
		return nullptr;
}

void ICSE::graphics::MemTexturedCanvasRGBA8::Unlock(void)
{
	m_locking = false;
	m_txunit.lock()->UpdateTexture(m_spaceid, *static_cast<MemCanvasRGBA8*>(this));
}

void ICSE::graphics::MemTexturedCanvasRGBA8::Resize(int w, int h)
{
	MemCanvasRenderTexturePool *pool = m_pool;
	this->~MemTexturedCanvasRGBA8();
	this->operator=(std::move(pool->ObtainCanvas(w, h)));
}

ICSE::graphics::MemTexturedCanvasRGBA8::MemTexturedCanvasRGBA8(int width, int height)
	: MemCanvasRGBA8(width, height),
	m_locking(false)
{
}
