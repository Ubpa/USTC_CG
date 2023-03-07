#pragma once

#include <USTL/compress_pair.h>
#include <UTemplate/Func.h>

namespace Ubpa {
	template<typename T, typename Updator>
	class Dirty {
		static_assert(!std::is_same_v<T, Updator>);
		static_assert(!std::is_const_v<T> && !std::is_reference_v<T>);
		static_assert(std::is_same_v<T&, Front_t<FuncTraits_ArgList<Updator>>>);

	public:
		template<typename U = Updator, std::enable_if_t<!std::is_pointer_v<U>&& std::is_default_constructible_v<U>, int> = 0>
		constexpr Dirty() noexcept {}

		constexpr Dirty(const Dirty& rhs)
			: value{ USTL::one_then_variadic_args_t{}, rhs.value.get_first() } {}
		constexpr Dirty(Dirty&& rhs) noexcept
			: value{ std::move(rhs.value) } {}

		Dirty& operator=(const Dirty& rhs) {
			value.get_first() = rhs.value.get_first();
			dirty = true;
			return *this;
		}

		Dirty& operator=(Dirty&& rhs) noexcept {
			value = std::move(rhs.value);
			dirty = true;
			return *this;
		}

		constexpr Dirty(const Updator& updator) : value{ USTL::one_then_variadic_args_t{}, updator } {}
		constexpr Dirty(Updator&& updator) : value{ USTL::one_then_variadic_args_t{}, std::move(updator) } {}
		constexpr Dirty(const Updator& updator, T&& t) : value{ USTL::one_then_variadic_args_t{}, updator, std::move(t) } {}
		constexpr Dirty(Updator&& updator, T&& t) : value{ USTL::one_then_variadic_args_t{}, std::move(updator), std::move(t) } {}

		template<typename U = Updator, std::enable_if_t<!std::is_pointer_v<U>&& std::is_default_constructible_v<U>, int> = 0>
		constexpr Dirty(T&& t) : value{ USTL::zero_then_variadic_args_t{}, std::move(t) } {}

		template<typename... Args>
		constexpr Dirty(USTL::zero_then_variadic_args_t, Args&&... args)
			: value{ USTL::zero_then_variadic_args_t{}, std::forward<Args>(args)... }
		{
			static_assert(!std::is_pointer_v<Updator> && std::is_default_constructible_v<Updator>);
		}

		template<typename Arg0, typename... Args>
		constexpr Dirty(USTL::one_then_variadic_args_t, Arg0&& arg0, Args&&... args)
			: value{ USTL::one_then_variadic_args_t{}, std::forward<Arg0>(arg0), std::forward<Args>(args)... } {}

		bool IsDirty() const noexcept { return dirty; }
		void SetDirty() noexcept { dirty = true; }
		void ForceSetNonDirty() noexcept { dirty = false; }

		template<typename... Args>
		void Update(Args&&... args) {
			if (!dirty)
				return;
			value.get_first()(value.get_second(), std::forward<Args>(args)...);
			dirty = false;
		}

		T& DirectGet() noexcept {
			dirty = true;
			return value.get_second();
		}

		const T& DirectGet() const noexcept { return value.get_second(); }

		template<typename... Args>
		const T& Get(Args&&... args) {
			if (dirty) {
				value.get_first()(value.get_second(), std::forward<Args>(args)...);
				dirty = false;
			}
			return value.get_second();
		}

	private:
		USTL::compress_pair<Updator, T> value;
		bool dirty{ true };
	};

	template<typename T, typename... Args>
	using AutoDirty = Dirty<T, void(*)(T&, Args...)>;
}
