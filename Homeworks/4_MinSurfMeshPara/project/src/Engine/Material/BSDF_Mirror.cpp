#include <Engine/Material/BSDF_Mirror.h>

#include <Engine/Material/SurfCoord.h>

using namespace Ubpa;

using namespace Ubpa::Math;

using namespace std;

const rgbf BSDF_Mirror::Sample_f(const normalf & wo, const pointf2 & texcoord, normalf & wi, float & PD) {
	cout << "WARNING::BSDF_Mirror:" << endl
		<< "\t" << "not implemented" << endl;
	return 0.f;
}
