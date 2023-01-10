#include <Engine/MeshEdit/ShortestPath.h>

#include <Engine/Primitive/TriMesh.h>
#include <Engine/Scene/CmptGeometry.h>
#include <Engine/Scene/CmptMaterial.h>
#include <Engine/Material/BSDF_Frostbite.h>
#include <Engine/Scene/SObj.h>

#include <set>
#include <map>
#include <queue>
#include <limits>

using namespace Ubpa;

using namespace std;

ShortestPath::ShortestPath(Ptr<SObj> triMeshObj)
	: heMesh(make_shared<HEMesh<V>>())
{
	Init(triMeshObj);
}

void ShortestPath::Clear() {
	heMesh->Clear();
	triMeshObj = nullptr;
	pathObj = nullptr;
}

Ptr<SObj> ShortestPath::GetPathObj() {
	return pathObj;
}

bool ShortestPath::Init(Ptr<SObj> triMeshObj) {
	Clear();

	if (triMeshObj == nullptr)
		return true;

	auto triMesh = CastTo<TriMesh>(triMeshObj->GetComponent<CmptGeometry>()->primitive);

	if (!triMesh || triMesh->GetType() == TriMesh::INVALID) {
		printf("ERROR::ShortestPath::Init:\n"
			"\t""!triMesh || trimesh is invalid\n");
		return false;
	}

	size_t nV = triMesh->GetPositions().size();
	vector<vector<size_t>> triangles;
	triangles.reserve(triMesh->GetTriangles().size());
	for (auto triangle : triMesh->GetTriangles())
		triangles.push_back({ triangle->idx[0], triangle->idx[1], triangle->idx[2] });
	heMesh->Reserve(nV);
	heMesh->Init(triangles);

	if (!heMesh->IsTriMesh()) {
		printf("ERROR::ShortestPath::Init:\n"
			"\t""trimesh is not a triangle mesh\n");
		heMesh->Clear();
		return false;
	}

	for (int i = 0; i < nV; i++) {
		auto v = heMesh->Vertices().at(i);
		v->pos = triMesh->GetPositions()[i];
	}

	this->triMeshObj = triMeshObj;
	string pathName = "$[ShortestPath]shortestPath";
	for (auto child : triMeshObj->GetChildren()) {
		if (child->name == pathName)
			pathObj = child;
	}
	if(!pathObj) {
		pathObj = SObj::New(triMeshObj, pathName);
		pathObj->AddComponent<CmptGeometry>(nullptr);
		pathObj->AddComponent<CmptMaterial>(BSDF_Frostbite::New(rgbf{ 1.f,0.f,0.f }));
	}
	return true;
}

bool ShortestPath::Run() {
	if (heMesh->IsEmpty() || !triMeshObj) {
		printf("ERROR::ShortestPath::Run\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}

	auto v0 = heMesh->Vertices()[Math::Rand_I() % heMesh->NumVertices()];
	auto v1 = heMesh->Vertices()[Math::Rand_I() % heMesh->NumVertices()];
	auto [dist, path] = FindShortestPath(v0, v1);

	pathObj->GetComponent<CmptGeometry>()->primitive = GenMesh(path);

	return true;
}

tuple<float, vector<ShortestPath::V*>> ShortestPath::FindShortestPath(V* v0, V* v1) {
	auto& vertices = heMesh->Vertices();
	for (auto v : vertices) {
		v->pre = nullptr;
		v->weight = std::numeric_limits<float>::max();
	}

	v0->weight = 0.f;

	auto comp = [](V* left, V* right)->bool {
		return left->weight < right->weight ||
			(left->weight == right->weight) && left < right;
	};
	set<V*, decltype(comp)> vQ(comp);
	vQ.insert(v0);
	bool found = false;
	while (!vQ.empty()) {
		auto iter = vQ.begin();
		V* v = *iter;
		vQ.erase(iter);

		if (v == v1) {
			found = true;
			break;
		}

		for (auto adjV : v->AdjVertices()) {
			float dist = pointf3::distance(adjV->pos, v->pos);
			if (v->weight + dist < adjV->weight) {
				vQ.erase(adjV);
				adjV->pre = v;
				adjV->weight = v->weight + dist;
				vQ.insert(adjV);
			}
		}
	}

	if (!found) {
		cout << "ERROR::ShortestPath::FindShortestPath:" << endl
			<< "\t" << "v0 can't not reach to v1" << endl;
		return { numeric_limits<float>::max(), vector<ShortestPath::V*> {} };
	}

	vector<V*> path;
	for (auto v = v1; v != nullptr; v = v->pre)
		path.push_back(v);

	return { v1->weight,path };
}

Ptr<TriMesh> ShortestPath::GenMesh(const std::vector<V*>& path) {
	vector<pointf3> positions;
	positions.reserve(2 * path.size());
	vector<unsigned> indices;
	indices.reserve(6 * path.size());
	vecf3 off0(0, 0.01f, 0);
	vecf3 off1(0, -0.01f, 0);
	positions.push_back(path[0]->pos + off0);
	positions.push_back(path[1]->pos + off1);
	for (size_t i = 1; i < path.size(); i++) {
		positions.push_back(path[i]->pos + off0);
		positions.push_back(path[i]->pos + off1);

		size_t curIndices[6] = {
			2 * i - 2,2 * i - 1,2 * i,
			2 * i + 1,2 * i,2 * i - 1
		};
		for (auto idx : curIndices)
			indices.push_back(static_cast<unsigned>(idx));
	}
	auto pathMesh = TriMesh::New(indices, positions);
	return pathMesh;
}