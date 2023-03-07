#include <UDP/Basic/vtable.h>
#include <iostream>

using namespace Ubpa;
using namespace std;

struct A { virtual ~A() = default; };
struct B : A {
protected:
	~B() = default;
};

int main() {
	A a;
	cout << vtable_of<B>::get() << endl;
	B* b = new B;
	cout << vtable(b) << endl;
}
