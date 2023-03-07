#pragma once

#include <UTemplate/Func.h>

#include <tuple>

namespace Ubpa {
	template<typename... Args>
	template<typename Slot>
	Connection Signal<Args...>::Connect(Slot&& slot) {
		using SlotArgList = typename FuncTraits<Slot>::ArgList;
		slots[id] = FuncExpand<void(Args...)>::template run(std::forward<Slot>(slot));
		return id++;
	}

	template<typename... Args>
	void Signal<Args...>::Emit(Args... args) const {
		for (auto p : slots)
			p.second(args...);
	}

	template<typename... Args>
	void Signal<Args...>::Disconnect(Connection&& connection) {
		slots.erase(connection.id);
		connection.id = static_cast<size_t>(-1);
	}
}
