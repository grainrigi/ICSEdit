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

#include "util/font/UTF8Reader.h"

using namespace ICSE::font;

#define ONE_MASK 0x7F
#define TWO_MASK 0xE0
#define THREE_MASK 0xC0
#define FOUR_MASK 0xE0

int ICSE::font::UTF8Reader::readCodePoint(void)
{
	unsigned char c = *m_curpoint;
	int ret = 0;
	if ((c & ONE_MASK) == c)
	{
		if (c != 0x00)
			m_curpoint++;
		return c;
	}
	else if ((c & 0xE0) == 0xC0)
	{
		ret |= (c & 0x1F) << 6;
		ret |= *++m_curpoint & 0x3F;
		m_curpoint++;
		return ret;
	}
	else if ((c & 0xF0) == 0xE0)
	{
		ret |= (c & 0x0F) << 12;
		ret |= (*++m_curpoint & 0x3F) << 6;
		ret |= *++m_curpoint & 0x3F;
		m_curpoint++;
		return ret;
	}
	else if ((c & 0xF8) == 0xF0)
	{
		ret |= (c & 0x07) << 18;
		ret |= (*++m_curpoint & 0x3F) << 12;
		ret |= (*++m_curpoint & 0x3F) << 6;
		ret |= (*++m_curpoint & 0x3F);
		m_curpoint++;
		return ret;
	}
	else
		return 0;
}
