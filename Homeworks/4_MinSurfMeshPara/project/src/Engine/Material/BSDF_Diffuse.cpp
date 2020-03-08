#include <Engine/Material/BSDF_Diffuse.h>

#include <Basic/Math.h>
#include <Basic/Image.h>

using namespace Ubpa;
using namespace std;

const rgbf BSDF_Diffuse::F(const normalf & wo, const normalf & wi, const pointf2 & texcoord) {
	cout << "WARNING::BSDF_Diffuse:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf BSDF_Diffuse::Sample_f(const normalf & wo, const pointf2 & texcoord, normalf & wi, float & PD) {
	cout << "WARNING::BSDF_Diffuse:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

float BSDF_Diffuse::PDF(const normalf & wo, const normalf & wi, const pointf2 & texcoord) {
	cout << "WARNING::BSDF_Diffuse:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf BSDF_Diffuse::GetAlbedo(const pointf2 & texcoord) const {
	if (!albedoTexture || !albedoTexture->IsValid())
		return colorFactor;

	return colorFactor * albedoTexture->Sample(texcoord, Image::Mode::BILINEAR).to_rgb();
}
