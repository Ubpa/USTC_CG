#include <UDP/Visitor/cVisitor.h>
#include <UDP/Visitor/ncVisitor.h>
#include <UDP/Visitor/cicVisitor.h>
#include <UDP/Visitor/cincVisitor.h>
#include <UDP/Visitor/ncicVisitor.h>
#include <UDP/Visitor/ncincVisitor.h>

#include <iostream>
#include <memory>

using namespace Ubpa;
using namespace std;

struct IShape { virtual ~IShape() = default; };

struct Triangle : IShape {};
struct Sphere : IShape {};

int main() {
	{ // non-const visitor
		Visitor<void(void*)> visitor;
		auto visit_tri = [](Triangle*) { cout << "Lambda(Triangle*)" << endl; };
		auto visit_sphere = [](Sphere*) { cout << "Lambda(Sphere*)" << endl; };
		visitor.Register(visit_tri, visit_sphere);
		Triangle tri;
		Sphere sphere;
		void* s0 = &tri;
		void* s1 = &sphere;
		visitor.Visit(s0);
		visitor.Visit(s1);
	}
	{ // const
		Visitor<void(const void*)> visitor;
		auto visit_tri = [](const Triangle*) { cout << "Lambda(const Triangle*)" << endl; };
		auto visit_sphere = [](const Sphere*) { cout << "Lambda(const Sphere*)" << endl; };
		visitor.Register(visit_tri, visit_sphere);
		Triangle tri;
		Sphere sphere;
		void* s0 = &tri;
		void* s1 = &sphere;
		visitor.Visit(s0);
		visitor.Visit(s1);
	}
	{ // non-const impl non-const visitor
		class V : public Visitor<void(V::*)(void*)> {
		public:
			V() {
				Register<Triangle>();
			}
		protected:
			void ImplVisit(Triangle*) {
				cout << "V::ImplVisit(Triangle*)" << endl;
			}
		};
		

		V visitor;
		auto visit_sphere = [](Sphere*) { cout << "Lambda(Sphere*)" << endl; };
		visitor.Register(visit_sphere);
		Triangle tri;
		Sphere sphere;
		void* s0 = &tri;
		void* s1 = &sphere;
		visitor.Visit(s0);
		visitor.Visit(s1);
	}
	{ // non-const impl const visitor
		class V : public Visitor<void(V::*)(const void*)> {
		public:
			V() {
				Register<Triangle>();
			}
		protected:
			void ImplVisit(const Triangle*) {
				cout << "V::ImplVisit(const Triangle*)" << endl;
			}
		};

		V visitor;
		auto visit_sphere = [](const Sphere*) { cout << "Lambda(const Sphere*)" << endl; };
		visitor.Register(visit_sphere);
		Triangle tri;
		Sphere sphere;
		void* s0 = &tri;
		void* s1 = &sphere;
		visitor.Visit(s0);
		visitor.Visit(s1);
	}
	{ // const impl non-const visitor
		class V : public Visitor<void(V::*)(void*)const> {
		public:
			V() {
				Register<Triangle>();
			}
		protected:
			void ImplVisit(Triangle*) const {
				cout << "(const V)::ImplVisit(Triangle*)" << endl;
			}
		};


		V visitor;
		auto visit_sphere = [](Sphere*) { cout << "Lambda(Sphere*)" << endl; };
		visitor.Register(visit_sphere);
		Triangle tri;
		Sphere sphere;
		void* s0 = &tri;
		void* s1 = &sphere;
		visitor.Visit(s0);
		visitor.Visit(s1);
	}
	{ // const impl const visitor
		class V : public Visitor<void(V::*)(const void*)const> {
		public:
			V() {
				Register<Triangle>();
			}
		protected:
			void ImplVisit(const Triangle*) const {
				cout << "(const V)::ImplVisit(const Triangle*)" << endl;
			}
		};

		V visitor;
		auto visit_sphere = [](const Sphere*) { cout << "Lambda(const Sphere*)" << endl; };
		visitor.Register(visit_sphere);
		Triangle tri;
		Sphere sphere;
		void* s0 = &tri;
		void* s1 = &sphere;
		visitor.Visit(s0);
		visitor.Visit(s1);
	}
}
