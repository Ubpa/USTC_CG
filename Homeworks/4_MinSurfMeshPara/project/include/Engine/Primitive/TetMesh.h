#pragma once
#include <Engine/Primitive/TriMesh.h>


namespace Ubpa {
	class TetMesh : public Primitive {
	public:
		TetMesh(const std::vector<pointf3> &pointlist, const std::vector<unsigned> &tetrahedronlist)
		{
			Init(pointlist, tetrahedronlist);
		}

	public:
		static const Ptr<TetMesh> New(const std::vector<pointf3> &pointlist, const std::vector<unsigned> &tetrahedronlist) {
			return Ubpa::New<TetMesh>(pointlist, tetrahedronlist);
		}

	public:
		void Init(const std::vector<pointf3>& pointlist, const std::vector<unsigned>& tetrahedronlist);

		bool Update(const std::vector<pointf3>& positions);

	protected:
		virtual ~TetMesh() = default;

	private:
		virtual void Init_AfterGenPtr() override { };

	public:
		virtual const bboxf3 GetBBox() const override {
			return trimesh->GetBBox();
		}
	public:

		const std::vector<pointf3>& GetPositions() const { return pointlist; }
		const std::vector<unsigned>& GetFaceIndice() const { return facelist; }
		const std::vector<unsigned>& GetTetIndice() const { return tetrahedronlist; }
		const std::vector<unsigned>& GetEdgeIndice() const { return edgelist; }
		const Ptr<TriMesh> GetTriMesh() const { return trimesh; }
		//void SetTriMesh(Ptr<TriMesh> mesh)  { trimesh = mesh; }

	public:
		void UpdateNorm();

	private:
		void GenEdge();  //generate edgelist from facelist
		void GenFace();
		static bool SortEdge(std::vector<unsigned>f1, std::vector<unsigned>f2);
		static bool SortFace(std::vector<unsigned>f1, std::vector<unsigned>f2);
	private:
		std::vector<unsigned> tetrahedronlist;
		std::vector<pointf3> pointlist;
		std::vector<unsigned> edgelist;
		std::vector<unsigned> facelist;
		std::vector<normalf> normlist;
		Ptr<TriMesh> trimesh;

		std::vector<bool> face_isbound;
		//std::vector<std::vector<unsigned>> norm_id;

		bboxf3 box;
	};
}

