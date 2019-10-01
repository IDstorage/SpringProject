#pragma once

#include "Spring_HPreHeader.h"

namespace spring {

	// IPointer's basic object type
	class IRef;

	template <class _Typ>
	class IPointer {

	public:
		IPointer() = default;
		IPointer(_Typ* obj) {
			this->object = obj;
			this->object->PlusRefCount(1);
		}
		IPointer(const _Typ& obj) : IPointer(&obj) { }
		IPointer(_Typ** obj) : IPointer(*obj) { }
		IPointer(const IPointer<_Typ>& ptr) {
			this->object = ptr.object;
			this->object->PlusRefCount(1);
		}
		// TODO: Remove if it's not nessesery anymore.
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

			object->PlusRefCount(-1);

			if (object->IsNoRef()) {
				delete object;
			}
		}

	private:
		IRef* object = nullptr;

	public:
		IPointer<_Typ>& operator=(_Typ* obj) {
			this->object = reinterpret_cast<IRef*>(obj);
			obj->PlusRefCount(1);
			return *this;
		}
		IPointer<_Typ>& operator=(const IPointer<_Typ>& ptr) {
			this->object = ptr.object;
			object->PlusRefCount(1);
			return *this;
		}

		_Typ& operator*() {
			return *reinterpret_cast<_Typ*>(object);
		}
		_Typ* operator->() {
			return reinterpret_cast<_Typ*>(object);
		}

		_Typ* Get() {
			return reinterpret_cast<_Typ*>(object);
		}

	public:
		bool IsNull() {
			return object == nullptr;
		}

	};

}