#include <Engine/Material/BSDF_CookTorrance.h>

#include <Basic/Math.h>

using namespace Ubpa;

using namespace std;

float BSDF_CookTorrance::NDF(const normalf & h) {
	cout << "WARNING::BSDF_CookTorrance:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

float BSDF_CookTorrance::Fr(const normalf & wi, const normalf & h) {
	cout << "WARNING::BSDF_CookTorrance:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

float BSDF_CookTorrance::G(const normalf & wo, const normalf & wi, const normalf & h){
	cout << "WARNING::BSDF_CookTorrance:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf BSDF_CookTorrance::F(const normalf & wo, const normalf & wi, const pointf2 & texcoord) {
	cout << "WARNING::BSDF_CookTorrance:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

float BSDF_CookTorrance::PDF(const normalf & wo, const normalf & wi, const pointf2 & texcoord) {
	cout << "WARNING::BSDF_CookTorrance:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf BSDF_CookTorrance::Sample_f(const normalf & wo, const pointf2 & texcoord, normalf & wi, float & pd) {
	cout << "WARNING::BSDF_CookTorrance:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}
