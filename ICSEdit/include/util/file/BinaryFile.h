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
#include "util/container/Uint8Array.h"

namespace ICSE{
namespace file{

	class BinaryFile {
	protected:
		ICSE::container::Uint8Array m_data;

	public:
		static BinaryFile loadFromFile(const std::string &fileName);

		BinaryFile(void);
		~BinaryFile(void) = default;

		BinaryFile(const BinaryFile &file) = default;
		BinaryFile &operator=(const BinaryFile &file) = default;
		BinaryFile(BinaryFile &&file) = default;
		BinaryFile &operator=(BinaryFile &&file) = default;

		virtual uint32_t getSize(void) const { return this->m_data.getSize(); }
		virtual const uint8_t *getData(void) const { return this->m_data.getPtr(); }
	private:
		
	};

}
}