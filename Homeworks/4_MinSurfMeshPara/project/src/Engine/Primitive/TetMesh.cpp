#pragma once

#include <Engine/Primitive/TetMesh.h>

#include <UGM/normal.h>
#include <UGM/point.h>

#include <algorithm>
#include <vector>
using namespace std;
void Ubpa::TetMesh::Init(const std::vector<pointf3>& plist, const std::vector<unsigned>& tlist)
{
	pointlist = plist;
	tetrahedronlist = tlist;
	vector<pointf2> texcoords;
	vector<normalf> tangents;
	texcoords.resize(pointlist.size(), { 0,0 });
	GenFace();
	GenEdge(); 
	UpdateNorm(); 
	//normlist.resize(pointlist.size(), normalf(0, 0, 1));
	trimesh = TriMesh::New(facelist, pointlist, normlist, texcoords, tangents);
}

bool Ubpa::TetMesh::Update(const std::vector<pointf3>& positions)
{
	pointlist = positions;
	UpdateNorm();
	trimesh->Update(pointlist);
	//trimesh->GetNormals() = normlist;
	return true;
}

void Ubpa::TetMesh::UpdateNorm()
{
	//normlist.clear();
	//normlist.resize(pointlist.size(), normalf(0, 0, 1));
	//std::vector<pointf3> normal(pointlist.size(),pointf3(0,0,0));
	//for (int i = 0; i < facelist.size() / 3; i++)
	//{
	//	pointf3 a, b, c;
	//	a[0] = pointlist[facelist[3 * i]][0] - pointlist[facelist[3 * i + 1]][0];
	//	a[1] = pointlist[facelist[3 * i]][1] - pointlist[facelist[3 * i + 1]][1];
	//	a[2] = pointlist[facelist[3 * i]][2] - pointlist[facelist[3 * i + 1]][2];

	//	b[0] = pointlist[facelist[3 * i + 2]][0] - pointlist[facelist[3 * i + 1]][0];
	//	b[1] = pointlist[facelist[3 * i + 2]][1] - pointlist[facelist[3 * i + 1]][1];
	//	b[2] = pointlist[facelist[3 * i + 2]][2] - pointlist[facelist[3 * i + 1]][2];

	//	c[0] = b[1] * a[2] - b[2] * a[1];
	//	c[1] = b[2] * a[0] - b[0] * a[2];
	//	c[2] = b[0] * a[1] - b[1] * a[0];
	//	for (int j = 0; j < norm_id[i].size(); j++)
	//	{
	//		normal[norm_id[i][j]][0] += c[0];
	//		normal[norm_id[i][j]][1] += c[1];
	//		normal[norm_id[i][j]][2] += c[2];
	//	}
	//}
	//for (int i = 0; i < pointlist.size(); i++)
	//{
	//	float norm = sqrt(normal[i][0] * normal[i][0] + normal[i][1] * normal[i][1] + normal[i][2] * normal[i][2]);
	//	normalf temnormal;
	//	if (abs(norm) < 1e-7)
	//	{
	//		normlist[i] = normalf(0, 0, 0);
	//	}
	//	else
	//	{
	//		normlist[i] = normalf(normal[i][0] / norm, normal[i][1] / norm, normal[i][2] / norm);
	//	}
	//}


	normlist.clear();
	normlist.resize(pointlist.size(), normalf(0.f));
	for (int i = 0; i < facelist.size() / 3; i++)
	{
		if (face_isbound[i] == false)
		{
			continue;
		}
		auto v0 = facelist[3 * i];
		auto v1 = facelist[3 * i + 1];
		auto v2 = facelist[3 * i + 2];

		auto d10 = pointlist[v0] - pointlist[v1];
		auto d12 = pointlist[v2] - pointlist[v1];
		auto wN = d12.cross(d10);

		normlist[v0] += wN.cast_to<normalf>();
		normlist[v1] += wN.cast_to<normalf>();
		normlist[v2] += wN.cast_to<normalf>();

	}

	for (int i = 0; i < pointlist.size(); i++)
	{
		if (normlist[i].norm() > 1e-6)
		{
			normlist[i].normalize_self();
		}
	}
}


void Ubpa::TetMesh::GenEdge()
{
	size_t numofface = facelist.size() / 3;
	vector<vector<unsigned>> edge;
	edge.resize(numofface * 3);
	vector<unsigned> tem(3);
	for (size_t i = 0; i < numofface; i++)
	{
		tem[0] = facelist[3 * i];
		tem[1] = facelist[3 * i + 1];
		tem[2] = facelist[3 * i + 2];
		std::sort(tem.begin(), tem.end());
		edge[3 * i].push_back(tem[0]);
		edge[3 * i].push_back(tem[1]);
		edge[3 * i + 1].push_back(tem[0]);
		edge[3 * i + 1].push_back(tem[2]);
		edge[3 * i + 2].push_back(tem[1]);
		edge[3 * i + 2].push_back(tem[2]);
	}
	std::sort(edge.begin(), edge.end(), SortEdge);
	vector<int> idx;
	if (edge[0][0] != edge[1][0] || edge[0][1] != edge[1][1])
	{
		idx.push_back(0);
	}
	for (int i = 1; i < 3 * numofface; i++)
	{
		if (edge[i][0] != edge[i - 1][0] || edge[i][1] != edge[i - 1][1])
		{
			idx.push_back(i);
		}
	}
	for (int i = 0; i < idx.size(); i++)
	{
		edgelist.push_back(edge[idx[i]][0]);
		edgelist.push_back(edge[idx[i]][1]);
	}
}
void Ubpa::TetMesh::GenFace()
{
	size_t numberoftetrahedra = tetrahedronlist.size() / 4;
	vector<vector<unsigned>> face;
	vector<unsigned> tem(4);
	face.resize(4 * numberoftetrahedra);
	for (size_t i = 0; i < numberoftetrahedra; i++)
	{
		tem[0] = tetrahedronlist[4 * i];
		tem[1] = tetrahedronlist[4 * i + 1];
		tem[2] = tetrahedronlist[4 * i + 2];
		tem[3] = tetrahedronlist[4 * i + 3];
		std::sort(tem.begin(), tem.end());
		face[4 * i].push_back(tem[0]);
		face[4 * i].push_back(tem[1]);
		face[4 * i].push_back(tem[2]);
		face[4 * i].push_back(static_cast<unsigned>(i));
		face[4 * i + 1].push_back(tem[0]);
		face[4 * i + 1].push_back(tem[1]);
		face[4 * i + 1].push_back(tem[3]);
		face[4 * i + 1].push_back(static_cast<unsigned>(i));
		face[4 * i + 2].push_back(tem[0]);
		face[4 * i + 2].push_back(tem[2]);
		face[4 * i + 2].push_back(tem[3]);
		face[4 * i + 2].push_back(static_cast<unsigned>(i));
		face[4 * i + 3].push_back(tem[1]);
		face[4 * i + 3].push_back(tem[2]);
		face[4 * i + 3].push_back(tem[3]);
		face[4 * i + 3].push_back(static_cast<unsigned>(i));
	}
	std::sort(face.begin(), face.end(), SortFace);
	vector<unsigned> idx;
	if (face[0][0] != face[1][0] || face[0][1] != face[1][1] || face[0][2] != face[1][2])
	{
		idx.push_back(0);
	}
	for (size_t i = 1; i < 4 * numberoftetrahedra; i++)
	{
		if (face[i][0] != face[i - 1][0] || face[i][1] != face[i - 1][1] || face[i][2] != face[i - 1][2])
		{
			idx.push_back(unsigned(i));
		}
	}
	/*if (face[4 * numberoftetrahedra - 1][0] != face[4 * numberoftetrahedra - 2][0] || face[4 * numberoftetrahedra - 1][1] != face[4 * numberoftetrahedra - 2][1] || face[4 * numberoftetrahedra - 1][2] != face[4 * numberoftetrahedra - 2][2])
	{
		idx.push_back(4 * numberoftetrahedra - 1);
	}*/

	size_t numberoffaces = idx.size();
	//vector<bool> face_isbound;
	face_isbound.resize(numberoffaces, false);
	vector<bool> point_isbound;
	point_isbound.resize(pointlist.size(), false);

	for (int i = 0; i < numberoffaces - 1; i++)
	{
		if (face[idx[i]][0] != face[idx[i] + 1][0] || face[idx[i]][1] != face[idx[i] + 1][1] || face[idx[i]][2] != face[idx[i] + 1][2])
		{
			face_isbound[i] = true;
			//point_isbound[face[idx[i]][0]] = true;
			//point_isbound[face[idx[i]][1]] = true;
			//point_isbound[face[idx[i]][2]] = true;
		}
	}
	if (idx[numberoffaces - 1] == 4 * numberoftetrahedra - 1)
	{
		face_isbound[numberoffaces - 1] = true;
		//point_isbound[face[numberoffaces - 1][0]] = true;
		//point_isbound[face[numberoffaces - 1][1]] = true;
		//point_isbound[face[numberoffaces - 1][2]] = true;
	}

	//norm_id.resize(numberoffaces);
	//for (int i = 0; i < numberoffaces; i++)
	//{
	//	if (face_isbound[i] == true)
	//	{
	//		norm_id[i].push_back(face[idx[i]][0]);
	//		norm_id[i].push_back(face[idx[i]][1]);
	//		norm_id[i].push_back(face[idx[i]][2]);
	//	}
	//	else
	//	{
	//		//if (point_isbound[face[idx[i]][0]] == false)
	//		//{
	//		//	norm_id[i].push_back(face[idx[i]][0]);
	//		//}
	//		//if (point_isbound[face[idx[i]][1]] == false)
	//		//{
	//		//	norm_id[i].push_back(face[idx[i]][1]);
	//		//}
	//		//if (point_isbound[face[idx[i]][2]] == false)
	//		//{
	//		//	norm_id[i].push_back(face[idx[i]][2]);
	//		//}
	//	}
	//}


	pointf3 v0, v1, v2;

	for (size_t i = 0; i < numberoffaces; i++)
	{
		int pointid = tetrahedronlist[4 * face[idx[i]][3]] + tetrahedronlist[4 * face[idx[i]][3] + 1] +
			tetrahedronlist[4 * face[idx[i]][3] + 2] + tetrahedronlist[4 * face[idx[i]][3] + 3] - face[idx[i]][0] - face[idx[i]][1] - face[idx[i]][2];
		v0[0] = pointlist[face[idx[i]][1]][0] - pointlist[face[idx[i]][0]][0];
		v0[1] = pointlist[face[idx[i]][1]][1] - pointlist[face[idx[i]][0]][1];
		v0[2] = pointlist[face[idx[i]][1]][2] - pointlist[face[idx[i]][0]][2];

		v1[0] = pointlist[face[idx[i]][2]][0] - pointlist[face[idx[i]][0]][0];
		v1[1] = pointlist[face[idx[i]][2]][1] - pointlist[face[idx[i]][0]][1];
		v1[2] = pointlist[face[idx[i]][2]][2] - pointlist[face[idx[i]][0]][2];

		v2[0] = pointlist[pointid][0] - pointlist[face[idx[i]][0]][0];
		v2[1] = pointlist[pointid][1] - pointlist[face[idx[i]][0]][1];
		v2[2] = pointlist[pointid][2] - pointlist[face[idx[i]][0]][2];

		float v = v0[0] * (v1[1] * v2[2] - v1[2] * v2[1]) - v0[1] * (v1[0] * v2[2] - v1[2] * v2[0]) + v0[2] * (v1[0] * v2[1] - v1[1] * v2[0]);
		if (v > 0)
		{
			facelist.push_back(face[idx[i]][0]);
			facelist.push_back(face[idx[i]][2]);
			facelist.push_back(face[idx[i]][1]);
		}
		else
		{
			facelist.push_back(face[idx[i]][0]);
			facelist.push_back(face[idx[i]][1]);
			facelist.push_back(face[idx[i]][2]);
		}
	}
}

bool Ubpa::TetMesh::SortEdge(std::vector<unsigned> f1, std::vector<unsigned> f2)
{
	if (f1[0] < f2[0])
	{
		return true;
	}
	if (f1[0] > f2[0])
	{
		return false;
	}


	if (f1[1] < f2[1])
	{
		return true;
	}
	if (f1[1] > f2[1])
	{
		return false;
	}

	return false;
}

bool Ubpa::TetMesh::SortFace(std::vector<unsigned> f1, std::vector<unsigned> f2)
{
	if (f1[0] < f2[0])
	{
		return true;
	}
	if (f1[0] > f2[0])
	{
		return false;
	}


	if (f1[1] < f2[1])
	{
		return true;
	}
	if (f1[1] > f2[1])
	{
		return false;
	}

	if (f1[2] < f2[2])
	{
		return true;
	}
	if (f1[2] > f2[2])
	{
		return false;
	}


	return false;
}
