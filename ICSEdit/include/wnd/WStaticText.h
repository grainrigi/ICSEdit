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

#pragma once
#include "wnd/AWSizableControl.h"
#include "util/font/FontRenderer.h"

namespace ICSE {
namespace wnd {

	enum eWStaticTextAlign
	{
		Left = 0x0001,
		Right = 0x0002,
		CenterHorz = 0x0003,
		Top = 0x0100,
		CenterVert = 0x0200,
		Bottom = 0x0300,
	};

	class WStaticText : public AWSizableControl {
		font::FontRenderer m_renderer;
		std::string m_str;
		bool m_rendered;
		int m_cnt;
		int m_align;
		uint8_t m_opaqueness;
	public:
		WStaticText(std::shared_ptr<font::Font> font);

		void SetFontFamily(std::shared_ptr<font::Font> font);
		void SetFontSize(int size);
		void SetOutlineSize(int size);

		void SetFontColor(const graphics::ColorDW &color);
		void SetFontColor(uint32_t color);
		void SetOutlineColor(const graphics::ColorDW &color);
		void SetOutlineColor(uint32_t color);

		void SetOpaqueness(uint8_t opaqueness);
		void SetAlign(int align);

		void SetText(const std::string &str);
		std::string GetText(void) const { return this->m_str; }
		
	private:
		 void update(void);
	};

}
}
