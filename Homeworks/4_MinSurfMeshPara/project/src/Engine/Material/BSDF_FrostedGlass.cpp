#include <Engine/Material/BSDF_FrostedGlass.h>

#include <Basic/Math.h>
#include <Basic/Image.h>

using namespace Ubpa;

using namespace std;

float BSDF_FrostedGlass::Fr(const normalf & v, const normalf & h, float ior) {
	cout << "WARNING::BSDF_FrostedGlass:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf BSDF_FrostedGlass::F(const normalf & wo, const normalf & wi, const pointf2 & texcoord) {
	cout << "WARNING::BSDF_FrostedGlass:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

// probability density function
float BSDF_FrostedGlass::PDF(const normalf & wo, const normalf & wi, const pointf2 & texcoord) {
	cout << "WARNING::BSDF_FrostedGlass:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf BSDF_FrostedGlass::Sample_f(const normalf & wo, const pointf2 & texcoord, normalf & wi, float & PD) {
	cout << "WARNING::BSDF_FrostedGlass:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf BSDF_FrostedGlass::GetColor(const pointf2 & texcoord) const {
	if (!colorTexture || !colorTexture->IsValid())
		return colorFactor;

	return colorFactor * (colorTexture->Sample(texcoord, Image::Mode::BILINEAR)).to_rgb();
}

float BSDF_FrostedGlass::GetRoughness(const pointf2 & texcoord) const {
	if (!roughnessTexture || !roughnessTexture->IsValid())
		return roughnessFactor;

	return roughnessTexture->Sample(texcoord, Image::Mode::BILINEAR)[0] * roughnessFactor;
}

float BSDF_FrostedGlass::GetAO(const pointf2 & texcoord) const {
	if (!aoTexture || !aoTexture->IsValid())
		return 1.0f;

	return aoTexture->Sample(texcoord, Image::Mode::BILINEAR)[0];
}

void BSDF_FrostedGlass::ChangeNormal(const pointf2 & texcoord, const normalf & tangent, normalf & normal) const {
	if (!normalTexture || !normalTexture->IsValid())
		return;

	const auto rgb = normalTexture->Sample(texcoord, Image::Mode::BILINEAR).to_rgb();
	normalf tangentSpaceNormal = 2.f * rgb.cast_to<normalf>() - normalf(1.f);

	normal = TangentSpaceNormalToWorld(tangent, normal, tangentSpaceNormal);
}
