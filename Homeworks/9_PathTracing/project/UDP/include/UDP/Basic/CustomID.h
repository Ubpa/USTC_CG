#pragma once

namespace Ubpa {
	template<typename T>
	struct CustomID {
		// return custom ID of T
		// if T is polymorphic, then return it's virtual table
		// * it will create a object of T, and get the virtual table from the object
		// * if T is not default constructable, you should call vtable_of<T>::regist(ptr) firstly
		// * or call get(ptr)
		// else return TypeID<T>
		static constexpr size_t get() noexcept;

		// return custom ID of T
		// if T is polymorphic, then return it's virtual table getting from ptr
		// else return TypeID<T>
		static constexpr size_t get(const T* ptr) noexcept;
	};

	template<typename T>
	constexpr size_t CustomIDof(const T* ptr) noexcept {
		return CustomID<T>::get(ptr);
	}
}

#include "details/CustomID.inl"
