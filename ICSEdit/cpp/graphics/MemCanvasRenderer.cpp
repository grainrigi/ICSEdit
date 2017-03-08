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

#include "graphics/MemCanvasRenderer.h"

using namespace ICSE::graphics;
using namespace ICSE::graphics::gl;

ICSE::graphics::MemCanvasRenderer::MemCanvasRenderer(Mesh2DRenderer *renderer)
	: m_renderer{renderer}
{
	m_texture.init();
	m_texture.uploadImage(nullptr, 2048, 2048, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	
	m_mesh = Mesh2D(4, Mesh2D::ATTR_POSITION | Mesh2D::ATTR_COORD, GL_DYNAMIC_DRAW);
	memset(&(*m_mesh.positions())[0], 0, sizeof(Position) * 4);
	memset(&(*m_mesh.coordinates())[0], 0, sizeof(Coordinate) * 4);
}

void ICSE::graphics::MemCanvasRenderer::Render(DrawEnv * env, const MemCanvasRGBA8 & canvas, const wnd::BoundingBox & bb)
{
	env->setTexture(m_texture.handle());
	glTexSubImage2D(GL_TEXTURE_2D, 0, GL_RGBA, canvas.width(), canvas.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, canvas.pixels());

	float rw = (float)bb.GetWidth() / (float)env->width() * 2.0f;
	float rh = (float)bb.GetHeight() / (float)env->height() * 2.0f;
	float rx = (float)bb.GetX() / (float)env->width() * 2.0f - 1.0f;
	float ry = 1.0f - (float)bb.GetY() / (float)env->height() * 2.0f;

	//float tw = (float)canvas.width() / 2048.0f;
	//float th = (float)canvas.height() / 2048.0f;
	float tw = 1.0f, th = 1.0f;

	Position *pos_ptr = &(*m_mesh.positions())[0];
	pos_ptr[0].x = rx; pos_ptr[0].y = ry;
	pos_ptr[1].x = rx + rw; pos_ptr[1].y = ry;
	pos_ptr[2].x = rx; pos_ptr[2].y = ry - rh;
	pos_ptr[3].x = rx + rw; pos_ptr[3].y = ry - rh;
	Coordinate *coord_ptr = &(*m_mesh.coordinates())[0];
	coord_ptr[1].x = tw;
	coord_ptr[2].y = th;
	coord_ptr[3].x = tw; coord_ptr[3].y = th;

	m_mesh.upload();
	m_renderer->draw(env, m_mesh);
}
