#include <Engine/Material/Beckmann.h>

#include <Engine/Material/SurfCoord.h>

using namespace Ubpa;
using namespace std;

float Beckmann::D(const normalf & wh) const {
	cout << "WARNING::Beckmann:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

float Beckmann::Lambda(const normalf & w) const {
	cout << "WARNING::Beckmann:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const normalf Beckmann::Sample_wh() const {
	cout << "WARNING::Beckmann:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}
