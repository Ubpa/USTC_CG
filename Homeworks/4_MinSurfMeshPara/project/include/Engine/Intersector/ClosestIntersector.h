#pragma once

#include "Intersector.h"

#include <Basic/Ptr.h>
#include <Basic/HeapObj.h>
#include <Engine/Primitive/Shape.h>

#include <UGM/point.h>
#include <UGM/point.h>
#include <UGM/normal.h>
#include <UGM/bbox.h>

#include <Engine/Viewer/Ray.h>

namespace Ubpa {
	class Element;

	// Ѱ������Ľ���
	class ClosestIntersector : public HeapObj, public SharedPtrVisitor<ClosestIntersector, Shape>, public Intersector {
	public:
		ClosestIntersector();

		using SharedPtrVisitor<ClosestIntersector, Shape>::Visit;
		void Visit(Ptr<BVHAccel> bvhAccel);
		void Visit(Ptr<SObj> sobj);

	public:
		static const Ptr<ClosestIntersector> New() { return Ubpa::New<ClosestIntersector>(); }

	protected:
		// ���� rst������ཻ������޸� ray.tMax
		void ImplVisit(Ptr<Sphere> sphere);
		void ImplVisit(Ptr<Plane> plane);
		void ImplVisit(Ptr<Triangle> triangle);
		void ImplVisit(Ptr<TriMesh> mesh);
		void ImplVisit(Ptr<Disk> disk);
		void ImplVisit(Ptr<Capsule> capsule);
	};
}
