#include <Engine/Intersector/ClosestIntersector.h>

#include <Engine/Scene/SObj.h>
#include <Engine/Viewer/Ray.h>

#include <Engine/Viewer/BVHAccel.h>

#include <Engine/Scene/CmptGeometry.h>
#include <Engine/Scene/CmptTransform.h>

#include <Engine/Primitive/Sphere.h>
#include <Engine/Primitive/Plane.h>
#include <Engine/Primitive/Triangle.h>
#include <Engine/Primitive/TriMesh.h>
#include <Engine/Primitive/Disk.h>
#include <Engine/Primitive/Capsule.h>

#include <Basic/Math.h>
#include <UGM/transform.h>

#include <stack>

using namespace Ubpa;

using namespace std;

ClosestIntersector::ClosestIntersector() {
	Regist<Sphere, Plane, Triangle, TriMesh, Disk, Capsule>();
}

void ClosestIntersector::Visit(Ptr<BVHAccel> bvhAccel) {
	cout << "WARNING::ClosestIntersector::ImplVisit:" << endl
		<< "\t" << "not implemented" << endl;
}

void ClosestIntersector::Visit(Ptr<SObj> sobj) {
	cout << "WARNING::ClosestIntersector::ImplVisit:" << endl
		<< "\t" << "not implemented" << endl;
}

void ClosestIntersector::ImplVisit(Ptr<Sphere> sphere) {
	cout << "WARNING::ClosestIntersector::ImplVisit:" << endl
		<< "\t" << "not implemented" << endl;
}

void ClosestIntersector::ImplVisit(Ptr<Plane> plane) {
	cout << "WARNING::ClosestIntersector::ImplVisit:" << endl
		<< "\t" << "not implemented" << endl;
}

void ClosestIntersector::ImplVisit(Ptr<Triangle> triangle) {
	cout << "WARNING::ClosestIntersector::ImplVisit:" << endl
		<< "\t" << "not implemented" << endl;
}

void ClosestIntersector::ImplVisit(Ptr<TriMesh> mesh) {
	cout << "WARNING::ClosestIntersector::ImplVisit:" << endl
		<< "\t" << "not implemented" << endl;
}

void ClosestIntersector::ImplVisit(Ptr<Disk> disk) {
	cout << "WARNING::ClosestIntersector::ImplVisit:" << endl
		<< "\t" << "not implemented" << endl;
}

void ClosestIntersector::ImplVisit(Ptr<Capsule> capsule) {
	cout << "WARNING::ClosestIntersector::ImplVisit:" << endl
		<< "\t" << "not implemented" << endl;
}
