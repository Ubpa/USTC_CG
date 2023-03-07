#pragma once

#include "Connection.h"

#include <map>
#include <functional>

namespace Ubpa {
	template<typename... Args>
	class Signal {
	public:
		template<typename Slot>
		Connection Connect(Slot&& slot);

		void Emit(Args... args) const;

		void Disconnect(Connection&& connection);

	private:
		size_t id{ 0 };
		std::map<size_t, std::function<void(Args...)>> slots;

	};
}

#include "details/Signal.inl"
