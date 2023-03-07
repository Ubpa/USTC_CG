#include "Hierarchy.h"

#include <_deps/imgui/imgui.h>
#include <UScene/core.h>

#include <UEngine/Engine.h>

#include "detail/Hierarchy.inl"

using namespace Ubpa;
using namespace std;

Cmpt::Hierarchy::Hierarchy()
    : base_flags{ ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth }
{
}

void Cmpt::Hierarchy::OnRegister() {
	detail::dynamic_reflection::ReflRegister_Hierarchy();
}

void Cmpt::Hierarchy::View_SObj(SObj* sobj) {
    ImGuiTreeNodeFlags node_flags = base_flags;
    if (selected_sobj == sobj)
        node_flags |= ImGuiTreeNodeFlags_Selected;
    if (sobj->children->size() == 0)
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    bool node_open = ImGui::TreeNodeEx(sobj, node_flags, sobj->name.c_str());
    if (ImGui::IsItemClicked())
        selected_sobj.val = sobj;
    if (node_open && sobj->children->size() != 0) {
        for (auto child : sobj->children)
            View_SObj(child);
        ImGui::TreePop();
    }
}

void Cmpt::Hierarchy::OnUpdate() {
    Engine::Instance().AddIMGUICommand([this]() {
        ImGui::Begin("Hierarchy");

        auto HelpMarker = [](const char* desc)
        {
            ImGui::TextDisabled("(?)");
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                ImGui::TextUnformatted(desc);
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }
        };
        HelpMarker("Multi-Scene Viewer"); // TODO: use reflection
        static bool align_label_with_current_x_position = false;
        /*ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnArrow", (unsigned int*)&base_flags, ImGuiTreeNodeFlags_OpenOnArrow);
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnDoubleClick", (unsigned int*)&base_flags, ImGuiTreeNodeFlags_OpenOnDoubleClick);
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAvailWidth", (unsigned int*)&base_flags, ImGuiTreeNodeFlags_SpanAvailWidth); ImGui::SameLine(); HelpMarker("Extend hit area to all available width instead of allowing more items to be layed out after the node.");
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanFullWidth", (unsigned int*)&base_flags, ImGuiTreeNodeFlags_SpanFullWidth);
        ImGui::Checkbox("Align label with current X position", &align_label_with_current_x_position);*/


        for (auto scene : SceneMngr::Instance().updatetingScenes) {
            if (ImGui::TreeNode(scene->root->name.c_str()))
            {
                if (align_label_with_current_x_position)
                    ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

                for (auto sobj : scene->root->children)
                    View_SObj(sobj);

                if (align_label_with_current_x_position)
                    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

                ImGui::TreePop();
            }
        }
        ImGui::End();
    });
}
