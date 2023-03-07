#pragma once

namespace Ubpa {
	// [Func]
	// - Ret([const] void*, Args...)
	// - Ret(Impl::*)([const] void*, Args...)[const]
	// [ID] vtable, TypeID, customed ID
	template<typename Func>
	class Visitor;
}

#include "details/Visitor.inl"
