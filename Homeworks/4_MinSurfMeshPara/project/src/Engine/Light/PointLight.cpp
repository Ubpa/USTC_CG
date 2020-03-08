#include <Engine/Light/PointLight.h>

using namespace Ubpa;

using namespace std;

float PointLight::Fwin(float d, float radius) {
	cout << "WARNING::PointLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf PointLight::Sample_L(const pointf3 & p, normalf & wi, float & distToLight, float & PD) const {
	cout << "WARNING::PointLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}
