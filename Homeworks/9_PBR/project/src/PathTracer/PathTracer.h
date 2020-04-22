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
		// rgbf Lo(const vecf3& wo, const pointf3& p);
		rgbf Trace(const rayf3& r);

		struct SampleLightResult {
			rgbf L{ 0.f };
			float p{ 0.f };
			pointf3 x{ 0.f };
			bool is_envlight{ false };
		};
		static SampleLightResult SampleLight(const Cmpt::Light* light, const Cmpt::L2W* l2w, const Cmpt::SObjPtr* ptr);

		struct SampleBRDFResult {
			rgbf brdf{ 0.f };
			float p{ 0.f };
			vecf3 wi{ 0.f };
		};
		static SampleBRDFResult SampleBRDF(const vecf3& wo, IntersectorClosest::Rst rst);

		const Scene* const scene;
		Image* const img;

		BVH bvh;
		IntersectorClosest clostest;
		IntersectorVisibility visibility;

		const Cmpt::Camera* const cam;
		const Cmpt::Camera::CoordinateSystem ccs;
	};
}