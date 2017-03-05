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
#include "util/basic/Delegate.h"

namespace ICSE {
namespace wnd {

	class AWControl {
		BoundingBox m_bb;

	public:
		AWControl(void);

		const BoundingBox &GetBB(void) const { return m_bb; }

		void Update(void) { dUpdate(); }
		

	protected:
		Delegate<> dUpdate;

		//These features are available but the accessibilities can be specified by the derived classes.
		Delegate<int, int> Move;
		Delegate<int, int> Resize;
	private:
		void move(int x, int y);
		void resize(int w, int h);
	};

}
}
