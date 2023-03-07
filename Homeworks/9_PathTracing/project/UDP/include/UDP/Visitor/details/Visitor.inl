#include "../../Basic/vtable.h"

#include "../Basic/CustomID.h"

#include <UTemplate/Func.h>
#include <UTemplate/Concept.h>
#include <UTemplate/TypeID.h>

#include <unordered_map>
#include <functional>

namespace Ubpa::detail::Visitor_ {
	template<typename Impl, typename Func>
	Concept(HaveImplVisit, MemFuncOf<Func>::template run<Impl>(&Impl::ImplVisit));

	template<typename Impl>
	struct Accessor : Impl {
		template<typename DerivedPtr, typename Ret, typename... Args>
		static Ret run(Impl* impl, const void* obj, Args... args) {
			// impl->ImplVisit
			return (impl->*MemFuncOf<Ret(DerivedPtr, Args...)>::template run<Impl>(&Impl::ImplVisit))(reinterpret_cast<DerivedPtr>(const_cast<void*>(obj)), std::forward<Args>(args)...);
		}

		template<typename DerivedPtr, typename Ret, typename... Args>
		static Ret run(const Impl* impl, const void* obj, Args... args) {
			// impl->ImplVisit
			return (impl->*MemFuncOf<Ret(DerivedPtr, Args...)const>::template run<Impl>(&Impl::ImplVisit))(reinterpret_cast<DerivedPtr>(const_cast<void*>(obj)), std::forward<Args>(args)...);
		}

		template<typename T>
		static Impl* cast(T* ptr) noexcept {
			return static_cast<Impl*>(ptr);
		}

		template<typename T>
		static const Impl* cast(const T* ptr) noexcept {
			return static_cast<const Impl*>(ptr);
		}
	};
}
