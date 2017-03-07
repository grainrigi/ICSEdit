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
#include "graphics/gl/GLTexture.h"

namespace ICSE {
namespace graphics {
	class MemTexturedCanvasRGBA8;

	class MemCanvasRenderTexturePool {
		static constexpr int BLOCK_WIDTH = 64;
		static constexpr int BLOCK_HEIGHT = 64;
		static constexpr int BLOCK_HORZ_COUNT = 32;
		static constexpr int BLOCK_VERT_COUNT = 32;

		

	public:
		MemCanvasRenderTexturePool(void);

		MemTexturedCanvasRGBA8 ObtainCanvas(int width, int height);
		void ReleaseSpace(int spaceid);

		struct SpaceUnit
		{
			int id;
			int x, y;
			int w, h;
		};

		class TextureUnit
		{
			gl::GLTexture m_texture;
			uint8_t m_map[BLOCK_HORZ_COUNT * BLOCK_VERT_COUNT];
			std::unordered_map<int, SpaceUnit> m_spaces;

		public:
			TextureUnit(void);

			GLuint texhandle(void) const { return m_texture.handle(); }
			SpaceUnit ObtainNewSpace(int width, int height);
			void ReleaseSpace(int spaceid);
			void UpdateTexture(int spaceid, const MemCanvasRGBA8 &canvas);
		};


	private:
		std::vector<TextureUnit> m_units;
		static int ObtainNewID(void);
		
	};

}
}