#pragma once

#include <UScene/UScene.h>
namespace Ubpa {
	using IntersectorClosest = IntersectorClostest; // typo
}

#include <vector>

namespace Ubpa {

	class PathTracer {
	public:
		PathTracer(const Scene* scene, const SObj* cam_obj, Image* img);

		void Run();

	private:
		rgbf Shade(const pointf3& p, const IntersectorClosest::Rst& intersection, const vecf3& wo, bool last_bounce_specular = false);

		struct SampleLightResult {
			rgbf L{ 0.f }; // light radiance
			float pdf{ 0.f }; // probability
			normalf norm{ 0.f }; // normalize normal
			pointf3 x{ 0.f }; // position on light
			bool is_infinity{ false }; // infinity distance
		};
		static SampleLightResult SampleLight(const Cmpt::Light* light, const Cmpt::L2W* l2w, const Cmpt::SObjPtr* ptr);

		struct SampleBRDFResult {
			rgbf brdf{ 0.f };
			float pdf{ 0.f };
			vecf3 wi{ 0.f };
		};
		static SampleBRDFResult SampleBRDF(const vecf3& wo, IntersectorClosest::Rst rst);

		const Scene* const scene;
		Image* const img;
		const EnvLight* env_light{ nullptr };

		BVH bvh;
		IntersectorClosest clostest;
		IntersectorVisibility visibility;

		const Cmpt::Camera* const cam;
		const Cmpt::Camera::CoordinateSystem ccs;
	};
}