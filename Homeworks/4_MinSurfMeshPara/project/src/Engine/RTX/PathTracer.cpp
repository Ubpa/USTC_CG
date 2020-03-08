#include <Engine/Viewer/PathTracer.h>

#include <Engine/Viewer/BVHAccel.h>

#include <Engine/Intersector/ClosestIntersector.h>
#include <Engine/Intersector/VisibilityChecker.h>

#include <Engine/Scene/Scene.h>
#include <Engine/Scene/SObj.h>

#include <Engine/Scene/CmptMaterial.h>
#include <Engine/Material/BSDF.h>

#include <Engine/Scene/CmptLight.h>
#include <Engine/Light/Light.h>

#include <Basic/Math.h>

using namespace Ubpa;

using namespace std;

PathTracer::PathTracer()
	:
	maxDepth(20),
	closestIntersector(ClosestIntersector::New()),
	visibilityChecker(VisibilityChecker::New())
{ }

void PathTracer::Init(Ptr<Scene> scene, Ptr<BVHAccel> bvhAccel) {
	RayTracer::Init(scene, bvhAccel);

	lights.clear();
	worldToLightVec.clear();
	lightToWorldVec.clear();
	lightToIdx.clear();

	auto cmptLights = scene->GetCmptLights();
	for (size_t i = 0; i < cmptLights.size(); i++) {
		auto cmptLight = cmptLights[i];
		auto light = cmptLight->light;

		lightToIdx[light] = static_cast<int>(i);

		lights.push_back(light);

		const auto lightToWorld = cmptLight->GetLightToWorldMatrixWithoutScale();
		const auto worldToLight = lightToWorld.inverse();

		worldToLightVec.push_back(worldToLight);
		lightToWorldVec.push_back(lightToWorld);
	}
}

const rgbf PathTracer::Trace(Ray & ray, int depth, rgbf pathThroughput) {
	//cout << "WARNING::PathTracer::Trace:" << endl
	//	<< "\t" << "not implemented" << endl;
	return { 1.f,0.f,1.f };
}
