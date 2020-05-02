#include "Inspector.h"

#include "Hierarchy.h"

#include "detail/Inspector.inl"

#include <_deps/imgui/imgui.h>
#include <UScene/core.h>

#include <UEngine/Engine.h>

#include <UDP/Visitor.h>
#include <UDP/Reflection/VarPtrVisitor.h>
#include <UDP/Reflection/ReflTraits.h>
#include <UDP/Reflection/Reflection.h>

using namespace Ubpa;
using namespace std;

class Cmpt::Inspector::Viewer_Obj : public ReflTraitsVisitor
{
public:
	static Viewer_Obj& Instance() {
		static Viewer_Obj instance;
		return instance;
	}

private:
	Viewer_Obj() {
		ReflTraitsIniter::Instance().Init(*this);
	}

private:
	virtual void Receive(void* obj, string_view name, ReflectionBase& refl) override;
};

class Cmpt::Inspector::Viewer_Var :
	public VarPtrVisitor<void(Viewer_Var::*)(string_view, const string&, ReflectionBase&)> {
public:
	static Viewer_Var& Instance() {
		static Viewer_Var instance;
		return instance;
	}

private:
	Viewer_Var() {
		Register<
			Primitive*, Ubpa::Light*, Ubpa::Material*,
			Texture2D*,

			bool, float, int, size_t,

			valf1, valf2, valf3, valf4,
			//vali1, vali2, vali3, vali4,
			//valu1, valu2, valu3, valu4,

			vecf1, vecf2, vecf3, vecf4,
			//veci1, veci2, veci3, veci4,
			//vecu1, vecu2, vecu3, vecu4,

			pointf1, pointf2, pointf3, pointf4,
			//pointi1, pointi2, pointi3, pointi4,
			//pointu1, pointu2, pointu3, pointu4,

			scalef1, scalef2, scalef3, scalef4,

			rgbf>();
	}

protected:
	template<template<typename,size_t>class FloatVec,size_t N>
	void ImplVisit(FloatVec<float, N>& floatN, string_view classname, const string& name, ReflectionBase& refl) {
		float f32_max = numeric_limits<float>::max();
		float f32_min = -numeric_limits<float>::max();
		if constexpr (N == 1)
			ImGui::DragScalar((string(classname) + "::" + name).c_str(), ImGuiDataType_Float, floatN.data(), 0.01f, &f32_min, &f32_max);
		else if constexpr (N == 2)
			ImGui::DragFloat2((string(classname) + "::" + name).c_str(), floatN.data(), 0.01f, f32_min, f32_max);
		else if constexpr (N == 3)
			ImGui::DragFloat3((string(classname) + "::" + name).c_str(), floatN.data(), 0.01f, f32_min, f32_max);
		else if constexpr (N == 4)
			ImGui::DragFloat4((string(classname) + "::" + name).c_str(), floatN.data(), 0.01f, f32_min, f32_max);
		else
			static_assert(false, "N = 1, 2, 3, 4");
	};

	template<typename Obj>
	void ImplVisit(Obj* const& obj, string_view classname, const string& name, ReflectionBase& refl) {
		ImGui::Text(name.data());
		if(obj != nullptr)
			Viewer_Obj::Instance().Visit(obj);
		else
			ImGui::Text("null");
	};

	void ImplVisit(rgbf& c, string_view classname, const string& name, ReflectionBase& refl) {
		ImGui::ColorEdit3((string(classname)+"::"+name).c_str(), c.data());
	}

	void ImplVisit(float& f, string_view classname, const string& name, ReflectionBase& refl) {
		float f32_max = numeric_limits<float>::max();
		float f32_min = -numeric_limits<float>::max();
		
		// range
		auto range = refl.FieldMeta(name, ReflAttr::range);
		auto seperator = range.find(',');
		if (seperator != string::npos) {
			auto min_str = range.substr(0, seperator);
			auto max_str = range.substr(seperator + 1, range.size() - seperator - 1);
			if (!min_str.empty())
				f32_min = static_cast<float>(atof(min_str.data()));
			if (!max_str.empty())
				f32_max = static_cast<float>(atof(max_str.data()));
		}

		ImGui::DragScalar((string(classname) + "::" + name).c_str(), ImGuiDataType_Float, &f, 0.01f, &f32_min, &f32_max, "%f", 1.0f);
	};

	void ImplVisit(int& i, string_view classname, const string& name, ReflectionBase& refl) {
		int i_max = numeric_limits<int>::max();
		int i_min = -numeric_limits<int>::max();

		// range
		auto range = refl.FieldMeta(name, ReflAttr::range);
		auto seperator = range.find(',');
		if (seperator != string::npos) {
			auto min_str = range.substr(0, seperator);
			auto max_str = range.substr(seperator + 1, range.size() - seperator - 1);
			if (!min_str.empty())
				i_min = static_cast<size_t>(atoi(min_str.data()));
			if (!max_str.empty())
				i_max = static_cast<size_t>(atoi(max_str.data()));
		}

		ImGui::DragInt((string(classname) + "::" + name).c_str(), &i, 1, i_min, i_max);
	};

	void ImplVisit(size_t& s, string_view classname, const string& name, ReflectionBase& refl) {
		size_t s_max = numeric_limits<size_t>::max();
		size_t s_min = numeric_limits<size_t>::min();

		// range
		auto range = refl.FieldMeta(name, ReflAttr::range);
		auto seperator = range.find(',');
		if (seperator != string::npos) {
			auto min_str = range.substr(0, seperator);
			auto max_str = range.substr(seperator + 1, range.size() - seperator - 1);
			if (!min_str.empty())
				s_min = static_cast<size_t>(atoi(min_str.data()));
			if (!max_str.empty())
				s_max = static_cast<size_t>(atoi(max_str.data()));
		}

		ImGui::DragScalar((string(classname) + "::" + name).c_str(), ImGuiDataType_U64, &s, 1, &s_min, &s_max);
	};

	void ImplVisit(bool& b, string_view classname, const string& name, ReflectionBase& refl) {
		ImGui::Checkbox((string(classname) + "::" + name).c_str(), &b);
	};
};

class Cmpt::Inspector::Viewer_Cmpt : public ReflTraitsVisitor
{
public:
	static Viewer_Cmpt& Instance() {
		static Viewer_Cmpt instance;
		return instance;
	}

private:
	Viewer_Cmpt() {
		ReflTraitsIniter::Instance().Init(*this);
	}

private:
	virtual void Receive(void* obj, string_view classname, ReflectionBase& refl) override {
		if (ImGui::CollapsingHeader(classname.data())) {
			for (const auto& [name, var] : refl.VarPtrs(obj)) {
				if (Viewer_Var::Instance().IsRegistered(var))
					Viewer_Var::Instance().Visit(var, classname, name, refl);
			}
		}
	}
};

void Cmpt::Inspector::Viewer_Obj::Receive(void* obj, string_view classname, ReflectionBase& refl) {
	for (const auto& [name, var] : refl.VarPtrs(obj)) {
		if (Viewer_Var::Instance().IsRegistered(var))
			Viewer_Var::Instance().Visit(var, classname, name, refl);
	}
}

void Cmpt::Inspector::OnRegister() {
	detail::dynamic_reflection::ReflRegister_Inspector();
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
			Viewer_Cmpt::Instance().Visit(cmpt);
		ImGui::End();
	});
}

