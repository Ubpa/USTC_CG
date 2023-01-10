#pragma once

#include <Engine/Viewer/RayTracer.h>

#include <UGM/transform.h>
#include <UGM/mat.h>

#include <vector>
#include <map>

namespace Ubpa {
	class Light;
	class BVHAccel;

	class ClosestIntersector;
	class VisibilityChecker;

	class BSDF;

	// 只能用于单线程
	class PathTracer : public RayTracer {
	public:
		PathTracer();

	public:
		static const Ptr<PathTracer> New() { return Ubpa::New<PathTracer>(); }

	protected:
		virtual ~PathTracer() = default;

	public:
		virtual const rgbf Trace(Ray& ray) { return Trace(ray, 0, rgbf(1.f)); }

		virtual void Init(Ptr<Scene> scene, Ptr<BVHAccel> bvhAccel) override;

	protected:
		// ray 处于世界坐标系
		const rgbf Trace(Ray& ray, int depth, rgbf pathThroughput);

	private:
		enum SampleLightMode {
			ALL,
			RandomOne,
		};

	public:
		int maxDepth;

	private:
		std::vector<Ptr<Light>> lights;
		std::map<Ptr<Light>, int> lightToIdx;
		std::vector<transformf> worldToLightVec;
		std::vector<transformf> lightToWorldVec;

		Ptr<ClosestIntersector> closestIntersector;
		Ptr<VisibilityChecker> visibilityChecker;
	};
}
