#pragma once

#include <UScene/core/Cmpt/Component.h>

#include <UDP/Basic/Read.h>

namespace Ubpa {
	class SObj;
}

namespace Ubpa::Cmpt {
	class
	[[description("Multi-Scene Viewer")]]
	Hierarchy : public Component {
	public:
		Read<Hierarchy, SObj*> selected_sobj;

		Hierarchy();

		void OnUpdate();

		static void OnRegister();

	private:
		void View_SObj(SObj* sobj);
		
		int base_flags;// ImGuiTreeNodeFlags
	};
}
