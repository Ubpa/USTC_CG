#include <UDP/Basic/Dirty.h>

#include <iostream>

using namespace Ubpa;
using namespace std;

class Vec {
public:
	Vec(float x, float y) : x{ x }, y{ y }{}
	void SetX(float value) {
		x = value;
		sum.SetDirty();
	}
	void SetY(float value) {
		y = value;
		sum.SetDirty();
	}
	float GetSum() const { return sum.Get(*this); }
private:
	float x;
	float y;
	mutable AutoDirty<float, const Vec&> sum = { [](float& s, const Vec& v) {
		s = v.x + v.y;
	} };
};

int main() {
	Vec v{ 1,2 };
	cout << v.GetSum() << endl;
	v.SetX(2.f);
	cout << v.GetSum() << endl;
}
