#include <Engine/Material/BSDF_MetalWorkflow.h>

#include <Basic/Sampler/CosHsSampler3D.h>

#include <Basic/Image.h>
#include <Basic/Math.h>

using namespace Ubpa;
using namespace std;

const rgbf BSDF_MetalWorkflow::F(const normalf & wo, const normalf & wi, const pointf2 & texcoord) {
	cout << "WARNING::BSDF_MetalWorkflow:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

float BSDF_MetalWorkflow::PDF(const normalf & wo, const normalf & wi, const pointf2 & texcoord) {
	cout << "WARNING::BSDF_MetalWorkflow:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf BSDF_MetalWorkflow::Sample_f(const normalf & wo, const pointf2 & texcoord, normalf & wi, float & pd) {
	cout << "WARNING::BSDF_MetalWorkflow:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf BSDF_MetalWorkflow::Fr(const normalf & w, const normalf & h, const rgbf & albedo, float metallic) {
	cout << "WARNING::BSDF_MetalWorkflow:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf BSDF_MetalWorkflow::GetAlbedo(const pointf2 & texcoord) const {
	if (!albedoTexture || !albedoTexture->IsValid())
		return colorFactor;

	return colorFactor * albedoTexture->Sample(texcoord, Image::Mode::BILINEAR).to_rgb();
}

float BSDF_MetalWorkflow::GetMetallic(const pointf2 & texcoord) const {
	if (!metallicTexture || !metallicTexture->IsValid())
		return metallicFactor;

	return metallicFactor * metallicTexture->Sample(texcoord, Image::Mode::BILINEAR)[0];
}

float BSDF_MetalWorkflow::GetRoughness(const pointf2 & texcoord) const {
	if (!roughnessTexture || !roughnessTexture->IsValid())
		return roughnessFactor;

	return roughnessFactor * roughnessTexture->Sample(texcoord, Image::Mode::BILINEAR)[0];
}

float BSDF_MetalWorkflow::GetAO(const pointf2 & texcoord) const {
	if (!aoTexture || !aoTexture->IsValid())
		return 1.0f;

	return aoTexture->Sample(texcoord, Image::Mode::BILINEAR)[0];
}

void BSDF_MetalWorkflow::ChangeNormal(const pointf2 & texcoord, const normalf & tangent, normalf & normal) const {
	if (!normalTexture || !normalTexture->IsValid())
		return;

	const auto rgb = normalTexture->Sample(texcoord, Image::Mode::BILINEAR).to_rgb();
	normalf tangentSpaceNormal = 2.f * rgb.cast_to<normalf>() - normalf(1.f);

	normal = TangentSpaceNormalToWorld(tangent, normal, tangentSpaceNormal);
}
