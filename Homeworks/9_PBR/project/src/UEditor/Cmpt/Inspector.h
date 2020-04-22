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
		Inspector();

		void OnStart();
		void OnUpdate(const Hierarchy* hierarchy);
		void OnStop();

		static void OnRegist();

	protected:
		class Viewer_Cmpt;
		Viewer_Cmpt* viewer_cmpt{ nullptr };
	};
}
