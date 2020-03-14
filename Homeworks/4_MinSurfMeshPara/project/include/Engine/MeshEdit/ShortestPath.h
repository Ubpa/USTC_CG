#pragma once

#include <Engine/Scene/SObj.h>

#include <Basic/HeapObj.h>

#include <UHEMesh/HEMesh.h>

#include <UGM/UGM>

#include <limits>

namespace Ubpa {
	class TriMesh;
	class Paramaterize;
	class MST;

	class ShortestPath : public HeapObj {
	public:
		struct V;
		struct E;
		struct P;
		struct V : public TVertex<V, E, P> {
			pointf3 pos;
			V* pre;
			float weight;
		};
		struct E : public TEdge<V, E, P> { };
		struct P :public TPolygon<V, E, P> { };

	public:
		ShortestPath(Ptr<SObj> triMeshObj);
	public:
		static const Ptr<ShortestPath> New(Ptr<SObj> triMeshObj) {
			return Ubpa::New<ShortestPath>(triMeshObj);
		}
	public:
		void Clear();
		bool Init(Ptr<SObj> triMeshObj);

		bool Run();
		Ptr<SObj> GetPathObj();

		std::tuple<float, std::vector<V*>> FindShortestPath(V* v0, V* v1);
		static Ptr<TriMesh> GenMesh(const std::vector<V*>& positions);

	private:
		Ptr<SObj> triMeshObj;
		Ptr<SObj> pathObj;
		const Ptr<HEMesh<V>> heMesh; // vertice order is same with triMesh

		friend class MST;
	};
}
