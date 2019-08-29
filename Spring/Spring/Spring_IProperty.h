#pragma once

#include <cassert>

namespace spring {

	enum class EIPropertySetting : int {
		GET_SET = 0,
		GET = 1,
		SET = 2,
		PRIVATE_GET = 4,
		PRIVATE_SET = 8
	};

	template <class _Typ, EIPropertySetting setting = EIPropertySetting::GET_SET>
	class IProperty {

	protected:
		_Typ data;

	public:
		IProperty(const _Typ& d = _Typ()) : data(d) {}
		IProperty(const IProperty<_Typ, setting>& p) : data(p.data) {}

		_Typ& operator=(const _Typ& d) {
			assert(setting != EIPropertySetting::GET);
			data = d;
			return data;
		}
		/*IProperty<_Typ, setting>& operator=(const IProperty<_Typ, setting>& d) {
			assert(setting != EIPropertySetting::GET);
			this->data = d.data;
			return *this;
		}*/

		operator const _Typ& () const {
			assert(setting != EIPropertySetting::SET);
			return data;
		}
	};


	/*template <class _Typ>
	class IProperty_Get : public IProperty<_Typ, EIPropertySetting::GET>;

	template <class _Typ>
	class IProperty_Set : public IProperty<_Typ, EIPropertySetting::SET>;*/

}