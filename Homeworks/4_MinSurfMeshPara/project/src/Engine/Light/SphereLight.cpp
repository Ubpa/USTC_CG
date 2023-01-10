#include <Engine/Light/SphereLight.h>

#include <Basic/Sampler/BasicSampler.h>

using namespace Ubpa;

using namespace std;

const rgbf SphereLight::Sample_L(const pointf3 & p, normalf & wi, float & distToLight, float & PD) const {
	cout << "WARNING::SphereLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

float SphereLight::PDF(const pointf3 & p, const normalf & wi) const {
	cout << "WARNING::SphereLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}
