#pragma once

#include <UScene/core/Cmpt/Component.h>

#include <UDP/Basic/Read.h>

#include <UGM/UGM.h>

namespace Ubpa {
	class SObj;
}

namespace Ubpa::Cmpt {
	class Hierarchy;

	class
	[[description("SObj's components Viewer")]]
	Inspector : public Component/*, public VarPtrVisitor<Inspector>*/ {
	public:

		void OnUpdate(const Hierarchy* hierarchy);

		static void OnRegister();

	protected:
		class Viewer_Cmpt;
		class Viewer_Obj;
		class Viewer_Var;
	};
}
