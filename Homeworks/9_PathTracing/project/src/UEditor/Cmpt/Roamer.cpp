#include "Roamer.h"

#include "detail/Roamer.inl"

#include <_deps/imgui/imgui.h>
#include <UScene/core.h>

using namespace Ubpa;
using namespace std;

void Cmpt::Roamer::OnRegist() {
	detail::dynamic_reflection::ReflRegister_Roamer();
}

void Cmpt::Roamer::OnUpdate(CmptTag::LastFrame<Cmpt::L2W> l2w, Position* pos, Rotation* rot) {
	auto& io = ImGui::GetIO();

	auto forward = -l2w->FrontInWorld(); // camera's forward is (0, 0, -1) in local
	auto right = l2w->RightInWorld();
	vecf3 worldUp{ 0,1,0 };

	float dt = 0.0167f;// io.DeltaTime;
	float dx = io.MouseDelta[0];
	float dy = io.MouseDelta[1];

	// TODO: key enum
	vecf3 move{ 0.f };
	if (io.KeysDown['W'])
		move += move_speed * dt * forward;
	if (io.KeysDown['S'])
		move -= move_speed * dt * forward;
	if (io.KeysDown['A'])
		move -= move_speed * dt * right;
	if (io.KeysDown['D'])
		move += move_speed * dt * right;
	if (io.KeysDown['Q'])
		move += move_speed * dt * worldUp;
	if (io.KeysDown['E'])
		move -= move_speed * dt * worldUp;
	pos->value += move;

	// right button
	if (io.MouseDown[1]) {
		quatf rx{ worldUp, - rotate_speed * dt * dx };
		quatf ry{ right, -rotate_speed * dt * dy };
		rot->value = rx * ry * rot->value;
	}
}
