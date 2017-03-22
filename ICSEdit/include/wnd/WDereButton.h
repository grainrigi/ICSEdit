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
#include "wnd/WCanvasControl.h"
#include "wnd/WStaticText.h"
#include "wnd/AWButton.h"
#include "wnd/WindowLayer.h"
#include "util/basic/Singleton.h"
#include "util/file/ImageFile.h"
#include "graphics/Geometry.h"

namespace ICSE {
namespace wnd {

	enum eWDereButtonType{
		Red = 0x0001,
		White = 0x0000,
		Small = 0x0002, //Unstarred in white
		Large = 0x0000,
	};


	class WDereButton : public AWButton {
		static constexpr int BUTTON_WIDTH = 256 / 2;
		static constexpr int BUTTON_HEIGHT = 72 / 2;
		static constexpr int BUTTON_INNER_HEIGHT = 64 / 2;
		static constexpr int BUTTON_LARGE_DEFAULT_WIDTH = 256 / 2;
		static constexpr int BUTTON_SMALL_DEFAULT_WIDTH = 170 / 2;

		std::shared_ptr<WStaticText> m_static;
		std::shared_ptr<WCanvasControl> m_canvas;
		int m_type;
	public:
		WDereButton(std::shared_ptr<WindowLayer> layer, std::shared_ptr<font::Font> fnt, int type = eWDereButtonType::White | eWDereButtonType::Small, int width = 0);
		
		std::shared_ptr<WStaticText> Text(void) const { return m_static; }
		void UsePresetTextStyle(void);
		void UsePresetTextStyle(int type);

		void SetWidth(int width);
		void Move(int x, int y);


	private:
		void OnResize(int w, int h);
		void OnMove(int x, int y);
		void updateButtonCanvas();
		virtual void setPushing(bool pushing) override;
		int getMinWidth(void);

	public:
		class ImageManager {
			friend class Singleton<ImageManager>;
			std::unique_ptr<graphics::MemCanvasRGBA8> m_tex;

			

			static constexpr graphics::Position2D POS_WHITE_SMALL{ 0 / 2, 144 / 2 };
			static constexpr graphics::Position2D POS_WHITE_LARGE{ 0 / 2, 0 / 2 };
			static constexpr graphics::Position2D POS_RED_SMALL{ 256 / 2, 144 / 2 };
			static constexpr graphics::Position2D POS_RED_LARGE{ 256 / 2, 0 / 2 };
			static constexpr int WHITE_SMALL_LEFT_LIMIT = 8 / 2;
			static constexpr int WHITE_SMALL_RIGHT_LIMIT = 247 / 2;
			static constexpr int WHITE_LARGE_LEFT_LIMIT = 54 / 2;
			static constexpr int WHITE_LARGE_RIGHT_LIMIT = 194 / 2;
			static constexpr int RED_LARGE_LEFT_LIMIT = 54 / 2;
			static constexpr int RED_LARGE_RIGHT_LIMIT = 181 / 2;
			static constexpr int RED_SMALL_LEFT_LIMIT = 54 / 2;
			static constexpr int RED_SMALL_RIGHT_LIMIT = 196 / 2;
		public:

			static constexpr int RedSmallMinWidth();
			static constexpr int RedLargeMinWidth();
			static constexpr int WhiteSmallMinWidth();
			static constexpr int WhiteLargeMinWidth();

			void BakeRedSmall(graphics::MemCanvasRGBA8 &dest, int width, bool pressed);
			void BakeRedLarge(graphics::MemCanvasRGBA8 &dest, int width, bool pressed);
			void BakeWhiteSmall(graphics::MemCanvasRGBA8 &dest, int width, bool pressed);
			void BakeWhiteLarge(graphics::MemCanvasRGBA8 &dest, int width, bool pressed);
		private:
			ImageManager();
		};
	};

}
}
