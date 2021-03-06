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

namespace ICSE{
namespace graphics{

	struct Position2D {
		float x;
		float y;
	};
	struct Position3D {
		float x;
		float y;
		float z;
	};
	typedef Position3D Position;
	typedef Position2D Coordinate;

	struct ColorF {
		float r;
		float g;
		float b;
		float a;
	};
	struct ColorDW {
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};
	typedef ColorF Color;

	struct RECT {
		int top;
		int left;
		int bottom;
		int right;

		int width(void) const { return std::abs(right - left); }
		int height(void) const { return std::abs(bottom - top); }
	};
}
}