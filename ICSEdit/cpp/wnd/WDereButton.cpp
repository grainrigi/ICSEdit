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

#include "wnd/WDereButton.h"
#include "util/file/ResourceManager.h"

using namespace ICSE::wnd;
using namespace ICSE::file;
using namespace ICSE::graphics;

ICSE::wnd::WDereButton::WDereButton(std::shared_ptr<WindowLayer> layer, std::shared_ptr<font::Font> fnt, int type, int width)
	: m_type(type)
{
	int w;

	if (width <= 0 || width > BUTTON_WIDTH)
	{
		if ((type & 0x02) == eWDereButtonType::Small)
			w = BUTTON_SMALL_DEFAULT_WIDTH;
		else
			w = BUTTON_LARGE_DEFAULT_WIDTH;
	}
	else
		w = width;

	this->Resize(w, BUTTON_HEIGHT);

	m_static = layer->CreateAscentCanvasControl<WStaticText>(fnt);
	m_static->Resize(w, BUTTON_INNER_HEIGHT);
	m_static->SetAlign(eWStaticTextAlign::CenterHorz | eWStaticTextAlign::CenterVert);
	m_canvas = layer->CreateDescentCanvasControl<WCanvasControl>(w, BUTTON_HEIGHT);
	m_canvas->Lock()->ClearWithColor(0x00000000);
	m_canvas->Unlock();

	this->Resize += [&](int w, int h) { this->OnResize(w, h); };
	this->AWControl::Move += [&](int x, int y) { this->OnMove(x, y); };

	this->SetWidth(w);
	this->Move(0, 0);
}

void ICSE::wnd::WDereButton::UsePresetTextStyle(void)
{
	UsePresetTextStyle(m_type);
}

void ICSE::wnd::WDereButton::UsePresetTextStyle(int type)
{
	if((type & 0x01) == eWDereButtonType::Red)
	{
		m_static->SetFontColor(0xffffffff);
		m_static->SetOutlineColor(0xff290057);
		m_static->SetOutlineSize(1);
	}
	else
	{
		m_static->SetFontColor(0xff464646);
		m_static->SetOutlineColor(0xff000000);
		m_static->SetOutlineSize(0);
	}
	if ((type & 0x02) == eWDereButtonType::Large)
		m_static->SetFontSize(32 / 2);
	else
		m_static->SetFontSize(28 / 2);
}

void ICSE::wnd::WDereButton::SetWidth(int width)
{
	if (width > BUTTON_WIDTH)
		width = BUTTON_WIDTH;
	else if (width < this->getMinWidth())
		width = this->getMinWidth();

	this->Resize(width, BUTTON_HEIGHT);
	this->updateButtonCanvas();
}

void ICSE::wnd::WDereButton::Move(int x, int y)
{
	this->AWControl::Move(x, y);
}

void ICSE::wnd::WDereButton::OnResize(int w, int h)
{
	m_canvas->Resize(w, BUTTON_HEIGHT);
	m_static->Resize(w, BUTTON_INNER_HEIGHT);
}

void ICSE::wnd::WDereButton::OnMove(int x, int y)
{
	m_canvas->Move(x, y);
	if (pushing())
		m_static->Move(x, y + (BUTTON_HEIGHT - BUTTON_INNER_HEIGHT) + STATIC_Y_OFFSET);
	else
		m_static->Move(x, y + STATIC_Y_OFFSET);
}

void ICSE::wnd::WDereButton::updateButtonCanvas()
{
	int width = m_canvas->GetBB().GetWidth();
	this->Resize(width, BUTTON_HEIGHT);

	switch(m_type)
	{
		case eWDereButtonType::White | eWDereButtonType::Small:
			Singleton<ImageManager>::getInstance().
				BakeWhiteSmall(*m_canvas->Lock(), width, pushing());
			break;
		case eWDereButtonType::White | eWDereButtonType::Large:
			Singleton<ImageManager>::getInstance().
				BakeWhiteLarge(*m_canvas->Lock(), width, pushing());
			break;
		case eWDereButtonType::Red | eWDereButtonType::Small:
			Singleton<ImageManager>::getInstance().
				BakeRedSmall(*m_canvas->Lock(), width, pushing());
			break;
		case eWDereButtonType::Red | eWDereButtonType::Large:
			Singleton<ImageManager>::getInstance().
				BakeRedLarge(*m_canvas->Lock(), width, pushing());
			break;
	}

	m_canvas->Unlock();
}

void ICSE::wnd::WDereButton::setPushing(bool pushing)
{
	auto& bb = this->GetBB();
	this->Move(bb.GetX(), bb.GetY());

	updateButtonCanvas();
}

int ICSE::wnd::WDereButton::getMinWidth(void)
{
	switch (m_type)
	{
		case eWDereButtonType::White | eWDereButtonType::Small:
			return ImageManager::WhiteSmallMinWidth();
		case eWDereButtonType::White | eWDereButtonType::Large:
			return ImageManager::WhiteLargeMinWidth();
		case eWDereButtonType::Red | eWDereButtonType::Small:
			return ImageManager::RedSmallMinWidth();
		case eWDereButtonType::Red | eWDereButtonType::Large:
			return ImageManager::RedLargeMinWidth();
	}

	return -1;
}

constexpr int ICSE::wnd::WDereButton::ImageManager::RedSmallMinWidth()
{
	return BUTTON_WIDTH - (RED_SMALL_RIGHT_LIMIT - RED_SMALL_LEFT_LIMIT);
}

constexpr int ICSE::wnd::WDereButton::ImageManager::RedLargeMinWidth()
{
	return BUTTON_WIDTH - (RED_LARGE_RIGHT_LIMIT - RED_LARGE_LEFT_LIMIT);
}

constexpr int ICSE::wnd::WDereButton::ImageManager::WhiteSmallMinWidth()
{
	return BUTTON_WIDTH - (WHITE_SMALL_RIGHT_LIMIT - WHITE_SMALL_LEFT_LIMIT);
}

constexpr int ICSE::wnd::WDereButton::ImageManager::WhiteLargeMinWidth()
{
	return BUTTON_WIDTH - (WHITE_LARGE_RIGHT_LIMIT - WHITE_LARGE_LEFT_LIMIT);
}

void ICSE::wnd::WDereButton::ImageManager::BakeRedSmall(graphics::MemCanvasRGBA8 & dest, int width, bool pressed)
{
	assert(width <= dest.width());
	assert(width <= BUTTON_WIDTH);
	assert(BUTTON_HEIGHT <= dest.height());

	int offy = pressed ? BUTTON_HEIGHT : 0;

	if (width < BUTTON_WIDTH)
	{
		int separatePoint = width - (BUTTON_WIDTH - RED_SMALL_RIGHT_LIMIT);

		MemCanvasRGBA8::BitBlt(dest, 0, 0, separatePoint, BUTTON_HEIGHT,
			*m_tex, POS_RED_SMALL.x, POS_RED_SMALL.y + offy);
		MemCanvasRGBA8::BitBlt(dest, separatePoint, 0, (BUTTON_WIDTH - RED_SMALL_RIGHT_LIMIT), BUTTON_HEIGHT,
			*m_tex, POS_RED_SMALL.x + RED_SMALL_RIGHT_LIMIT, POS_RED_SMALL.y + offy);
	}
	else
	{
		MemCanvasRGBA8::BitBlt(dest, 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT,
			*m_tex, POS_RED_SMALL.x, POS_RED_SMALL.y + offy);
	}
}

void ICSE::wnd::WDereButton::ImageManager::BakeRedLarge(graphics::MemCanvasRGBA8 & dest, int width, bool pressed)
{
	assert(width <= dest.width());
	assert(width <= BUTTON_WIDTH);
	assert(BUTTON_HEIGHT <= dest.height());

	int offy = pressed ? BUTTON_HEIGHT : 0;

	if (width < BUTTON_WIDTH)
	{
		int separatePoint = width - (BUTTON_WIDTH - RED_LARGE_RIGHT_LIMIT);

		MemCanvasRGBA8::BitBlt(dest, 0, 0, separatePoint, BUTTON_HEIGHT,
			*m_tex, POS_RED_LARGE.x, POS_RED_LARGE.y + offy);
		MemCanvasRGBA8::BitBlt(dest, separatePoint, 0, (BUTTON_WIDTH - RED_LARGE_RIGHT_LIMIT), BUTTON_HEIGHT,
			*m_tex, POS_RED_LARGE.x + RED_LARGE_RIGHT_LIMIT, POS_RED_LARGE.y + offy);
	}
	else
	{
		MemCanvasRGBA8::BitBlt(dest, 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT,
			*m_tex, POS_RED_LARGE.x, POS_RED_LARGE.y + offy);
	}
}

void ICSE::wnd::WDereButton::ImageManager::BakeWhiteSmall(graphics::MemCanvasRGBA8 & dest, int width, bool pressed)
{
	assert(width <= dest.width());
	assert(width <= BUTTON_WIDTH);
	assert(BUTTON_HEIGHT <= dest.height());

	int offy = pressed ? BUTTON_HEIGHT : 0;

	if (width < BUTTON_WIDTH)
	{
		int separatePoint = width - (BUTTON_WIDTH - WHITE_SMALL_RIGHT_LIMIT);

		MemCanvasRGBA8::BitBlt(dest, 0, 0, separatePoint, BUTTON_HEIGHT,
			*m_tex, POS_WHITE_SMALL.x, POS_WHITE_SMALL.y + offy);
		MemCanvasRGBA8::BitBlt(dest, separatePoint, 0, (BUTTON_WIDTH - WHITE_SMALL_RIGHT_LIMIT), BUTTON_HEIGHT,
			*m_tex, POS_WHITE_SMALL.x + WHITE_SMALL_RIGHT_LIMIT, POS_WHITE_SMALL.y + offy);
	}
	else
	{
		MemCanvasRGBA8::BitBlt(dest, 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT,
			*m_tex, POS_WHITE_SMALL.x, POS_WHITE_SMALL.y + offy);
	}
}

void ICSE::wnd::WDereButton::ImageManager::BakeWhiteLarge(graphics::MemCanvasRGBA8 & dest, int width, bool pressed)
{
	assert(width <= dest.width());
	assert(width <= BUTTON_WIDTH);
	assert(BUTTON_HEIGHT <= dest.height());

	int offy = pressed ? BUTTON_HEIGHT : 0;

	if (width < BUTTON_WIDTH)
	{
		int separatePoint = width - (BUTTON_WIDTH - WHITE_LARGE_RIGHT_LIMIT);

		MemCanvasRGBA8::BitBlt(dest, 0, 0, separatePoint, BUTTON_HEIGHT,
			*m_tex, POS_WHITE_LARGE.x, POS_WHITE_LARGE.y + offy);
		MemCanvasRGBA8::BitBlt(dest, separatePoint, 0, (BUTTON_WIDTH - WHITE_LARGE_RIGHT_LIMIT), BUTTON_HEIGHT,
			*m_tex, POS_WHITE_LARGE.x + WHITE_LARGE_RIGHT_LIMIT, POS_WHITE_LARGE.y + offy);
	}
	else
	{
		MemCanvasRGBA8::BitBlt(dest, 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT,
			*m_tex, POS_WHITE_LARGE.x, POS_WHITE_LARGE.y + offy);
	}
}

ICSE::wnd::WDereButton::ImageManager::ImageManager()
{
	auto texfile = ResourceManagerSingleton::getInstance().LoadResource("img/button_tex_shrink.png");
	m_tex.reset(new MemCanvasRGBA8(ImageFile::LoadFromFile(*texfile, ImageFile::FORMAT_RGBA).CreateMemCanvas()));
}
