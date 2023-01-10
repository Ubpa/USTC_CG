#include <Engine/Light/CapsuleLight.h>

#include <Basic/Sampler/BasicSampler.h>
#include <Basic/Math.h>

using namespace Ubpa;

using namespace std;

const rgbf CapsuleLight::Sample_L(const pointf3 & p, normalf & wi, float & distToLight, float & PD) const {
	cout << "WARNING::CapsuleLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

float CapsuleLight::PDF(const pointf3 & p, const normalf & wi) const {
	cout << "WARNING::CapsuleLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}
