#include <Engine/MeshEdit/Simulate.h>
#include <Eigen/Sparse>

using namespace Ubpa;
using namespace std;
using namespace Eigen;

void Simulate::Clear() {
	this->positions.clear();
	this->velocity.clear();
}

/////////////////////// Init//////////////////////////////////////////////
bool Simulate::Init() {
	Clear();
	positions = vector<pointf3>(positions_backup);	
	length_list.resize(edgelist.size() / 2);
	mass = 1;

	//	Init extra force
	force_ext.resize(positions.size());
	// Init velocity
	this->velocity.resize(positions.size());
	for (int i = 0; i < positions.size(); i++)
	{
		force_ext[i] = vecf3(0, -gravity, 0);
		for (int j = 0; j < 3; j++)
		{
			this->velocity[i][j] = 0;//初始速度为0
		}
	}
	// Init length
	for (size_t i = 0; i < edgelist.size() / 2; i++)
	{
		pointf3 p1 = positions[edgelist[i * 2]];
		pointf3 p2 = positions[edgelist[i * 2 + 1]];
		length_list[i] = pointf3::distance(p1, p2);
	}

	// Init pos as Eigen::vector
	Pos2Mat(positions,x_vec);
	// Init some matrixs
	InitMat();
	return true;
}

//===========================================IO========================================================

void Ubpa::Simulate::Restore()
{
	Init();
}

void Ubpa::Simulate::SetLeftFix()
{
	//固定网格x坐标最小点
	fixed_id.clear();
	fixed_coords.clear();
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
		if (abs(positions[i][0] - x) < 1e-2)
		{
			fixed_id.push_back(i);
			fixed_coords.push_back(positions[i]);
		}
	}

	Init();
	
}

void Ubpa::Simulate::SetRightFix()
{
	//固定网格x坐标最大点
	fixed_id.clear();
	fixed_coords.clear();
	double x = -100000;
	for (int i = 0; i < positions.size(); i++)
	{
		if (positions[i][0] > x)
		{
			x = positions[i][0];
		}
	}

	for (int i = 0; i < positions.size(); i++)
	{
		if (abs(positions[i][0] - x) < 1e-2)
		{
			fixed_id.push_back(i);
			fixed_coords.push_back(positions[i]);
		}
	}

	Init();
}

void Ubpa::Simulate::SetUpFix()
{
	//固定网格y坐标最大点
	fixed_id.clear();
	fixed_coords.clear();
	double y = -100000;
	for (int i = 0; i < positions.size(); i++)
	{
		if (positions[i][1] > y)
		{
			y = positions[i][1];
		}
	}

	for (int i = 0; i < positions.size(); i++)
	{
		if (abs(positions[i][1] - y) < 1e-2)
		{
			fixed_id.push_back(i);
			fixed_coords.push_back(positions[i]);
		}
	}
	Init();
}

void Ubpa::Simulate::SetDownFix()
{
	//固定网格y坐标最小点
	fixed_id.clear();
	fixed_coords.clear();
	double y = 100000;
	for (int i = 0; i < positions.size(); i++)
	{
		if (positions[i][1] < y)
		{
			y = positions[i][1];
		}
	}

	for (int i = 0; i < positions.size(); i++)
	{
		if (abs(positions[i][1] - y) < 1e-2)
		{
			fixed_id.push_back(i);
			fixed_coords.push_back(positions[i]);
		}
	}

	Init();
}

void Ubpa::Simulate::ClearFix()
{
	fixed_id.clear();
	fixed_coords.clear();
	Init();
}

//===========================================RUN========================================================
bool Simulate::Run() {
	//cout << "positions:" << endl << positions[0] << endl << positions[1] << endl << positions[2] << endl << endl;
	//cout << "velocity:" << endl << velocity[0] << endl << velocity[1] << endl << velocity[2] << endl << endl;
	SimulateOnce();
	//cout << "------------------------------------------------------------------------" << endl << endl;
	return true;
}

void Simulate::SimulateOnce() {
	//Init_b_optim();

	//size_t count_ = 0;
	if (method == kEuler) {
		
		EulerMethod();
	}
	else if(method == kPD)
	{
		
		ProjectiveDynamics();
	}

	//for (int i = 0; i < positions.size(); i++)
	//{

	//	this->velocity[i][0] += -h * 1.f;
	//	this->positions[i][0] += h * this->velocity[i][0];
	//}
}

//================================ Implicit Euler method========================================================
void Simulate::EulerMethod() {
	InitY();
	//std::vector<pointf3> old_positions(positions);
	//std::vector<pointf3> cur_positions = vector<pointf3>(y_list);
	//NewtonIteration(cur_positions);
	//positions.clear();
	//positions.assign(cur_positions.begin(), cur_positions.end());
	//for (size_t i = 0; i < fixed_coords.size(); i++)
	//{
	//	positions[fixed_id[i]] = fixed_coords[i];
	//}
	//UpdateVelocity(cur_positions, old_positions);
	
	
	float err = 1e4;
	int count = 0;
	while (err > 1e-3) {
		err = NewtonIteration();
	}
	cout << "count：" << count << ", error: " << err << endl;
	NewtonIteration();
	UpdateVelocity();
	Mat2Pos(x_vec, positions);
}

void Simulate::InitY()
{
	y_list.resize(positions.size());
	y_vec.resize(positions.size(), 3);
	for (size_t i = 0; i < y_list.size(); i++)
	{
		vecf3 v(velocity[i][0], velocity[i][1], velocity[i][2]);
		y_list[i] = positions[i] + h * v + h * h * force_ext[i] / mass;
		for (int j = 0; j < 3; j++)
			y_vec(i, j) = y_list[i][j];
		//velocity[i] = pointf3(positions[i][0], positions[i][1], positions[i][2]);//为了计算之后的速度
	}
	x_vec = y_vec;

	for (size_t i = 0; i < fixed_id.size(); i++)
	{
		for (int j = 0; j < 3; j++)
			x_vec(i, j) = fixed_coords[i][j];
	}
	
}

void Simulate::UpdateVelocity() {
	for (size_t i = 0; i < velocity.size(); i++)
	{
		vecf3 old_p(positions[i][0], positions[i][1], positions[i][2]);
		vecf3 new_p(x_vec(i, 0), x_vec(i, 1), x_vec(i, 2));
		vecf3 v = (new_p - old_p) / h;
		velocity[i] = pointf3(v[0], v[1], v[2]);
	}
	for (size_t i = 0; i < fixed_id.size(); i++)
	{
		velocity[fixed_id[i]] = pointf3(0, 0, 0);
	}
}

void Simulate::NewtonIteration(std::vector<pointf3>& cur_positions)
{	
	float err = 1e4;
	int count = 0;
	while (abs(err) > 1e-4) {
		UpdateStep(cur_positions);
		err = 0;
		for (int i = 0; i < cur_positions.size(); i++)
		{
			Eigen::Vector3f x_vec;
			x_vec << cur_positions[i][0], cur_positions[i][1], cur_positions[i][2];
			x_vec -= step_list[i];
			err += step_list[i].norm();
			//err += abs(step_list[i][0]) + abs(step_list[i][1]) + abs(step_list[i][2]);
			cur_positions[i] = pointf3(x_vec[0], x_vec[1], x_vec[2]);
		}
		err /= cur_positions.size();
		for (size_t i = 0; i < fixed_coords.size(); i++)
		{
			cur_positions[fixed_id[i]] = fixed_coords[i];
		}
		count++;
	}
	cout << "count：" << count << ", error: " << err << endl;	
}

float Ubpa::Simulate::NewtonIteration()
{
	float err = UpdateStep();
	x_vec -= step_vec;
	for (size_t i = 0; i < fixed_coords.size(); i++)
	{
		for (int j = 0; j < 3; j++)
			x_vec(fixed_id[i], j) = fixed_coords[i][j];
	}
	return err;
}

void Simulate::UpdateStep(const std::vector<pointf3>& cur_positions){
	vector<Eigen::Matrix3f> gradiant_force;
	vector<Eigen::Vector3f> g_list;
	vector<Eigen::Vector3f> force_in_list;
	Eigen::Matrix3f unit_mat = Eigen::Matrix3f::Identity();
	
	float len;
	float new_len;
	Eigen::Vector3f rij,pi,pj;
	Eigen::Vector3f r_vec;
	Eigen::Matrix3f r_mat;

	Eigen::Vector3f temp_f;
	Eigen::Matrix3f temp_grad;

	gradiant_force.resize(cur_positions.size());
	g_list.resize(cur_positions.size());
	force_in_list.resize(cur_positions.size());


	for (int i = 0; i < cur_positions.size(); i++){
		gradiant_force[i].setZero();
		g_list[i].setZero();
		force_in_list[i].setZero();
	}


	for (size_t i = 0; i < edgelist.size() / 2; i++){	
		len = length_list[i];
		
		size_t index_i = edgelist[i * 2];
		size_t index_j = edgelist[i * 2 + 1];

		for (size_t k = 0; k < 3; k++) {
			pi[k] = cur_positions[index_i][k];
			pj[k] = cur_positions[index_j][k];
		}
		
		rij = pi - pj;

		new_len = rij.norm();
	
		temp_f = stiff * (len - new_len) / new_len * rij;
		force_in_list[index_i] += temp_f;
		force_in_list[index_j] -= temp_f;
 
		r_vec << rij[0], rij[1], rij[2];
		r_mat = r_vec * r_vec.transpose();

		temp_grad = stiff * (len / new_len - 1) * unit_mat - stiff * len / pow(new_len, 3) * r_mat;
		gradiant_force[index_i] += temp_grad;
		gradiant_force[index_j] += temp_grad;
	}

	step_list.clear();
	for (int i = 0; i < cur_positions.size(); i++){
		pointf3 x = cur_positions[i];
		Eigen::Matrix3f gradiant_g;
		Eigen::Vector3f g;
		Eigen::Vector3f step;
		vecf3 g_vec;
		for (int k = 0; k < 3; k++) {
			g(k) = mass * (x[k] - y_list[i][k]) - h * h * force_in_list[i][k];
		}

		gradiant_g.setZero();
		gradiant_g = mass * unit_mat - h * h * gradiant_force[i];
		step = gradiant_g.llt().solve(g);//3*3矩阵运算代价很小
		step_list.push_back(step);
	}
}

float Ubpa::Simulate::UpdateStep()
{
	vector<Eigen::Matrix3f> gradiant_g_list, gradiant_force_list;
	gradiant_g_list.resize(positions.size());
	gradiant_force_list.resize(positions.size());
	for (int i = 0; i < positions.size(); i++) {
		gradiant_g_list[i].setZero();
		gradiant_force_list[i].setZero();
	}
	
	MatrixXf g_vec = MatrixXf::Zero(positions.size(), 3);
	MatrixXf force_in_vec = MatrixXf::Zero(positions.size(), 3);
	Eigen::Matrix3f unit3_mat = Eigen::Matrix3f::Identity();

	float len;
	float new_len;
	Eigen::Vector3f rij, pi, pj;
	Eigen::Vector3f r_vec;
	Eigen::Matrix3f r_mat;

	Eigen::Vector3f temp_f;
	Eigen::Matrix3f temp_grad;


	for (size_t i = 0; i < edgelist.size() / 2; i++) {
		len = length_list[i];

		size_t index_i = edgelist[i * 2];
		size_t index_j = edgelist[i * 2 + 1];

		pi = x_vec.row(index_i);
		pj = x_vec.row(index_j);
		rij = pi - pj;

		new_len = rij.norm();

		temp_f = stiff * (len - new_len) / new_len * rij;
		force_in_vec.row(index_i) += temp_f;
		force_in_vec.row(index_j) -= temp_f;

		r_vec << rij[0], rij[1], rij[2];
		r_mat = r_vec * r_vec.transpose();

		temp_grad = stiff * (len / new_len - 1) * unit3_mat - stiff * len / pow(new_len, 3) * r_mat;
		gradiant_force_list[index_i] += temp_grad;
		gradiant_force_list[index_j] += temp_grad;
	}

	step_vec.resize(3, positions.size());
	float err = 0;
	g_vec = (mass * (x_vec - y_vec) - h * h * force_in_vec).transpose();
	for (int i = 0; i < positions.size(); i++) {
		Vector3f g = g_vec.col(i);
		Eigen::Vector3f step;
		Eigen::Matrix3f gradiant_g;
		gradiant_g = mass * unit3_mat - h * h * gradiant_force_list[i] ;
		step = gradiant_g.llt().solve(g);//3*3矩阵运算代价很小
		step_vec.col(i) = step;
		err += step.lpNorm<1>();
	}
	step_vec.transposeInPlace();
	
	/*Eigen::VectorXf b = g_vec.reshaped<RowMajor>();

	std::vector<Triplet<float>> triplet_list(positions.size() * 9);
	for (int i = 0; i < positions.size(); i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				triplet_list.push_back(Triplet<float>(i * 3 + j, i * 3 + k, gradiant_g_list[i](j, k)));
			}
		}
	}
	SparseMatrix<float> A(positions.size() * 3, positions.size() * 3);
	A.setFromTriplets(triplet_list.begin(), triplet_list.end());
	SimplicialLLT<SparseMatrix<float>> solver;
	solver.compute(A);
	Eigen::VectorXf result = solver.solve(b);
	step_vec = result.reshaped(3, positions.size()).transpose();*/
	return err;
}

void Simulate::UpdateVelocity(const std::vector<pointf3>& new_positions, const std::vector<pointf3>& old_positions)
{
	for (size_t i = 0; i < velocity.size(); i++)
	{
		vecf3 new_p(new_positions[i][0], new_positions[i][1], new_positions[i][2]);
		vecf3 old_p(old_positions[i][0], old_positions[i][1], old_positions[i][2]);
		//vecf3 new_p(positions[i][0], positions[i][1], positions[i][2]);
		//vecf3 old_p(velocity[i][0], velocity[i][1], velocity[i][2]);
		vecf3 v = (new_p - old_p) / h;
		velocity[i] = pointf3(v[0], v[1], v[2]);
	}
	
	for (size_t i = 0; i < fixed_id.size(); i++)
	{
		velocity[fixed_id[i]] = pointf3(0, 0, 0);
	}
}
//==============================Projective Dynamics=====================================================

void Simulate::ProjectiveDynamics() {
	InitY();
	float error = 10000;
	while (abs(error) > 1e-6)
	{
		error = IterationOnceOptimize();
		//std::cout << error << std::endl;
	}
	UpdateVelocity();
	for (size_t i = 0; i < fixed_coords.size(); i++) {
		for (int j = 0; j < 3; j++)
			x_vec(fixed_id[i], j) = fixed_coords[i][j];
	}
	Mat2Pos(x_vec,positions);
}

void Simulate::InitMat()
{
	vector<SparseVector<float>> A_list;
	vector<SparseVector<float>> S_list;
	SparseMatrix<float> identity_spmat;
	
	size_t m = positions.size();
	size_t s = edgelist.size() / 2;

	identity_spmat.resize(m, m);
	identity_spmat.setIdentity();	
	S_list.resize(s);
	A_list.resize(s);

	L_spmat.resize(m, m);
	J_spmat.resize(m, s);
	L_spmat.setZero();
	J_spmat.setZero();
	//A_list, S_list
	for (size_t i = 0; i < s; i++)
	{
		A_list[i].resize(m);
		S_list[i].resize(s);
		A_list[i].insert(edgelist[i * 2]) = 1;
		A_list[i].insert(edgelist[i * 2 + 1]) = -1;
		S_list[i].insert(i) = 1;
	}
	//L_mat, J_mat
	for (size_t i = 0; i < s; i++)
	{
		L_spmat += A_list[i] * A_list[i].transpose() * stiff;
		J_spmat += A_list[i] * S_list[i].transpose() * stiff;
	}

	//K_mat
	size_t num_fix = fixed_id.size();
	size_t n = m - num_fix;
	
	K_spmat.resize(n, m);
	K_spmat.setZero();
	vector<Triplet<float>> triplets;
	triplets.reserve(n);
	for (size_t i = 0,j = 0 ; i < m; i++)
	{
		if (j<fixed_id.size() && i == fixed_id[j])
		{
			j++;
			continue;
		}
		else {
			triplets.push_back(Triplet<float>(i-j, i, 1));
		}
		
	}
	K_spmat.setFromTriplets(triplets.begin(), triplets.end());
	
	//A_mat, solver
	A_optim_spmat.resize(m, m);
	A_optim_spmat = mass * identity_spmat + h * h * L_spmat;
	
	SparseMatrix<float> A = K_spmat * A_optim_spmat * K_spmat.transpose();
	solver.compute(A.pruned());
}

float Simulate::IterationOnceOptimize()
{
	UpdateD();
	float error = UpdateX();
	return error;
}


void Simulate::UpdateD()
{
	d_vec.resize(edgelist.size() / 2, 3);
	d_vec.setZero();

	for (size_t i = 0; i < edgelist.size() / 2; i++)
	{
		Eigen::Vector3f pi = x_vec.row(edgelist[i * 2]);
		Eigen::Vector3f pj = x_vec.row(edgelist[i * 2 + 1]);
		Eigen::Vector3f d = pi - pj;
		d_vec.row(i) = length_list[i] * d / (d.norm());
	}
}

float Simulate::UpdateX() {

	Eigen::MatrixXf X = x_vec;
	Eigen::MatrixXf bb = X - K_spmat.transpose() * K_spmat * X;
	Eigen::MatrixXf B = K_spmat * (h * h * J_spmat * d_vec + mass * y_vec - A_optim_spmat * bb);
	X = solver.solve(B);
	X = K_spmat.transpose() * X + bb;
	float err = 0;
	for (size_t i = 0; i < positions.size(); i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			err += abs(X(i, j) - x_vec(i, j));
		}
	}
	err /= positions.size();
	x_vec = X;
	return err;
}


void Simulate::Pos2Mat(const std::vector<pointf3>& pos, Eigen::MatrixXf& mat){
	mat = Eigen::MatrixXf(pos.size(), 3);
	for (size_t i = 0; i < pos.size(); i++) {
		mat(i, 0) = pos[i][0];
		mat(i, 1) = pos[i][1];
		mat(i, 2) = pos[i][2];
	}
}

void Simulate::Mat2Pos(const Eigen::MatrixXf& mat, std::vector<pointf3>& pos){
	pos.resize(mat.rows());
	for (size_t i = 0; i < mat.rows(); i++)
	{
		pos[i] = pointf3(mat(i, 0), mat(i, 1), mat(i, 2));
	}
}



