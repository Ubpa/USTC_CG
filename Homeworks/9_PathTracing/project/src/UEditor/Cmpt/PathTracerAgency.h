#pragma once

#include <UDP/Basic/Read.h>
#include <UScene/core/Cmpt/Component.h>

namespace Ubpa {
	class BVH;
}

namespace Ubpa::Cmpt {
	class SObjPtr;
	class Material;
	class Camera;
	class L2W;
	class SObjPtr;

	class PathTracerAgency : public Component {
	public:
		void OnUpdate(const Cmpt::Camera* cam, const Cmpt::L2W* l2w, const SObjPtr* ptr);
		static void OnRegister();

	private:
		BVH* bvh{ nullptr };
	};
}
