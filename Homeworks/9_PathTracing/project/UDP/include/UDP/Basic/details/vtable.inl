#pragma once

#include <UTemplate/Basic.h>

namespace Ubpa::detail::vtable_ {
	template<typename Base>
	struct Derived;
}

namespace Ubpa {
	template<typename T>
	inline const void* vtable_of<T>::get() noexcept {
		if (!value) {
			if constexpr (std::is_constructible_v<T>) {
				T tmp;
				regist(&tmp);
			}
			else if constexpr (is_derived_constructible_v<T>) {
				detail::vtable_::Derived<T> tmp{};
				regist(&tmp.t);
			}
			else
				assert("vtable_of::get: vtable regist fail");
		}
		return value;
	}
}

namespace Ubpa::detail::vtable_ {
	template<typename Base>
	struct Derived : Base {
		Derived() {}
		~Derived() {}
		Base t;
	};
}
