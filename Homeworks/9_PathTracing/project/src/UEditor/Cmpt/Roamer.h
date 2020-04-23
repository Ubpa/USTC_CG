#pragma once

#include <UDP/Basic/Read.h>
#include <UScene/core/Cmpt/Component.h>
#include <UScene/core/Cmpt/Transform.h>

namespace Ubpa::Cmpt {
	class L2W;
	class Position;
	class Rotation;

	class Roamer : public Component {
	public:
		float move_speed = 5.f;
		float rotate_speed = 0.1f;
		
		void OnUpdate(CmptTag::LastFrame<Cmpt::L2W> l2w, Position* pos, Rotation* rot);

		static void OnRegist();
	};
}
