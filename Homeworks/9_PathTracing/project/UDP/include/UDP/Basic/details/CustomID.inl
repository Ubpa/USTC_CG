#pragma once

#include "../vtable.h"
#include <UTemplate/TypeID.h>

namespace Ubpa {
	template<typename T>
	constexpr size_t CustomID<T>::get() noexcept {
		static_assert(!std::is_const_v<T> && !std::is_pointer_v<T>,
			"GetID: <T> must be non-const and non-pointer");
		if constexpr (std::is_polymorphic_v<T>)
			return reinterpret_cast<size_t>(vtable_of<T>::get());
		else
			return Ubpa::TypeID<T>;
	}

	template<typename T>
	constexpr size_t CustomID<T>::get(const T* ptr) noexcept {
		assert(ptr != nullptr);
		if constexpr (std::is_void_v<T> || std::is_polymorphic_v<T>)
			return reinterpret_cast<size_t>(vtable(ptr));
		else
			return Ubpa::TypeID<T>;
	}
}
