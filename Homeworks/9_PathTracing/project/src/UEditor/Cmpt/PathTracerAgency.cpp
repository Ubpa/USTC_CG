#include "PathTracerAgency.h"

#include "detail/PathTracerAgency.inl"

#include "../../PathTracer/PathTracer.h"
#include <UScene/core/Cmpt/Camera.h>
#include <UScene/core/Cmpt/L2W.h>
#include <UScene/core/Cmpt/SObjPtr.h>

#include <_deps/imgui/imgui.h>
#include <UBL/Image.h>
#include <UEngine/SceneMngr.h>

#include <UScene/tool/Accel/BVH.h>

#include <thread>

using namespace Ubpa;
using namespace std;

void Cmpt::PathTracerAgency::OnRegister() {
	detail::dynamic_reflection::ReflRegister_PathTracerAgency();
}

void Cmpt::PathTracerAgency::OnUpdate(const Cmpt::Camera* cam, const Cmpt::L2W* l2w, const Cmpt::SObjPtr* ptr) {
	auto& io = ImGui::GetIO();
	float ar = cam->ar;
	if (io.KeysDown['P']) {
		ptr->value->AddCommand([spp = this->spp, width = this->width, sobj = ptr->value.get(), ar]() {
			thread ptThread([spp, width, sobj, ar]() {
				auto height = static_cast<size_t>(width / ar);
				Image img(width, height, 3);
				PathTracer path_tracer(SceneMngr::Instance().actived_scene, sobj, &img, spp);
				path_tracer.Run();
				// post process

				// Reinhard tone mapping, gamma correction
				for (size_t j = 0; j < height; j++) {
					for (size_t i = 0; i < width; i++) {
						auto& color = img.At<rgbf>(i, j);

						color[0] = color[0] / (color[0] + 1);
						color[1] = color[1] / (color[1] + 1);
						color[2] = color[2] / (color[2] + 1);

						color[0] = std::pow(color[0], 1.f / 2.2f);
						color[1] = std::pow(color[1], 1.f / 2.2f);
						color[2] = std::pow(color[2], 1.f / 2.2f);
					}
				}

				img.Save("../data/rst.png", true);
			});
			ptThread.detach();
			sobj->Detach<PathTracerAgency>();
		});
	}
}
