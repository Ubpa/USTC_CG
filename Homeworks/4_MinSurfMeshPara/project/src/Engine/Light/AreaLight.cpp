#include <Engine/Light/AreaLight.h>

#include <UGM/point.h>

using namespace Ubpa;
using namespace std;

const rgbf AreaLight::Sample_L(const pointf3 & p, normalf & wi, float & distToLight, float & PD) const {
	cout << "WARNING::AreaLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

float AreaLight::PDF(const pointf3 & p, const normalf & wi) const {
	cout << "WARNING::AreaLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

bool AreaLight::Hit(const pointf3 & p, const vecf3 & dirToLight, pointf3 & hitPos) const {
	cout << "WARNING::AreaLight:" << endl
		<< "\t" << "not implemented" << endl;
	return false;
}
