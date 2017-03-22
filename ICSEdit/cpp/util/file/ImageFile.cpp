#include "util/file/ImageFile.h"

#include "util/file/stb_image.h"
#include "util/file/BinaryFile.h"

using ICSE::file::BinaryFile;
using ICSE::file::ImageFile;

using ICSE::container::Uint8Array;
using ICSE::graphics::MemCanvasRGBA8;

ImageFile ImageFile::LoadFromFile(const BinaryFile &file, int format)
{
	return LoadFromMemory(file.getData(), file.getSize(), format);
}

ImageFile ICSE::file::ImageFile::LoadFromFile(const std::string & filepath, int format)
{
	BinaryFile file = BinaryFile::LoadFromFile(filepath);
	return LoadFromFile(file, format);
}

ImageFile ICSE::file::ImageFile::LoadFromMemory(const uint8_t * buffer, uint32_t size, int format)
{
	ImageFile img;
	img.m_pixels = Uint8Array(reinterpret_cast<uint8_t*>(stbi_load_from_memory(
			reinterpret_cast<const stbi_uc*>(buffer), 
			size, 
			&img.m_x, 
			&img.m_y, 
			&img.m_channels, 
			format
		)),
		img.m_x * img.m_y * img.m_channels, 
		[](uint8_t *ptr) -> void {
			stbi_image_free(ptr);
		}
	);

	return img;
}

ImageFile::ImageFile(void)
	: m_x(0),
	m_y(0),
	m_channels(0)
{
}

ICSE::graphics::MemCanvasRGBA8 ICSE::file::ImageFile::CreateMemCanvas(void)
{
	MemCanvasRGBA8 canvas(m_x, m_y);

	assert(m_channels == 3 || m_channels == 4);

	if(m_channels == 3)
	{
		uint8_t *sptr = m_pixels.getPtr();
		uint8_t *dptr = reinterpret_cast<uint8_t*>(canvas.pixels());
		for(int i = 0; i < m_x * m_y; i++)
		{
			dptr[i * 4] = sptr[i * 3];
			dptr[i * 4 + 1] = sptr[i * 3 + 1];
			dptr[i * 4 + 2] = sptr[i * 3 + 2];
			dptr[i * 4 + 3] = 255;
		}
	}
	else if(m_channels == 4)
	{
		memcpy(canvas.pixels(), m_pixels.getPtr(), m_x * m_y * sizeof(uint32_t));
	}

	return canvas;
}

