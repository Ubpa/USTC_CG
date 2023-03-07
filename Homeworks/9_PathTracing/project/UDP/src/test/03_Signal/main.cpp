#include <UDP/Signal/Signal.h>

#include <iostream>

using namespace Ubpa;
using namespace std;

int main() {
	Signal<int, int> mouseMoved;
	auto connection = mouseMoved.Connect([](int x, int y) {
		cout << x << ", " << y << endl;
		});
	mouseMoved.Emit(3, 4);
}
