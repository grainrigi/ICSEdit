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
#include "wnd/BoundingBox.h"
#include "graphics/MemCanvasRGBA8.h"
#include "graphics/drawenv.h"
#include "graphics/Mesh2DRenderer.h"

namespace ICSE {
namespace graphics {

	class MemCanvasRenderer {
		Mesh2DRenderer *m_renderer;
		gl::GLTexture m_texture;
		Mesh2D m_mesh;
	public:
		MemCanvasRenderer(Mesh2DRenderer *renderer);

		void Render(DrawEnv *env, const MemCanvasRGBA8 &canvas, const wnd::BoundingBox &bb);
		
	private:
		
	};

}
}
