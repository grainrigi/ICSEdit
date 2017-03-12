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

#include "wnd/WindowLayer.h"


using namespace ICSE::wnd;
using namespace ICSE::graphics;


ICSE::wnd::WindowLayer::WindowLayer(ControlWindow * parent, ControlRendererShader *shader)
	: m_parent{ parent },
	m_id(WindowLayer::ObtainID()),
	m_ascent(shader), m_descent(shader)
{
}

void ICSE::wnd::WindowLayer::RemoveControl(uint32_t id)
{
	auto it = m_controls.find(id);
	if (it != m_controls.end())
	{
		m_controls.erase(it);
	}
	else
	{
		auto it = m_ascentcontrols.find(id);
		if(it != m_ascentcontrols.end())
		{
			m_ascent.RemoveControl(it->second->m_canvas.txunitid(), id);
			m_ascentcontrols.erase(it);
		}
		else
		{
			auto it = m_descentcontrols.find(id);
			if(it != m_descentcontrols.end())
			{
				m_descent.RemoveControl(it->second->m_canvas.txunitid(), id);
				m_descentcontrols.erase(it);
			}
		}
	}
}

void ICSE::wnd::WindowLayer::UpdateAll(void)
{
	for (auto &control : m_controls)
		control.second->Update();
	for (auto &control : m_ascentcontrols)
		control.second->Update();
	for (auto &control : m_descentcontrols)
		control.second->Update();
}

void ICSE::wnd::WindowLayer::RenderAll(DrawEnv *env)
{
	m_descent.RenderAll(env);
	m_ascent.RenderAll(env);
}

uint32_t ICSE::wnd::WindowLayer::ObtainID(void)
{
	std::atomic_uint32_t ctr(0);

	return ctr++;
}
