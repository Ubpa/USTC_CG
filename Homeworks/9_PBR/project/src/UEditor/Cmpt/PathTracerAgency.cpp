#include "PathTracerAgency.h"

#include "detail/PathTracerAgency.inl"

#include "../../PathTracer/PathTracer.h"
#include <UScene/core/Cmpt/Camera.h>
#include <UScene/core/Cmpt/L2W.h>
#include <UScene/core/Cmpt/SObjPtr.h>

#include <_deps/imgui/imgui.h>
#include <_deps/taskflow/taskflow.hpp>
#include <UBL/Image.h>
#include <UEngine/SceneMngr.h>

#include <UScene/tool/Accel/BVH.h>

#include <thread>

using namespace Ubpa;
using namespace std;

void Cmpt::PathTracerAgency::OnRegist() {
	detail::dynamic_reflection::ReflRegist_PathTracerAgency();
}

void Cmpt::PathTracerAgency::OnUpdate(const Cmpt::Camera* cam, const Cmpt::L2W* l2w, const Cmpt::SObjPtr* ptr) {
	auto& io = ImGui::GetIO();
	float ar = cam->ar;
	if (io.KeysDown['P']) {
		ptr->value->AddCommand([sobj = ptr->value.get(), ar]() {
			thread ptThread([sobj, ar]() {
				size_t width = 1024;
				auto height = static_cast<size_t>(width / ar);
				Image img(1024, height, 3);
				PathTracer path_tracer(SceneMngr::Instance().actived_scene, sobj, &img);
				path_tracer.Run();
				img.Save("../data/rst.png");
			});
			ptThread.detach();
			sobj->Detach<PathTracerAgency>();
		});
	}
}
