#include "Inspector.h"

#include "Hierarchy.h"

#include "detail/Inspector.inl"

#include <_deps/imgui/imgui.h>
#include <UScene/core.h>

#include <UEngine/Engine.h>

#include <UDP/Visitor.h>
#include <UDP/Reflection/VarPtrVisitor.h>
#include <UDP/Reflection/ReflTraits.h>

using namespace Ubpa;
using namespace std;

class Cmpt::Inspector::Viewer_Cmpt : public ReflTraitsVisitor {
public:
	Viewer_Cmpt() {
		ReflTraitsIniter::Instance().Init(*this);
	}

private:
	// obj is used for special visit (not just name and vars)
	virtual void Receive(void* obj, std::string_view name, ReflectionBase& refl) override {
		if (ImGui::CollapsingHeader(name.data())) {
			for (const auto& [name, var] : refl.VarPtrs(obj)) {
				ImGui::Text(name.data());
				// TODO: edit
			}
		}
	};
};

Cmpt::Inspector::Inspector() = default;

void Cmpt::Inspector::OnStart() {
	viewer_cmpt = new Viewer_Cmpt;
}

void Cmpt::Inspector::OnStop() {
	delete viewer_cmpt;
	viewer_cmpt = nullptr;
}

void Cmpt::Inspector::OnRegist() {
	detail::dynamic_reflection::ReflRegist_Inspector();
}

void Cmpt::Inspector::OnUpdate(const Hierarchy* hierarchy) {
	if (!hierarchy->selected_sobj)
		return;

	Engine::Instance().AddIMGUICommand([this, sobj = hierarchy->selected_sobj]() {
		ImGui::Begin("Inspector");
		ImGui::Text(("name: " + sobj->name).c_str());
		ImGui::Text(("number of children: " + to_string(sobj->children->size())).c_str());
		auto cmpts = sobj->Components();
		ImGui::Text(("number of components: " + to_string(cmpts.size())).c_str());
		for (auto [cmpt, size] : cmpts)
			viewer_cmpt->Visit(cmpt);
		ImGui::End();
	});
}

