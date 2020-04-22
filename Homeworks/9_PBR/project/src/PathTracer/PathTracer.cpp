#include "PathTracer.h"

#include <UBL/Image.h>

#include <iostream>

using namespace Ubpa;
using namespace std;

void PathTracer::Run() {
	img->SetAll(0.f);

	const size_t spp = 2; // sample per pixel

	for (size_t j = 0; j < img->height; j++) {
		for (size_t i = 0; i < img->width; i++) {
			for (size_t k = 0; k < spp; k++) {
				float u = i / float(img->width);
				float v = j / float(img->height);
				rayf3 r = cam.GenRay(u, v, ccs);
				rgbf Lo = Trace(r);
				img->At<rgbf>(i, j) += Lo / spp;
			}
		}
		float progress = (j + 1) / float(img->height);
		cout << progress << endl;
	}
}

rgbf PathTracer::Trace(const rayf3& r) {
	return rgbf{ 1.f,0.f,1.f };


}
