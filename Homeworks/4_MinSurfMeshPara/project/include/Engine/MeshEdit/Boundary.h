#pragma once
#pragma once
#include <UHEMesh/HEMesh.h>
#include <UGM/UGM>
#include <Basic/HeapObj.h>

namespace Ubpa {
	class TriMesh;

	class Boundary
	{
	public:
		Boundary() {};
		virtual ~Boundary() {};

	protected:
		class V;
		class E;
		class P;
		class V : public TVertex<V, E, P> {
		public:
			vecf3 pos;
		};
		class E : public TEdge<V, E, P> { };
		class P :public TPolygon<V, E, P> { };

	public:
		virtual void Setting(std::vector<size_t> boundary_index, std::vector<pointf2>& boundary_list) {};
	};
}