#pragma once

#include <Engine/Scene/CmptSimu/MassSpring.h>
#include <UGM/normal.h>
#include <UGM/point.h>

#include <algorithm>
#include <vector>
using namespace std;


void Ubpa::MassSpring::Init(Ptr<TetMesh> Mesh)
{
	mesh_type = MassSpring::Tet;
	mesh = Mesh;
	pointlist = Mesh->GetPositions();
	edgelist = Mesh->GetEdgeIndice();
	NewSimu();

}

void Ubpa::MassSpring::Init(Ptr<TriMesh> Mesh)
{
	mesh_type = MassSpring::Tri;
	mesh = Mesh;
	pointlist = Mesh->GetPositions();
	GenEdge(Mesh->GetIndice());
	NewSimu();
}

bool Ubpa::MassSpring::Update(const std::vector<pointf3>& positions)
{
	pointlist = positions;

	if (mesh_type == MassSpring::Tet)
	{
		auto tem = CastTo<TetMesh>(mesh);
		tem->Update(positions);
	}
	else
	{
		auto tem = CastTo<TriMesh>(mesh);
		tem->Update(positions);
	}
	
	return true;
}


void Ubpa::MassSpring::GenEdge(const std::vector<unsigned>& facelist)
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
	for (size_t i = 1; i < 3 * numofface; i++)
	{
		if (edge[i][0] != edge[i - 1][0] || edge[i][1] != edge[i - 1][1])
		{
			idx.push_back(static_cast<unsigned>(i));
		}
	}
	for (size_t i = 0; i < idx.size(); i++)
	{
		edgelist.push_back(edge[idx[i]][0]);
		edgelist.push_back(edge[idx[i]][1]);
	}
}

bool Ubpa::MassSpring::SortEdge(std::vector<unsigned> f1, std::vector<unsigned> f2)
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
