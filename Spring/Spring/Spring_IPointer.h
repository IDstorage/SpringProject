#pragma once

#include "Spring_HPreHeader.h"

namespace spring {

	template <class _Typ>
	class IPointer {

	public:
		IPointer() = default;
		IPointer(_Typ* obj) {
			this->object = obj;
			this->object->refCount++;
		}
		IPointer(const _Typ& obj) {
			this->object = &obj;
			this->object->refCount++;
		}
		IPointer(_Typ** obj) {
			this->object = *obj;
			this->object->refCount++;
		}
		IPointer(const IPointer<_Typ>& ptr) {
			this->object = ptr.object;
			this->object->refCount++;
		}
		IPointer(std::nullptr_t nptr) {
			this->object = nullptr;
		}

		template <class _Child>
		static IPointer<_Typ> Cast(IPointer<_Child> child) {
			IPointer<_Typ> ptr = reinterpret_cast<_Typ*>(child.Get());
			return ptr;
		}

	public:
		~IPointer() {
			if (object == nullptr)
				return;

			if (--object->refCount == 0) {
				int a = 0;
				delete object;
			}
		}

	private:
		_Typ* object = nullptr;

	public:
		IPointer<_Typ>& operator=(_Typ* obj) {
			this->object = obj;
			obj->refCount++;
			return *this;
		}
		IPointer<_Typ>& operator=(const IPointer<_Typ>& ptr) {
			this->object = ptr.object;
			object->refCount++;
			return *this;
		}

		_Typ& operator*() {
			return *object;
		}
		_Typ* operator->() {
			return object;
		}

		_Typ* Get() {
			return object;
		}

	public:
		bool IsNull() {
			return object == nullptr;
		}

	};

}