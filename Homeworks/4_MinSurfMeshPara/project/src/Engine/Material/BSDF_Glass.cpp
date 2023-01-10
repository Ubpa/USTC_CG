#include <Engine/Material/BSDF_Glass.h>

#include <Engine/Material/SurfCoord.h>

using namespace Ubpa;

using namespace Ubpa::Math;

using namespace std;

const rgbf BSDF_Glass::Sample_f(const normalf & wo, const pointf2 & texcoord, normalf & wi, float & PD) {
	cout << "WARNING::BSDF_Glass:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}
