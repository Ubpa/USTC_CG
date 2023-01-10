#include <Engine/Intersector/VisibilityChecker.h>

#include <Engine/Scene/SObj.h>
#include <Engine/Viewer/Ray.h>

#include <Engine/Viewer/BVHAccel.h>
#include <Engine/Primitive/Sphere.h>
#include <Engine/Primitive/Plane.h>
#include <Engine/Primitive/Triangle.h>
#include <Engine/Primitive/TriMesh.h>
#include <Engine/Primitive/Disk.h>
#include <Engine/Primitive/Capsule.h>

#include <stack>

using namespace Ubpa;

using namespace std;

VisibilityChecker::VisibilityChecker() {
	Regist<Sphere, Plane, Triangle, Disk, Capsule>();
}

void VisibilityChecker::Visit(Ptr<BVHAccel> bvhAccel) {
	cout << "WARNING::VisibilityChecker::ImplVisit:" << endl
		<< "\t" << "not implemented" << endl;
}

void VisibilityChecker::ImplVisit(Ptr<Sphere> sphere) {
	cout << "WARNING::VisibilityChecker::ImplVisit:" << endl
		<< "\t" << "not implemented" << endl;
}

void VisibilityChecker::ImplVisit(Ptr<Plane> plane) {
	cout << "WARNING::VisibilityChecker::ImplVisit:" << endl
		<< "\t" << "not implemented" << endl;
}

void VisibilityChecker::ImplVisit(Ptr<Triangle> triangle) {
	cout << "WARNING::VisibilityChecker::ImplVisit:" << endl
		<< "\t" << "not implemented" << endl;
}

void VisibilityChecker::ImplVisit(Ptr<Disk> disk) {
	cout << "WARNING::VisibilityChecker::ImplVisit:" << endl
		<< "\t" << "not implemented" << endl;
}

void VisibilityChecker::ImplVisit(Ptr<Capsule> capsule) {
	cout << "WARNING::VisibilityChecker::ImplVisit:" << endl
		<< "\t" << "not implemented" << endl;
}
