#pragma once

#include <UDP/Basic/Read.h>
#include <UScene/core/Cmpt/Component.h>

namespace Ubpa::Cmpt {
	class SObjPtr;
	class Material;
	class Camera;
	class L2W;
	class SObjPtr;

	class PathTracerAgency : public Component {
	public:
		[[range("1,")]]
		size_t spp{ 2 };
		[[range("100,")]]
		size_t width{ 400 };

		void OnUpdate(const Cmpt::Camera* cam, const Cmpt::L2W* l2w, const SObjPtr* ptr);
		static void OnRegister();
	};
}
