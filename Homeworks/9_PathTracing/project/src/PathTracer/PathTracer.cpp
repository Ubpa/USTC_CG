#include "PathTracer.h"

#include <UBL/Image.h>

#include <iostream>

using namespace Ubpa;
using namespace std;

PathTracer::PathTracer(const Scene* scene, const SObj* cam_obj, Image* img)
	: scene{ scene },
	bvh{ const_cast<Scene*>(scene) },
	img{ img },
	cam{ cam_obj->Get<Cmpt::Camera>() },
	ccs{ cam->GenCoordinateSystem(cam_obj->Get<Cmpt::L2W>()->value) }
{
	scene->Each([this](const Cmpt::Light* light) ->bool {
		if (!vtable_is<EnvLight>(light->light.get()))
			return true; // continue

		env_light = static_cast<const EnvLight*>(light->light.get());
		return false; // stop
	});
}

void PathTracer::Run() {
	img->SetAll(0.f);

	const size_t spp = 2; // samples per pixel

	Intersectors intersectors;

	for (size_t j = 0; j < img->height; j++) {
		for (size_t i = 0; i < img->width; i++) {
			for (size_t k = 0; k < spp; k++) {
				float u = (i + rand01<float>() - 0.5f) / img->width;
				float v = (j + rand01<float>() - 0.5f) / img->height;
				rayf3 r = cam->GenRay(u, v, ccs);
				rgbf Lo = Shade(intersectors, intersectors.clostest.Visit(&bvh, r), -r.dir, true);
				img->At<rgbf>(i, j) += Lo / spp;
			}
		}
		float progress = (j + 1) / float(img->height);
		cout << progress << endl;
	}
}

rgbf PathTracer::Shade(const Intersectors& intersectors, const IntersectorClosest::Rst& intersection, const vecf3& wo, bool last_bounce_specular) {
	// TODO: HW9 - Trace
	// [ Tips ]
	// - EnvLight::Radiance(<direction>), <direction> is pointing to environment light
	// - rayf3: point, dir, tmin, **tmax**
	// - Intersectors::visibility.Visit(&bvh, <rayf3>)
	//   - tmin = EPSILON<float>
	//   - tmax = distance to light - EPSILON<float>
	// - Intersectors::cloest.Visit(&bvh, <rayf3>)
	//   - tmin = as default
	//   - tmax = as default
	//
	// struct IntersectorClosest::Rst {
	//	 bool IsIntersected() const noexcept { return sobj != nullptr; }
	//	 const SObj* sobj{ nullptr }; // intersection sobj
	//	 pointf3 pos; // intersection point's position
	//	 pointf2 uv; // texcoord
	//   normalf n; // normal, normalized
	//	 vecf3 tangent; // perpendicular to normal, normalized
	// };

	constexpr rgbf error_color = rgbf{ 1.f,0.f,1.f };
	constexpr rgbf todo_color = rgbf{ 0.f,1.f,0.f };
	constexpr rgbf zero_color = rgbf{ 0.f,0.f,0.f };

	if (!intersection.IsIntersected()) {
		if (last_bounce_specular && env_light != nullptr) {
			// TODO: environment light

			return todo_color;
		}
		else
			return zero_color;
	}
	
	if (!intersection.sobj->Get<Cmpt::Material>()) {
		auto light = intersection.sobj->Get<Cmpt::Light>();
		if(!light) return error_color;

		if (last_bounce_specular) { // avoid double-count
			auto area_light = dynamic_cast<const AreaLight*>(light->light.get());
			if (!area_light) return error_color;

			// TODO: area light

			return todo_color;
		}else
			return zero_color;
	}
	
	rgbf L_dir{ 0.f };
	rgbf L_indir{ 0.f };

	scene->Each([=, &L_dir](const Cmpt::Light* light, const Cmpt::L2W* l2w, const Cmpt::SObjPtr* ptr) {
		// TODO: L_dir += ...
		SampleLightResult sample_light_rst = SampleLight(light, l2w, ptr);
		if (sample_light_rst.is_infinity) {
			// TODO: L_dir of environment light
			// - only use SampleLightResult::L, n, pd
			// - SampleLightResult::x is useless
		}
		else {
			// TODO: L_dir of area light
		}
	});

	// TODO: Russian Roulette
	// - rand01<float>() : random in [0, 1)

	// TODO: recursion
	// - use PathTracer::SampleBRDF to get wi and pd (probability density)
	// - use PathTracer::BRDF to get BRDF value

	return todo_color; // TODO: combine L_dir and L_indir
}

PathTracer::SampleLightResult PathTracer::SampleLight(const Cmpt::Light* light, const Cmpt::L2W* l2w, const Cmpt::SObjPtr* ptr) {
	PathTracer::SampleLightResult rst;
	if (vtable_is<AreaLight>(light->light.get())) {
		auto area_light = static_cast<const AreaLight*>(light->light.get());
		auto geo = ptr->value->Get<Cmpt::Geometry>();
		if (!geo) return rst; // invalid
		if (!vtable_is<Square>(geo->primitive.get())) return rst; // not support

		auto Xi = uniform_in_square<float>(); // [0, 1] x [0, 1]
		pointf3 pos_light_space{ 2 * Xi[0] - 1, 0, 2 * Xi[1] - 1 };
		scalef3 s = l2w->WorldScale();
		float area = s[0] * s[1] * Square::area;

		rst.L = area_light->Radiance(Xi.cast_to<pointf2>());
		rst.pd = 1.f / area;
		rst.x = l2w->value * pos_light_space;
		rst.is_infinity = false;
		rst.n = l2w->UpInWorld().cast_to<normalf>();
	}
	else if (vtable_is<EnvLight>(light->light.get())) {
		auto env_light = static_cast<const EnvLight*>(light->light.get());

		auto [radiance, direction, pdf] = env_light->Sample();
		rst.L = radiance;
		rst.n = direction.cast_to<normalf>();
		rst.pd = pdf;
		rst.x = pointf3{ std::numeric_limits<float>::max() };
		rst.is_infinity = true;
	}
	return rst;
}

std::tuple<vecf3, float> PathTracer::SampleBRDF(IntersectorClosest::Rst intersection, const vecf3& wo) {
	auto mat = intersection.sobj->Get<Cmpt::Material>();
	if (!mat) return { vecf3{0.f}, 0.f };
	auto brdf = dynamic_cast<const stdBRDF*>(mat->material.get());
	if (!brdf) return { vecf3{0.f}, 0.f };
	matf3 surface_to_world = svecf::TBN(intersection.n.cast_to<vecf3>(), intersection.tangent);
	matf3 world_to_surface = surface_to_world.inverse();
	svecf s_wo = (world_to_surface * wo).cast_to<svecf>();

	rgbf albedo = brdf->Albedo(intersection.uv);
	float metalness = brdf->Metalness(intersection.uv);
	float roughness = brdf->Roughness(intersection.uv);

	auto [s_wi, pdf] = brdf->Sample(albedo, metalness, roughness, s_wo);
	vecf3 wi = surface_to_world * s_wi;

	return { wi,pdf };
}

rgbf PathTracer::BRDF(IntersectorClosest::Rst intersection, const vecf3& wi, const vecf3& wo) {
	auto mat = intersection.sobj->Get<Cmpt::Material>();
	if (!mat) return rgbf{ 1.f,0.f,1.f };
	auto brdf = dynamic_cast<const stdBRDF*>(mat->material.get());
	if (!brdf) return rgbf{ 1.f,0.f,1.f };
	matf3 surface_to_world = svecf::TBN(intersection.n.cast_to<vecf3>(), intersection.tangent);
	matf3 world_to_surface = surface_to_world.inverse();
	svecf s_wi = (world_to_surface * wi).cast_to<svecf>();
	svecf s_wo = (world_to_surface * wo).cast_to<svecf>();

	rgbf albedo = brdf->Albedo(intersection.uv);
	float metalness = brdf->Metalness(intersection.uv);
	float roughness = brdf->Roughness(intersection.uv);

	return brdf->BRDF(albedo, metalness, roughness, s_wi, s_wo);
}
