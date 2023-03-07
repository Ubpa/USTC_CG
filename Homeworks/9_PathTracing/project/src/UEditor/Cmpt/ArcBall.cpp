#include "ArcBall.h"

#include "Hierarchy.h"

#include "detail/ArcBall.inl"

#include <_deps/imgui/imgui.h>

using namespace Ubpa;

void Cmpt::ArcBall::OnRegister() {
	detail::dynamic_reflection::ReflRegister_ArcBall();
}

void Cmpt::ArcBall::OnUpdate(const Hierarchy* hierarchy) {
	auto& io = ImGui::GetIO();

	auto get_sphere_p = [=]() {
		vecf2 p{ io.MousePos[0] / io.DisplaySize[0], io.MousePos[1] / io.DisplaySize[1] };
		p = speed * (2 * p - vecf2{ 1.f });
		if (reverse_x)
			p[0] = -p[0];
		if (reverse_y)
			p[1] = -p[1];
		if (p.norm() > 1) {
			p.normalize_self();
			return vecf3{ p[0],p[1],0 };
		}
		else
			return vecf3{ p[0],p[1],std::sqrt(1 - p.norm2()) };
	};

	if (io.KeyAlt) {// Alt
		if (io.MouseClicked[0]) {
			rot = quatf::identity();
			sphere_p0 = get_sphere_p();
		}
		
		if (io.MouseDownDuration[0] > 0) {
			vecf3 sphere_p1 = get_sphere_p();
			if (sphere_p1 == sphere_p0)
				return;
			vecf3 axis = sphere_p0.cross(sphere_p1);
			auto q = quatf{ axis, std::acos(sphere_p0.cos_theta(sphere_p1)) };
			auto rotCmpt = hierarchy->selected_sobj->Get<Cmpt::Rotation>();
			rotCmpt->value = q * rot.inverse() * rotCmpt->value;
			rot = q;
		}
	}
}
