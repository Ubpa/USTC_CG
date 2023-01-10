#include <Engine/MeshEdit/MST.h>

#include <Engine/Primitive/TriMesh.h>
#include <Engine/Scene/CmptGeometry.h>
#include <Engine/Scene/CmptMaterial.h>
#include <Engine/Material/BSDF_Frostbite.h>
#include <Engine/Scene/SObj.h>
#include <Engine/Primitive/Sphere.h>
#include <Engine/Scene/CmptTransform.h>


#include <set>
#include <map>
#include <queue>
#include <limits>

using namespace Ubpa;

using namespace std;

MST::MST(Ptr<SObj> triMeshObj)
	: shortestPath(ShortestPath::New(nullptr))
{
	Init(triMeshObj);
}

void MST::Clear() {
	shortestPath->Clear();
	treeObj = nullptr;
}

Ptr<SObj> MST::GetTreeObj() {
	return treeObj;
}

bool MST::Init(Ptr<SObj> triMeshObj) {
	Clear();
	if (!shortestPath->Init(triMeshObj)) {
		cout << "ERROR::MST::Init:" << endl
			<< "\t" << "ShortestPath::Init fail" << endl;
	}

	string treeName = "$[MST]tree";
	for (auto child : triMeshObj->GetChildren()) {
		if (child->name == treeName)
			treeObj = child;
	}
	if (!treeObj) {
		treeObj = SObj::New(triMeshObj, treeName);
		treeObj->AddComponent<CmptGeometry>(nullptr);
		treeObj->AddComponent<CmptMaterial>(BSDF_Frostbite::New(rgbf{ 1.f,0.f,0.f }));
	}

	return true;
}

bool MST::Run() {
	std::vector<ShortestPath::V*> vertices;
	constexpr size_t N = 6;
	for (size_t i = 0; i < N; i++)
		vertices.push_back(shortestPath->heMesh->Vertices()[Math::Rand_I() % shortestPath->heMesh->NumVertices()]);

	auto tree = FindMST(vertices);
	while (!treeObj->GetChildren().empty())
		treeObj->DelChild(*treeObj->GetChildren().begin());
	for (auto& path : tree) {
		auto pathObj = SObj::New(treeObj, "path");
		auto mesh = ShortestPath::GenMesh(path);
		pathObj->AddComponent<CmptGeometry>(mesh);
		pathObj->AddComponent<CmptMaterial>(BSDF_Frostbite::New(rgbf{ 1.f,0.f,0.f }));
	}
	for (auto v : vertices) {
		auto vObj = SObj::New(treeObj, "v");
		vObj->AddComponent<CmptGeometry>(Sphere::New());
		vObj->AddComponent<CmptTransform>(v->pos, scalef3{0.02f});
		vObj->AddComponent<CmptMaterial>(BSDF_Frostbite::New(rgbf{ 0.f,1.f,0.f }));
	}

	return true;
}

const vector<vector<ShortestPath::V*>> MST::FindMST(const vector<ShortestPath::V*>& vertices) {
	struct E {
		size_t i;
		size_t j;

		E(size_t i, size_t j) :i(std::min(i, j)), j(std::max(i, j)) {}
		bool operator<(const E& e)const {
			return i < e.i || i == e.i && j < e.j;
		}
	};

	struct WE {
		E e;
		float w;

		WE(E e, float w) :e(e), w(w) {}
		bool operator<(const WE& we)const {
			return w < we.w || w == we.w && e < we.e;
		}
	};

	size_t N = vertices.size();
	map<E, tuple<float, vector<ShortestPath::V*>>> e2wp;
	WE minWE{ E{static_cast<size_t>(-1),static_cast<size_t>(-1)},numeric_limits<float>::max() };
	for (size_t i = 0; i < N; i++) {
		for (size_t j = i + 1; j < N; j++) {
			auto dp = shortestPath->FindShortestPath(vertices[i], vertices[j]);
			E e(i, j);
			e2wp[e] = dp;
			float w = get<0>(dp);
			if (w < minWE.w)
				minWE = WE{ e, w };
		}
	}

	map<size_t, set<size_t>> v2adj;
	for (size_t i = 0; i < N; i++) {
		set<size_t>& adj = v2adj[i];
		for (size_t j = 0; j < N; j++) {
			if(j!=i)
				adj.insert(j);
		}
	}
	
	vector<vector<ShortestPath::V*>> tree;
	tree.push_back(get<1>(e2wp[minWE.e]));
	set<size_t> curTree;
	curTree.insert(minWE.e.i);
	curTree.insert(minWE.e.j);
	set<WE> weQ;
	for (auto v : v2adj[minWE.e.i]) {
		if (v == minWE.e.j)
			continue;
		E iv{ minWE.e.i, v };
		weQ.insert({ iv, get<0>(e2wp[iv]) });
	}
	for (auto v : v2adj[minWE.e.j]) {
		if (v == minWE.e.i)
			continue;
		E jv{ minWE.e.j, v };
		weQ.insert({ jv, get<0>(e2wp[jv]) });
	}

	while (curTree.size() != N) {
		auto iter = weQ.begin();
		E e = iter->e;
		float w = iter->w;
		weQ.erase(iter);

		tree.push_back(get<1>(e2wp[e]));

		auto Add = [&](size_t i) {
			if (curTree.find(i) != curTree.end())
				return;

			for (auto v : curTree) {
				E iv{ i,v };
				weQ.erase({ iv , get<0>(e2wp[iv]) });
			}
			curTree.insert(i);
		};
		Add(e.i);
		Add(e.j);
	}

	return tree;
}
