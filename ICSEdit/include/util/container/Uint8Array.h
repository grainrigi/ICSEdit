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

namespace ICSE{
namespace container{

	class Uint8Array {
		uint32_t m_size;
		uint8_t *m_array;
		std::function<void(uint8_t*)> m_deleter;

	public:
		Uint8Array(void);
		Uint8Array(uint32_t size);
		Uint8Array(uint8_t *ptr, uint32_t size, std::function<void(uint8_t*)> deleter);
		~Uint8Array(void);

		Uint8Array(const Uint8Array &ary);
		Uint8Array &operator=(const Uint8Array &ary);
		Uint8Array(Uint8Array &&ary);
		Uint8Array &operator=(Uint8Array &&ary);

		uint32_t getSize(void) const { return m_size; }
		uint8_t *getPtr(void) const { return this->m_array; }
	private:
		static void default_deleter(uint8_t *ptr);
	};

}
}