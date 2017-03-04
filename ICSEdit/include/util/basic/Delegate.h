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

namespace ICSE {

	template<typename ...Args>
	class Delegate {
	public:
		typedef std::function<void(Args...)> func_type;
	private:
		std::vector <func_type> m_funcs;

	public:

		Delegate(void)
		{
		}

		void  AddFunction(func_type func)
		{
			m_funcs.push_back(func);
		}

		/* std::function is not comparable
		void RemoveFunction(func_type func)
		{
			auto it = std::remove(m_funcs.begin(), m_funcs.end(), func);
			m_funcs.erase(it, m_funcs.end());
		}
		*/

		void InvokeAll(Args ...args)
		{
			for (auto fn : m_funcs)
				fn(args...);
		}

		Delegate &operator+=(func_type func) { AddFunction(func); return *this; }
		//Delegate &operator-=(func_type func) { RemoveFunction(func); return *this; }
		void operator()(Args ...args) { InvokeAll(args...); }

		Delegate(const Delegate&) = delete;
		Delegate(Delegate&&) = delete;
		Delegate &operator=(const Delegate&) = delete;
		Delegate &operator=(Delegate&&) = delete;
		
	private:
		
	};

}
