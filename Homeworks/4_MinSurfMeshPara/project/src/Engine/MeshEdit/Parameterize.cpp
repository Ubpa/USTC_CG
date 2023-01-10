#include <Engine/MeshEdit/Parameterize.h>

#include <Engine/MeshEdit/MinSurf.h>

#include <Engine/Primitive/TriMesh.h>

using namespace Ubpa;

using namespace std;

Parameterize::Parameterize(Ptr<TriMesh> triMesh): heMesh(make_shared<HEMesh<V>>()) {
	display_status = koff;
	boundary_type = kSquare;
	method = kUniform;
	Init(triMesh);
}

void Parameterize::Clear() {
	heMesh->Clear();
	triMesh = nullptr;
}

bool Parameterize::Init(Ptr<TriMesh> triMesh) {
	Clear();
	/*
	仿照 MinSurf::Init
	*/
	if (triMesh == nullptr)
		return true;
	if (triMesh->GetType() == TriMesh::INVALID) {
		printf("ERROR::Parameterize::Init:\n"
			"\t""trimesh is invalid\n");
		return false;
	}
	// init half-edge structure as triangle grid
	size_t nV = triMesh->GetPositions().size();
	vector<vector<size_t>> triangles;
	triangles.reserve(triMesh->GetTriangles().size());
	for (auto triangle : triMesh->GetTriangles())
		triangles.push_back({ triangle->idx[0], triangle->idx[1], triangle->idx[2] });
	heMesh->Reserve(nV);
	heMesh->Init(triangles);

	if (!heMesh->IsTriMesh() || !heMesh->HaveBoundary()) {
		printf("ERROR::Parameterize::Init:\n"
			"\t""trimesh is not a triangle mesh or hasn't a boundaries\n");
		heMesh->Clear();
		return false;
	}
	// 将TriMesh 转化成 半边结构处理
	// triangle mesh's positions ->  half-edge structure's positions
	for (int i = 0; i < nV; i++) {
		auto v = heMesh->Vertices().at(i);
		v->pos = triMesh->GetPositions()[i].cast_to<vecf3>();
	}

	this->triMesh = triMesh;
	return true;
}

/// Core Code
/// ///////////////////////////////////////////////////////////////////////
bool Parameterize::Run() {
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::MinSurf::Run\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}

	Paramaterization();
	//将半边结构输出成TriMesh 三角网格数据结构,改变外部结构
	// half-edge structure -> triangle mesh
	size_t nV = heMesh->NumVertices();
	size_t nF = heMesh->NumPolygons();
	vector<pointf3> positions;
	vector<unsigned> indice;
	positions.reserve(nV);
	indice.reserve(3 * nF);
	for (auto v : heMesh->Vertices())
		positions.push_back(v->pos.cast_to<pointf3>());
	for (auto f : heMesh->Polygons()) { // f is triangle
		for (auto v : f->BoundaryVertice()) // vertices of the triangle
			indice.push_back(static_cast<unsigned>(heMesh->Index(v)));
	}

	triMesh->Init(indice, positions);
	triMesh->Update(texcoords);//更新网格，进行贴图，建立图像与平面的映射关系
	return true;
}

void Parameterize::Paramaterization()
{
	InitBoundary();
	InitMatrix();
	Predecomposition();
	Solve();
}

void Parameterize::InitBoundary()
{
	size_t nB = heMesh->Boundaries()[0].size();//对封闭的无亏格的图形，boundary只有一个
	for (int i = 0; i < nB; i++)
	{
		boundary_index.push_back(heMesh->Index(heMesh->Boundaries()[0][i]->Origin()));//将boundary的顶点位置加入
	}
	if (boundary_type == kSquare)
		boundary_ptr = make_shared<SquareBoundary>();
	else if (boundary_type == kCircle)
		boundary_ptr = make_shared<CircleBoundary>();
	boundary_ptr->Setting(boundary_index, boundary_list);
}

void Parameterize::InitMatrix()
{
	if (method == kCotangent)
		InitCotangent();
	else if (method == kUniform)
		InitUniform();
}

//平均重心坐标法
void Parameterize::InitUniform()
{

	size_t nV = heMesh->NumVertices();

	//	generate Laplace Matrix
	// 
	vector<Eigen::Triplet<double>> triplet_list;

	for (size_t i = 0; i < nV; i++)
	{
		V* v1 = heMesh->Vertices()[i];
		triplet_list.push_back(Eigen::Triplet<double>(i, i, 1));
		if (!v1->IsBoundary())//不要在边界的点
		{
			double connect_num = v1->AdjVertices().size();
			for (size_t j = 0; j < connect_num; j++)
			{
				triplet_list.push_back(Eigen::Triplet<double>(i, heMesh->Index(v1->AdjVertices()[j]), -1 / connect_num));//重心关系
			}
		}
	}

	laplace_mat.resize(nV, nV);
	laplace_mat.setZero();

	laplace_mat.setFromTriplets(triplet_list.begin(), triplet_list.end());

}

/*Laplace-Beltrami 重心法*/
void Parameterize::InitCotangent()
{
	size_t nV = heMesh->NumVertices();
	vector<Eigen::Triplet<double>> triplet_list;

	for (size_t i = 0; i < nV; i++)
	{
		V* v = heMesh->Vertices()[i];
		triplet_list.push_back(Eigen::Triplet<double>(i, i, 1));

		if (!v->IsBoundary())
		{
			int pre = 0, next = 0, cur = 0;
			int vsize = v->AdjVertices().size();
			vector<size_t> neighbor_index;
			vector<double> weight_list;
			double weight_sum = 0;
			for (cur = 0; cur < vsize; cur++)
			{
				pre = (cur - 1 + vsize) % vsize;
				next = (cur + 1) % vsize;
				V* v1 = v->AdjVertices()[pre];
				V* v2 = v->AdjVertices()[cur];
				V* v3 = v->AdjVertices()[next];

				weight_list.push_back(Cotangent(v, v1, v2) + Cotangent(v, v3, v2));
				neighbor_index.push_back(heMesh->Index(v2));
				weight_sum += weight_list.back();
			}
			for (size_t j = 0; j < neighbor_index.size(); j++)
			{
				triplet_list.push_back(Eigen::Triplet<double>(i, neighbor_index[j], -weight_list[j] / weight_sum));
			}
		}
	}
	laplace_mat.resize(nV, nV);
	laplace_mat.setZero();

	laplace_mat.setFromTriplets(triplet_list.begin(), triplet_list.end());
}


void Parameterize::Predecomposition()
{
	solver.compute(laplace_mat);
	if (solver.info() != Eigen::Success)
	{
		throw std::exception("Compute Matrix Is Error!");
		return;
	}
}

void Parameterize::Solve()
{
	size_t nV = heMesh->NumVertices();
	Eigen::VectorXd bx(nV), by(nV);
	Eigen::VectorXd x(nV), y(nV);

	bx.setZero(); by.setZero();

	for (int i = 0; i < boundary_index.size(); i++)
	{
		V* v = heMesh->Vertices()[boundary_index[i]];//三维边界点坐标
		bx[boundary_index[i]] = boundary_list[i][0];//二维边界点坐标
		by[boundary_index[i]] = boundary_list[i][1];
	}

	x = solver.solve(bx);
	y = solver.solve(by);

	for (size_t i = 0; i < nV; i++)
	{
		if (display_status == kon)//平面化
		{
			heMesh->Vertices()[i]->pos.at(0) = x(i);//x
			heMesh->Vertices()[i]->pos.at(1) = y(i);//y
			heMesh->Vertices()[i]->pos.at(2) = 0;//z
		}
		texcoords.push_back(pointf2(x(i), y(i)));
	}
}


///Setting IO
/// ////////////////////////////////////////////////////////////////////////////////////
/// 
/// 

void Parameterize::SetDisplay()
{
	display_status = kon;
}


void Parameterize::SetUniformMethod()
{
	method = kUniform;
}

void Parameterize::SetCotanMethod()
{
	method = kCotangent;
}

void Parameterize::SetMethod(Method m)
{
	method = m;
}

void Ubpa::Parameterize::SetSquare()
{
	boundary_type = kSquare;
}


void Ubpa::Parameterize::SetCircle()
{
	boundary_type = kCircle;
}


void Ubpa::Parameterize::SetBoundary(BoundaryType bt)
{
	boundary_type = bt;
}





//Mathematics
/////////////////////////////////////////////////////////////////////////////////////////////////
//距离
double Parameterize::Distance(V* v1, V* v2)
{
	return sqrt((v1->pos.at(0) - v2->pos.at(0)) * (v1->pos.at(0) - v2->pos.at(0)) 
		+ (v1->pos.at(1) - v2->pos.at(1)) * (v1->pos.at(1) - v2->pos.at(1)) 
		+ (v1->pos.at(2) - v2->pos.at(2)) * (v1->pos.at(2) - v2->pos.at(2)));
}
//Cos值
double Parameterize::Cosine(V* v0, V* v1, V* v2)
{	//余弦公式
	double edge1 = Distance(v0, v1);
	double edge2 = Distance(v1, v2);
	double edge3 = Distance(v0, v2);
	return (edge1 * edge1 + edge2 * edge2 - edge3 * edge3) / (2 * edge1 * edge2);
}
//Cotan
double Parameterize::Cotangent(V* v0, V* v1, V* v2)
{
	double cosine = Cosine(v0, v1, v2);
	return cosine / sqrt(1 - cosine * cosine);
}

