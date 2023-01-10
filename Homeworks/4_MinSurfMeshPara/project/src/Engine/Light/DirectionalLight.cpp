#include <Engine/Light/DirectionalLight.h>

using namespace Ubpa;

using namespace std;

const rgbf DirectionalLight::Sample_L(const pointf3 & p, normalf & wi, float & distToLight, float & PD) const {
	cout << "WARNING::DirectionalLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}
