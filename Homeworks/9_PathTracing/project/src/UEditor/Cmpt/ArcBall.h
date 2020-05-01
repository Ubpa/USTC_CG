#pragma once

#include <UScene/core/Cmpt/Component.h>
#include <UGM/UGM.h>

namespace Ubpa::Cmpt {
	class Hierarchy;

	class ArcBall : public Component {
	public:
		bool reverse_x{ false };
		bool reverse_y{ true };
		float speed{ 1.f };

		void OnUpdate(const Hierarchy* hierarchy);

		static void OnRegister();
		
	private:
		vecf3 sphere_p0;
		quatf rot;
	};
}
