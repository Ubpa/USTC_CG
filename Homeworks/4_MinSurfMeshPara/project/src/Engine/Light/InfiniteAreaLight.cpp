#include <Engine/Light/InfiniteAreaLight.h>

#include <Basic/Image.h>
#include <Basic/Sampler/BasicSampler.h>
#include <Engine/Primitive/Sphere.h>
#include <Basic/Math.h>

using namespace Ubpa;

using namespace std;

void InfiniteAreaLight::SetImg(Ptr<Image> img) {
	if(img && img->IsValid())
		this->img = img;
	cout << "WARNING::InfiniteAreaLight:" << endl
		<< "\t" << "not implemented" << endl;
}

const rgbf InfiniteAreaLight::Sample_L(const pointf3 & p, normalf & wi, float & distToLight, float & PD) const {
	cout << "WARNING::InfiniteAreaLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

float InfiniteAreaLight::PDF(const pointf3 & p, const normalf & wi) const {
	cout << "WARNING::InfiniteAreaLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf InfiniteAreaLight::Le(const Ray & ray) const {
	cout << "WARNING::InfiniteAreaLight:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}

const rgbf InfiniteAreaLight::GetColor(const pointf2 & texcoord) const {
	if (!img || !img->IsValid())
		return intensity * colorFactor;

	return intensity * colorFactor * (img->Sample(texcoord, Image::Mode::BILINEAR)).to_rgb();
}
