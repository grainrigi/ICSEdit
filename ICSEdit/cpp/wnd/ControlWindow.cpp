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

#include "wnd/ControlWindow.h"
#include "graphics/drawenv.h"

using namespace ICSE::sdl;
using namespace ICSE::wnd;
using namespace ICSE::graphics;

void ICSE::wnd::ControlWindow::Update(void)
{
	dUpdate();
}

void ICSE::wnd::ControlWindow::MakeCurrent(void)
{
	SDL_GL_MakeCurrent(this->window(), m_context);
}

std::shared_ptr<WindowLayer> ICSE::wnd::ControlWindow::createLayer(void)
{
	std::shared_ptr<WindowLayer> layer(new WindowLayer(this, &m_ctlshader));
	m_layers.insert(std::make_pair(layer->id(), layer));
	return layer;
}

void ICSE::wnd::ControlWindow::removeLayer(uint32_t id)
{
	auto it = m_layers.find(id);
	if (it != m_layers.end())
		m_layers.erase(it);
}

ICSE::wnd::ControlWindow::ControlWindow(const char *name, uint32_t width, uint32_t height, int flags)
	: SDLWindow(name, width, height, flags | SDL_WINDOW_OPENGL)
{
	initContextAndRenderer();
	dUpdate += [&]() { this->OnUpdate(); };
}

void ICSE::wnd::ControlWindow::initContextAndRenderer(void)
{
	m_context = SDL_GL_CreateContext(this->window());
	SDL_GL_MakeCurrent(this->window(), m_context);

	m_m2drend.LateInitialize();
	m_cvsrend.LateInitialize(&m_m2drend);
	m_ctlshader.LateInitialize();
}

void ICSE::wnd::ControlWindow::OnUpdate(void)
{
	DrawEnv env;
	env.setWidth(width());
	env.setHeight(height());

	this->MakeCurrent();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto &layer : m_layers)
	{
		layer.second->UpdateAll();
		layer.second->RenderAll(&env);
	}

	SDL_GL_SwapWindow(this->window());
}
