#pragma once

#include <UScene/core/Cmpt/Camera.h>
#include <UGM/UGM.h>

namespace Ubpa {
	class BVH;
	class Image;

	class PathTracer {
	public:
		PathTracer(BVH* bvh, Image* img,
			const Cmpt::Camera& cam, const Cmpt::Camera::CoordinateSystem& ccs)
			: bvh{ bvh }, img{ img }, cam{ cam }, ccs{ ccs }{}

		void Run();

	private:
		// rgbf Lo(const vecf3& wo, const pointf3& p);
		rgbf Trace(const rayf3& r);

		BVH* const bvh;
		Image* const img;
		const Cmpt::Camera cam;
		const Cmpt::Camera::CoordinateSystem ccs;
	};
}