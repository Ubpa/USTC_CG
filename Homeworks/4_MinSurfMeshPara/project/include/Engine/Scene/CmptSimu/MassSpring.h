#pragma once
#include <Engine/Primitive/TriMesh.h>
#include <Engine/Primitive/TetMesh.h>
#include <Engine/MeshEdit/Simulate.h>
namespace Ubpa {
	class MassSpring final: public Primitive {
	public:

		enum Mesh_TYPE
		{
			Tri,
			Tet,
		};
		MassSpring(Ptr<TetMesh> Mesh)
		{
			Init(Mesh);
		}
		MassSpring(Ptr<TriMesh> Mesh)
		{
			Init(Mesh);
		}

	public:
		static const Ptr<MassSpring> New(Ptr<TetMesh> Mesh) {
			return Ubpa::New<MassSpring>(Mesh);
		}

		static const Ptr<MassSpring> New(Ptr<TriMesh> Mesh) {
			return Ubpa::New<MassSpring>(Mesh);
		}

	public:
		void Init(Ptr<TetMesh> Mesh);
		void Init(Ptr<TriMesh> Mesh);

		bool Update(const std::vector<pointf3>& positions);

	private:

	private:
		virtual ~MassSpring() = default;

	private:
		virtual void Init_AfterGenPtr() override {
		};
	public:
		virtual const bboxf3 GetBBox() const override {
			return mesh->GetBBox();
		}
	public:

		const std::vector<pointf3>& GetPositions() const { return pointlist; }
		const std::vector<unsigned>& GetEdgeIndice() const { return edgelist; }
		const Ptr<Primitive> GetMesh() const { return mesh; }
		const Ptr<TriMesh> GetTriMesh() const { if (mesh_type == Tri) return CastTo<TriMesh>(mesh); else return CastTo<TetMesh>(mesh)->GetTriMesh(); };
		const MassSpring::Mesh_TYPE GetMeshType() const { return mesh_type; }

		


	public:
		void GenEdge(const std::vector<unsigned>& facelist);  //generate edgelist from facelist
		
		void RunSimu() { simulation->Run(); Update(simulation->GetPositions()); };
		void InitSimu() { simulation->Init(); Update(simulation->GetPositions()); };
		Ptr<Simulate> GetSimu() {return simulation;};
	private:
		void NewSimu() { simulation = Simulate::New(pointlist, edgelist); };
		static bool SortEdge(std::vector<unsigned>f1, std::vector<unsigned>f2);
	private:
		std::vector<unsigned> tetrahedronlist;
		std::vector<pointf3> pointlist;
		std::vector<unsigned> edgelist;

		Ptr<Primitive> mesh;
		Mesh_TYPE mesh_type;

		Ptr<Simulate> simulation;


		bboxf3 box;
	};
}

