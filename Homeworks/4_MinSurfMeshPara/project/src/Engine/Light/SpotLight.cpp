#include <Engine/Light/SpotLight.h>

using namespace Ubpa;

using namespace std;

float SpotLight::Fwin(float d, float radius) {
	cout << "WARNING::SpotLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf SpotLight::Sample_L(const pointf3 & p, normalf & wi, float & distToLight, float & PD) const {
	cout << "WARNING::SpotLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

float SpotLight::Falloff(const normalf & wi) const {
	cout << "WARNING::SpotLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}
