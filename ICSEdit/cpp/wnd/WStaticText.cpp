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

#include "wnd/WStaticText.h"

using namespace ICSE::wnd;

ICSE::wnd::WStaticText::WStaticText(std::shared_ptr<font::Font> font)
	: m_renderer(font),
	AWSizableControl(),
	m_str(""),
	m_rendered(false),
	m_align(0),
	m_opaqueness(255)
{
	dUpdate += [&]() { this->update(); };
	this->AWControl::Resize += [&](int w, int h) { this->m_rendered = false; };
}

void ICSE::wnd::WStaticText::SetFontFamily(std::shared_ptr<font::Font> font)
{
	m_renderer.SetFontFamily(font);
	m_rendered = false;
}

void ICSE::wnd::WStaticText::SetFontSize(int size)
{
	m_renderer.SetFontHeight(size);
	m_rendered = false;
}

void ICSE::wnd::WStaticText::SetOutlineSize(int size)
{
	m_renderer.SetOutlineWidth(size);
	m_rendered = false;
}

void ICSE::wnd::WStaticText::SetFontColor(const graphics::ColorDW & color)
{
	m_renderer.SetFontColor(color.UintRepr());
	m_rendered = false;
}

void ICSE::wnd::WStaticText::SetFontColor(uint32_t color)
{
	m_renderer.SetFontColor(color);
	m_rendered = false;
}

void ICSE::wnd::WStaticText::SetOutlineColor(const graphics::ColorDW & color)
{
	m_renderer.SetOutlineColor(color.UintRepr());
	m_rendered = false;
}

void ICSE::wnd::WStaticText::SetOutlineColor(uint32_t color)
{
	m_renderer.SetOutlineColor(color);
	m_rendered = false;
}

void ICSE::wnd::WStaticText::SetOpaqueness(uint8_t opaqueness)
{
	m_opaqueness = opaqueness;
	m_rendered = false;
}

void ICSE::wnd::WStaticText::SetAlign(int align)
{
	m_align = align;
	m_rendered = false;
}

void ICSE::wnd::WStaticText::SetText(const std::string & str)
{
	m_str = str;
	m_rendered = false;
}

void ICSE::wnd::WStaticText::update(void)
{
	if(!m_rendered)
	{
		auto canvas = m_canvas.Lock();

		auto start = std::chrono::high_resolution_clock::now();

		int x, y;
		graphics::RECT rect;
		rect = m_renderer.GetRenderRect(m_str.c_str());

		switch(m_align & 0xff)
		{
			case eWStaticTextAlign::Left:
				x = 0;
				break;
			case eWStaticTextAlign::Right:
				x = m_canvas.width() - rect.width() - 1;
				break;
			case eWStaticTextAlign::CenterHorz:
				x = (m_canvas.width() - rect.width()) >> 1;
				break;
			default:
				x = 0;
				break;
		}
		switch(m_align & 0xff00)
		{
			case eWStaticTextAlign::Top:
				y = 0;
				break;
			case eWStaticTextAlign::Bottom:
				y = m_canvas.height() - rect.height() - 1;
				break;
			case eWStaticTextAlign::CenterVert:
				y = (m_canvas.height() - rect.height()) >> 1;
				break;
			default:
				y = 0;
				break;
		}
		x = x < 0 ? 0 : x;
		y = y < 0 ? 0 : y;
		
		canvas->ClearWithColor(0x00000000);
		m_renderer.RenderText(*canvas, m_str.c_str(), x, y, m_opaqueness);

		auto end = std::chrono::high_resolution_clock::now();

		m_cnt = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		m_canvas.Unlock();

		m_rendered = true;
	}
}


