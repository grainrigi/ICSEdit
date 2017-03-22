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

#include "TestControlWindow.h"
#include "wnd/WindowManager.h"
#include "util/file/BinaryFile.h"
#include "util/font/FontRenderer.h"

using namespace ICSE;
using namespace ICSE::wnd;
using namespace ICSE::file;
using namespace ICSE::font;

ICSE::TestControlWindow::TestControlWindow(void)
	: ControlWindow("test", 1024, 768),
	m_memcanvas(1024, 768),
	m_mousex(0),
	m_mousey(0)
{
	dUpdate.PrependFunction([&]() { this->OnUpdate(); });
	this->OnInit();
	dOnEvent.AddFunction([&](SDL_Event& e) { this->OnEvt(e); });
}

void ICSE::TestControlWindow::OnEvt(SDL_Event & evt)
{
	if(evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE)
		ICSE::Singleton<ICSE::wnd::WindowManager>::getInstance().PostQuitMessage();
	else if(evt.type == SDL_MOUSEMOTION)
	{
		m_mousex = evt.motion.x;
		m_mousey = evt.motion.y;
	}
}

void ICSE::TestControlWindow::OnInit(void)
{
	this->MakeCurrent();
	m_mainlayer = this->createLayer();
	//m_canvas = m_mainlayer->CreateAscentCanvasControl<WCanvasControl>(1024, 768);


	BinaryFile bfnt = BinaryFile::LoadFromFile("fnt_b.otf");
	auto fnt = std::shared_ptr<Font>(new Font(Font::LoadFromFile(bfnt)));
	
	
	m_button = m_mainlayer->CreateControl<WDereButton>(m_mainlayer, fnt, eWDereButtonType::Small | eWDereButtonType::White, 85);
	m_button->UsePresetTextStyle();
	m_button->Text()->SetText(u8"キャンセル");
	m_button->Move(100, 100);

	auto b2 = m_mainlayer->CreateControl<WDereButton>(m_mainlayer, fnt, eWDereButtonType::Large | eWDereButtonType::Red, 85);
	b2->UsePresetTextStyle();
	b2->Text()->SetText(u8"OK");
	b2->Move(190, 100);

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	/*m_button->OnClick += [&]()
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
			u8"結果",
			u8"はずれです",
			window());
	};*/
	

	//memcpy(cvs->pixels(), m_memcanvas.pixels(), 1024 * 768 * 4);
	//m_canvas->Unlock();
}

void ICSE::TestControlWindow::OnUpdate(void)
{
	//m_button->Move(m_mousex, m_mousey);
}
