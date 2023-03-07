#pragma once

#include "cVisitor.h"

namespace Ubpa {
	// non-const impl const visitor
	template<typename Impl, typename Ret, typename... Args>
	class Visitor<Ret(Impl::*)(const void*, Args...)> : private Visitor<Ret(const void*, Args...)> {
	public:
		using Visitor<Ret(const void*, Args...)>::Register;

		inline Ret Visit(size_t ID, const void* ptr, Args... args);

		template<typename T>
		inline Ret Visit(const T* ptr, Args... args);

		inline bool IsRegistered(size_t ID) const;
		template<typename T>
		inline bool IsRegistered() const;

		inline bool IsRegistered(const void* ptr) const;

	protected:
		// ImplVisit
		template<typename... Deriveds>
		void Register();

	private:
		// ImplVisit
		template<typename Derived>
		void RegisterOne();

	private:
		std::unordered_map<size_t, std::function<Ret(Impl*, const void*, Args...)>> impl_callbacks;
	};
}

#include "details/ncicVisitor.inl"
