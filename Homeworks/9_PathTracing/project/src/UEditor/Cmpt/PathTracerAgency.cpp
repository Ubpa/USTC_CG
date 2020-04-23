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
				size_t width = 400;
				auto height = static_cast<size_t>(width / ar);
				Image img(width, height, 3);
				PathTracer path_tracer(SceneMngr::Instance().actived_scene, sobj, &img);
				path_tracer.Run();
				// post process

				// gamma correction
				for (size_t j = 0; j < height; j++) {
					for (size_t i = 0; i < width; i++) {
						auto& color = img.At<rgbf>(i, j);
						color[0] = std::sqrt(color[0]);
						color[1] = std::sqrt(color[1]);
						color[2] = std::sqrt(color[2]);
					}
				}
				// flip
				Image flipped_img(width, height, 3);
				for (size_t j = 0; j < height; j++) {
					for (size_t i = 0; i < width; i++)
						flipped_img.At<rgbf>(i, height - 1 - j) = img.At<rgbf>(i, j);
				}

				flipped_img.Save("../data/rst.png");
			});
			ptThread.detach();
			sobj->Detach<PathTracerAgency>();
		});
	}
}
