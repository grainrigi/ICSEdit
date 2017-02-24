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

/**
*  @brief  シングルトンホルダー
*/
namespace ICSE {
	template <class _Ty, class... _Args>
	class Singleton
	{
	public:
		typedef _Ty InstanceType;

	public:
		/// 生成
		static void create(_Args... args)
		{
			if (_instance == NULL) {
				_instance = new InstanceType(args...);
			}
		}
		/// 破棄
		static void destroy()
		{
			if (_instance != NULL) {
				delete _instance;
				_instance = NULL;
			}
		}

	#if 1
		/// インスタンスゲット
		static InstanceType& getInstance()
		{
			return *_instance;
		}
	#else
		/// インスタンスゲット
		static InstanceType* getInstance()
		{
			return _instance;
		}
	#endif
		/// インスタンス参照ゲット
		static InstanceType& getInstanceRef()
		{
			return *_instance;
		}

		/// インスタンスポインタゲット
		static InstanceType* getInstancePtr()
		{
			return _instance;
		}

		/// 生成済み??
		static bool isCreate() { return _instance != NULL; };
		/// 破棄済み??
		static bool isDestroy() { return _instance == NULL; };

	private:
		static InstanceType* _instance; //!< インスタンス

	private:
		// 禁止
		Singleton() = delete;
		virtual ~Singleton() = delete;

		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		Singleton(const Singleton&&) = delete;
		Singleton& operator=(const Singleton&&) = delete;
	};

	template <class _Ty, class... _Args>
	typename Singleton<_Ty, _Args...>::InstanceType* Singleton<_Ty, _Args...>::_instance = NULL;

}