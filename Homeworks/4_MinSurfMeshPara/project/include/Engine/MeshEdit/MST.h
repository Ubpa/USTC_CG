#pragma once

#include "ShortestPath.h"

#include <Engine/Scene/SObj.h>

#include <Basic/HeapObj.h>

#include <UHEMesh/HEMesh.h>

#include <UGM/UGM>

#include <limits>

namespace Ubpa {
	class TriMesh;
	class Paramaterize;

	class MST : public HeapObj {
	public:
		MST(Ptr<SObj> triMeshObj);
	public:
		static const Ptr<MST> New(Ptr<SObj> triMeshObj) {
			return Ubpa::New<MST>(triMeshObj);
		}
	public:
		void Clear();
		bool Init(Ptr<SObj> triMeshObj);

		bool Run();
		Ptr<SObj> GetTreeObj();

	private:
		const std::vector<std::vector<ShortestPath::V*>> FindMST(const std::vector<ShortestPath::V*>& vertices);

	private:
		Ptr<ShortestPath> shortestPath;
		Ptr<SObj> treeObj;
	};
}
