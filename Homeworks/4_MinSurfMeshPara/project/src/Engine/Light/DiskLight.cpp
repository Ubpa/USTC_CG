#include <Engine/Light/DiskLight.h>

#include <Basic/Sampler/BasicSampler.h>

using namespace Ubpa;
using namespace std;

const rgbf DiskLight::Sample_L(const pointf3 & p, normalf & wi, float & distToLight, float & PD) const {
	cout << "WARNING::DiskLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

float DiskLight::PDF(const pointf3 & p, const normalf & wi) const {
	cout << "WARNING::DiskLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}
