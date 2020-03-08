#include <Engine/Material/BSDF_Frostbite.h>

#include <Basic/Image.h>
#include <Basic/Math.h>
#include <Basic/Sampler/CosHsSampler3D.h>

using namespace Ubpa;

using namespace std;

const rgbf BSDF_Frostbite::Fr(const normalf & w, const normalf & h, const rgbf & albedo, float metallic) {
	cout << "WARNING::BSDF_Frostbite:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const float BSDF_Frostbite::Fr_DisneyDiffuse(const normalf & wo, const normalf & wi, float linearRoughness) {
	cout << "WARNING::BSDF_Frostbite:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf BSDF_Frostbite::F(const normalf & wo, const normalf & wi, const pointf2 & texcoord) {
	cout << "WARNING::BSDF_Frostbite:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

// probability density function
float BSDF_Frostbite::PDF(const normalf & wo, const normalf & wi, const pointf2 & texcoord) {
	cout << "WARNING::BSDF_Frostbite:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

// PD is probability density
// return albedo
const rgbf BSDF_Frostbite::Sample_f(const normalf & wo, const pointf2 & texcoord, normalf & wi, float & PD) {
	cout << "WARNING::BSDF_Frostbite:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

void BSDF_Frostbite::ChangeNormal(const pointf2 & texcoord, const normalf & tangent, normalf & normal) const {
	if (!normalTexture || !normalTexture->IsValid())
		return;

	const auto rgb = normalTexture->Sample(texcoord, Image::Mode::BILINEAR).to_rgb();
	normalf tangentSpaceNormal = 2.f * rgb.cast_to<normalf>() - normalf(1.f);

	normal = TangentSpaceNormalToWorld(tangent, normal, tangentSpaceNormal);
}

const rgbf BSDF_Frostbite::GetAlbedo(const pointf2 & texcoord) const {
	if (!albedoTexture || !albedoTexture->IsValid())
		return colorFactor;

	return colorFactor * albedoTexture->Sample(texcoord, Image::Mode::BILINEAR).to_rgb();
}

float BSDF_Frostbite::GetMetallic(const pointf2 & texcoord) const {
	if (!metallicTexture || !metallicTexture->IsValid())
		return metallicFactor;

	return metallicFactor * metallicTexture->Sample(texcoord, Image::Mode::BILINEAR)[0];
}

float BSDF_Frostbite::GetRoughness(const pointf2 & texcoord) const {
	if (!roughnessTexture || !roughnessTexture->IsValid())
		return roughnessFactor;

	return roughnessFactor * roughnessTexture->Sample(texcoord, Image::Mode::BILINEAR)[0];
}

float BSDF_Frostbite::GetAO(const pointf2 & texcoord) const {
	if (!aoTexture || !aoTexture->IsValid())
		return 1.0f;

	return aoTexture->Sample(texcoord, Image::Mode::BILINEAR)[0];
}