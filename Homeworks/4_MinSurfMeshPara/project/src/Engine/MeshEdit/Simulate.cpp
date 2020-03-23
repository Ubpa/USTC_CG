#include <Engine/MeshEdit/Simulate.h>


#include <Eigen/Sparse>

using namespace Ubpa;

using namespace std;
using namespace Eigen;


void Simulate::Clear() {
	this->positions.clear();
	this->velocity.clear();
}

bool Simulate::Init() {
	//Clear();


	this->velocity .resize(positions.size());
	for (int i = 0; i < positions.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			this->velocity[i][j] = 0;
		}
	}




	return true;
}

bool Simulate::Run() {
	SimulateOnce();

	// half-edge structure -> triangle mesh

	return true;
}

void Ubpa::Simulate::SetLeftFix()
{
	//固定网格x坐标最小点
	fixed_id.clear();
	double x = 100000;
	for (int i = 0; i < positions.size(); i++)
	{
		if (positions[i][0] < x)
		{
			x = positions[i][0];
		}
	}

	for (int i = 0; i < positions.size(); i++)
	{
		if (abs(positions[i][0] - x) < 1e-5)
		{
			fixed_id.push_back(i);
		}
	}

	Init();
}

void Simulate::SimulateOnce() {
	// TODO
	//cout << "WARNING::Simulate::SimulateOnce:" << endl;
//		<< "\t" << "not implemented" << endl;

	
	for (int i = 0; i < positions.size(); i++)
	{

		this->velocity[i][0] += -h*1.f;
		this->positions[i][0] += h * this->velocity[i][0];
	}



}
