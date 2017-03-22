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
#include "util/container/Uint8Array.h"
#include "util/file/BinaryFile.h"
#include "graphics/MemCanvasRGBA8.h"

namespace ICSE{
namespace file{

	class ImageFile {
		int32_t m_x;
		int32_t m_y;
		int32_t m_channels;
		ICSE::container::Uint8Array m_pixels;

	public:
		enum {
			FORMAT_RGB = 3,
			FORMAT_RGBA = 4,
		};

		//The file size is limited up to 4GiB
		static ImageFile LoadFromFile(const BinaryFile &file, int format);
		static ImageFile LoadFromFile(const std::string &filepath, int format);
		static ImageFile LoadFromMemory(const uint8_t *buffer, uint32_t size, int format);
		ImageFile(void);
		~ImageFile(void) = default;

		ImageFile(const ImageFile &img) = default;
		ImageFile &operator=(const ImageFile &img) = default;
		ImageFile(ImageFile &&img) = default;
		ImageFile &operator=(ImageFile &&img) = default;

		int32_t getX(void) const { return this->m_x; }
		int32_t getY(void) const { return this->m_y; }
		int32_t getChannelCount(void) const { return this->m_channels; }

		uint32_t getPixelsSize(void) const { return this->m_pixels.getSize(); }
		const uint8_t *getPixels(void) const { return this->m_pixels.getPtr(); }

		graphics::MemCanvasRGBA8 CreateMemCanvas(void);

	private:
	};

}
}
