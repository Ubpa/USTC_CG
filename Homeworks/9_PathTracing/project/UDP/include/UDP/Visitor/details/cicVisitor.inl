#pragma once

namespace Ubpa {
	template<typename Impl, typename Ret, typename... Args>
	Ret Visitor<Ret(Impl::*)(const void*, Args...)const>::Visit(size_t ID, const void* ptr, Args... args) const {
		assert("ERROR::Visitor::Visit: unregistered" && IsRegistered(ID));
		auto target = impl_callbacks.find(ID);
		if (target != impl_callbacks.end())
			return target->second(detail::Visitor_::Accessor<Impl>::template cast(this), ptr, std::forward<Args>(args)...);
		else
			return this->callbacks.find(ID)->second(ptr, std::forward<Args>(args)...);
	}

	template<typename Impl, typename Ret, typename... Args>
	template<typename T>
	inline Ret Visitor<Ret(Impl::*)(const void*, Args...)const>::Visit(const T* ptr, Args... args) const {
		return Visit(CustomIDof(ptr), ptr, std::forward<Args>(args)...);
	}

	template<typename Impl, typename Ret, typename... Args>
	template<typename... Deriveds>
	void Visitor<Ret(Impl::*)(const void*, Args...)const>::Register() {
		(RegisterOne<Deriveds>(), ...);
	}

	template<typename Impl, typename Ret, typename... Args>
	template<typename Derived>
	void Visitor<Ret(Impl::*)(const void*, Args...)const>::RegisterOne() {
		impl_callbacks[CustomID<Derived>::get()] =
			[](const Impl* impl, const void* ptr, Args... args) {
				return detail::Visitor_::Accessor<Impl>::
					template run<const Derived*, Ret>(impl, ptr, std::forward<Args>(args)...);
			};
	}

	template<typename Impl, typename Ret, typename... Args>
	bool Visitor<Ret(Impl::*)(const void*, Args...)const>::IsRegistered(size_t ID) const {
		return impl_callbacks.find(ID) != impl_callbacks.end()
			|| this->callbacks.find(ID) != this->callbacks.end();
	}

	template<typename Impl, typename Ret, typename... Args>
	template<typename T>
	bool Visitor<Ret(Impl::*)(const void*, Args...)const>::IsRegistered() const {
		return IsRegistered(CustomID<T>());
	}

	template<typename Impl, typename Ret, typename... Args>
	bool Visitor<Ret(Impl::*)(const void*, Args...)const>::IsRegistered(const void* ptr) const {
		return IsRegistered(static_cast<size_t>(vtable(ptr)));
	}
}
