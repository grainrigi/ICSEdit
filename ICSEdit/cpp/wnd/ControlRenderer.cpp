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
#include "wnd/ControlRenderer.h"
#include "graphics/MemCanvasRenderTexturePool.h"


using namespace ICSE::wnd;
using namespace ICSE::graphics;
using namespace ICSE::graphics::gl;

ICSE::wnd::ControlObserver::ControlObserver(ControlRenderer * renderer)
	: m_renderer{ renderer }
{
}


void ICSE::wnd::ControlObserver::NotifyResize(const AWCanvasControl * control)
{
	m_renderer->OnResize(control);
}


void ICSE::wnd::ControlObserver::NotifyMove(const AWCanvasControl * control)
{
	m_renderer->OnMove(control);
}

void ICSE::wnd::ControlObserver::NotifyDeletion(uint32_t txunitid, uint32_t control_id)
{
	if(m_renderer != nullptr)
	{
		m_renderer->RemoveControl(txunitid, control_id);
	}
}

void ICSE::wnd::ControlObserver::Invalidate(void)
{
	m_renderer = nullptr;
}

ICSE::wnd::ControlRenderer::ControlRenderer(void)
	: m_observer{new ControlObserver(this)},
	m_pool(this)
{
}

void ICSE::wnd::ControlRenderer::LateInitialize(void)
{
	initShader();
}

void ICSE::wnd::ControlRenderer::AddCanvasControl(const std::shared_ptr<AWCanvasControl> control)
{
	control->InitCanvas(control->GetBB().GetWidth(), control->GetBB().GetHeight(), m_pool);

	DrawInfo info;
	control->m_observer = m_observer;
	info.control_id = control->id();
	info.bbox = control->GetBB();
	info.space = m_pool.GetTxUnit(control->m_canvas.txunitid())->GetSpaceInfo(control->m_canvas.spaceid());
	info.texwidth = control->m_canvas.width();
	info.texheight = control->m_canvas.height();

	if(m_drawunits.find(control->m_canvas.txunitid()) == m_drawunits.end())
		createDrawUnit(control->m_canvas.txunitid());

	m_drawunits[control->m_canvas.txunitid()].AddInfo(info);
}

void ICSE::wnd::ControlRenderer::RemoveControl(uint32_t txunitid, uint32_t control_id)
{
	m_drawunits[txunitid].RemoveDrawInfo(control_id);
}

void ICSE::wnd::ControlRenderer::RenderAll(DrawEnv *env)
{
	m_shader.use();

	Matrix4f mat;
	mat.identity();
	mat *= Matrix4f::createScale(2.0f / (float)env->width(), 2.0f / -(float)env->height(), 1.0f);
	glUniform2f(m_unif_mat_loc, 2.0f / (float)env->width(), 2.0f / -(float)env->height());

	for (auto &unit : m_drawunits)
		unit.second.RenderAll(m_shader);

	scavengeGC();
}


void ICSE::wnd::ControlRenderer::OnResize(const AWCanvasControl * control)
{
	if (m_drawunits.find(control->m_canvas.txunitid()) == m_drawunits.end())
	{
		createDrawUnit(control->m_canvas.txunitid());
		
		DrawInfo info;

		info.space = m_pool.GetTxUnit(control->m_canvas.txunitid())->GetSpaceInfo(control->m_canvas.spaceid());
		info.bbox = control->GetBB();
		info.texwidth = control->m_canvas.width();
		info.texheight = control->m_canvas.height();
		info.setAttribute(DrawInfo::FLAG_DIRTY);
		this->m_drawunits[control->m_canvas.txunitid()].AddInfo(info);
	}
	else {
		DrawInfo &info = m_drawunits.find(control->m_canvas.txunitid())->second.GetDrawInfo(control->id());

		info.space = m_pool.GetTxUnit(control->m_canvas.txunitid())->GetSpaceInfo(control->m_canvas.spaceid());
		info.bbox = control->GetBB();
		info.texwidth = control->m_canvas.width();
		info.texheight = control->m_canvas.height();
		info.setAttribute(DrawInfo::FLAG_DIRTY);
	}
}


void ICSE::wnd::ControlRenderer::OnMove(const AWCanvasControl * control)
{
	DrawInfo &info = m_drawunits[control->m_canvas.txunitid()].GetDrawInfo(control->id());
	
	info.bbox = control->GetBB();
	info.setAttribute(DrawInfo::FLAG_DIRTY);
}

ICSE::wnd::ControlRenderer::TxPoolHook::TxPoolHook(ControlRenderer * parent)
	: m_parent{ parent }
{
}

MemTexturedCanvasRGBA8 ICSE::wnd::ControlRenderer::TxPoolHook::ObtainCanvas(int width, int height)
{
	MemTexturedCanvasRGBA8 && a = this->MemCanvasRenderTexturePool::ObtainCanvas(width, height);

	return std::move(a);
}

MemCanvasRenderTexturePool * ICSE::wnd::ControlRenderer::GetPool(void)
{
	return static_cast<MemCanvasRenderTexturePool*>(&m_pool);
}

void ICSE::wnd::ControlRenderer::createDrawUnit(uint32_t txunitid)
{
	m_drawunits.insert(std::make_pair(txunitid, DrawUnit(txunitid, this)));
}

void ICSE::wnd::ControlRenderer::scavengeGC(void)
{
	for (auto &unit : m_drawunits)
		unit.second.scavengeGC();
}

void ICSE::wnd::ControlRenderer::fullGC(void)
{
	for (auto &unit : m_drawunits)
		unit.second.fullGC();
}

void ICSE::wnd::ControlRenderer::initShader(void)
{
	std::string vShader{
		"uniform vec2 unif_disp;"
		"attribute vec4 attr_pos; "
		"attribute vec2 attr_uv;"
		"varying vec2 vary_uv;"
		"void main(){"
		"  gl_Position = vec4(attr_pos.x * unif_disp.x, attr_pos.y * unif_disp.y, 0.0, 1.0) + vec4(-1.0, 1.0, 0.0, 0.0);"
		"  vary_uv = attr_uv;"
		"}"
	};

	std::string fShader{
		"uniform sampler2D unif_texture;"
		"varying vec2 vary_uv;"
		"void main(){"
		"  gl_FragColor = texture2D(unif_texture, vary_uv);"
		"}"
	};

	m_shader = GLShaderSet::createFromString(
		vShader,
		fShader,
		{
			{ 0, "attr_pos" },
			{ 1, "attr_uv" }
		}
	);

	glUniform1i(m_shader.getUniformLocation("unif_texture"), 0);
	m_unif_mat_loc = m_shader.getUniformLocation("unif_disp");
}

ICSE::wnd::ControlRenderer::DrawUnit::DrawUnit(uint32_t txunitid, ControlRenderer *parent)
	: m_txunitid(txunitid),
	m_parent(parent),
	m_vbuf(GLVBOTarget::ARRAY_BUFFER, GL_DYNAMIC_DRAW),
	m_ibuf(GLVBOTarget::ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
	first_free(-1)
{
}

void ICSE::wnd::ControlRenderer::DrawUnit::AddInfo(DrawInfo info)
{
	int index = -1;

	//empty space exists
	if (first_free != -1)
	{
		index = first_free;
		first_free = m_infos[index].next_free;
		m_infos[index] = info;
		m_infos[index].setAttribute(DrawInfo::FLAG_DIRTY);
	}
	else
	{
		m_infos.push_back(info);
		index = m_infos.size() - 1;
		m_infos[index].setAttribute(DrawInfo::FLAG_DIRTY);
		resizeVBO(m_infos.capacity());
	}

	m_infomap[info.control_id] = index;
}

void ICSE::wnd::ControlRenderer::DrawUnit::RenderAll(graphics::gl::GLShaderSet &shader)
{
	glBindTexture(GL_TEXTURE_2D, m_parent->m_pool.GetTxUnit(m_txunitid)->texhandle());

	int size = m_infos.size();
	DrawInfo *ptr = &m_infos[0];

	for (int i = 0; i < size; i++, ptr++)
	{
		if (ptr->hasAttribute(DrawInfo::FLAG_DIRTY))
			updateInfo(i);
	}
	shader.use();
	glBindBuffer(GL_ARRAY_BUFFER, m_vbuf.handle());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibuf.handle());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Position));
	
	

	//m_ibuf.bind();
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//uint16_t indices[6]{ 0, 1, 2, 2, 1, 3 };

	glDrawElements(GL_TRIANGLES, size * 6, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

ControlRenderer::DrawInfo & ICSE::wnd::ControlRenderer::DrawUnit::GetDrawInfo(uint32_t control_id)
{
	return m_infos[m_infomap[control_id]];
}

void ICSE::wnd::ControlRenderer::DrawUnit::updateInfo(int index)
{
	Vertex pos[4];
	uint16_t indices[6];
	memset(indices, 0, sizeof(indices));
	memset(pos, 0, sizeof(pos));

	auto &info = m_infos[index];

	if (info.hasAttribute(DrawInfo::FLAG_UNUSED))
	{
		m_ibuf.uploadDataRange(sizeof(indices), indices, index * 2 * 6);
	}
	else
	{
		float rw = (float)info.bbox.GetWidth();
		float rh = (float)info.bbox.GetHeight();
		float rx = (float)info.bbox.GetX();
		float ry = (float)info.bbox.GetY();
		//float rw = 1.0f, rh = 1.0f, rx = 0.0f, ry = 0.0f;
		float tw = (float)info.texwidth / (float)MemCanvasRenderTexturePool::textureWidth();
		float th = (float)info.texheight / (float)MemCanvasRenderTexturePool::textureHeight();
		float tx = info.space.x * (float)MemCanvasRenderTexturePool::BLOCK_WIDTH / (float)MemCanvasRenderTexturePool::textureWidth();
		float ty = info.space.y * (float)MemCanvasRenderTexturePool::BLOCK_HEIGHT / (float)MemCanvasRenderTexturePool::textureHeight();
		pos[0].pos.x = rx; pos[0].pos.y = ry;
		pos[1].pos.x = rx + rw; pos[1].pos.y = ry;
		pos[2].pos.x = rx; pos[2].pos.y = ry + rh;
		pos[3].pos.x = rx + rw; pos[3].pos.y = ry + rh;
		pos[0].coord.x = tx; pos[0].coord.y = ty;
		pos[1].coord.x = tx + tw; pos[1].coord.y = ty;
		pos[2].coord.x = tx; pos[2].coord.y = ty + th;
		pos[3].coord.x = tx + tw; pos[3].coord.y = ty + th;
		m_vbuf.uploadData(sizeof(pos), pos);
		indices[0] = index * 4;
		indices[1] = index * 4 + 1;
		indices[2] = index * 4 + 2;
		indices[3] = index * 4 + 2;
		indices[4] = index * 4 + 1;
		indices[5] = index * 4 + 3;
		m_ibuf.uploadDataRange(sizeof(indices), indices, index * 2 * 6);
	}
	/*glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
	info.unsetAttribute(DrawInfo::FLAG_DIRTY);
}

void ICSE::wnd::ControlRenderer::DrawUnit::RemoveDrawInfo(uint32_t control_id)
{
	int index = m_infomap[control_id];
	m_infomap.erase(control_id);
	m_infos[index].unuse();
	m_infos[index].next_free = first_free;
	first_free = index;
}

void ICSE::wnd::ControlRenderer::DrawUnit::resizeVBO(int size)
{
	if (size > m_vbuf.size() / sizeof(Vertex))
	{
		m_vbuf.extend(size * sizeof(Vertex));
		m_ibuf.extend(size * 2 * 6);
		for (auto &info : m_infos)
			info.setAttribute(DrawInfo::FLAG_DIRTY);
	}
}

bool ICSE::wnd::ControlRenderer::DrawUnit::scavengeGC(void)
{
	if (first_free != -1)
	{
		int cur = first_free;
		first_free = m_infos[cur].next_free;
		m_infos[cur] = m_infos[m_infos.size() - 1];
		m_infos.pop_back();
		return true;
	}

	return false;
}

void ICSE::wnd::ControlRenderer::DrawUnit::fullGC(void)
{
	while (scavengeGC());
}

void ICSE::wnd::ControlRenderer::DrawInfo::unuse(void)
{
	setAttribute(FLAG_UNUSED | FLAG_DIRTY);
	control_id = 0;
}
