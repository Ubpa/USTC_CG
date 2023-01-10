#pragma once

#include <Basic/HeapObj.h>
#include <Engine/MeshEdit/Boundary.h>
#include <Engine/MeshEdit/SquareBoundary.h>
#include <Engine/MeshEdit/CircleBoundary.h>
#include <Eigen/Sparse>
/*
将三维点通过合理的方法投影到平面上
//已知：
//		边界值，邻点间相邻关系，重心坐标公式
//求出：
//		找到内部点的位置
*/
namespace Ubpa {
	class TriMesh;
	class MinSurf;

	// mesh boundary == 1
	class Parameterize : public HeapObj {
	public:
		enum Method
		{
			kUniform,
			kCotangent
		}method;

		enum BoundaryType {
			kSquare,
			kCircle
		}boundary_type;
		
		enum DisplayStatus
		{
			koff,
			kon
		}display_status;

		
	public:
		Parameterize(Ptr<TriMesh> triMesh);
	public:
		static const Ptr<Parameterize> New(Ptr<TriMesh> triMesh) {
			return Ubpa::New<Parameterize>(triMesh);
		}
	//Core
	public:
		void Clear();
		bool Init(Ptr<TriMesh> triMesh);
		void Paramaterization();
		bool Run();
	//IO
	public:
		void SetDisplay();
		void SetUniformMethod();
		void SetCotanMethod();
		void SetMethod(Method);
		void SetSquare();
		void SetCircle();
		void SetBoundary(BoundaryType);
	private:
		void InitBoundary();
		void InitUniform();
		void InitCotangent();
		void InitMatrix();
		//	Sparse Matrix Predecomposition
		void Predecomposition();

		void Solve();

	private:
		class V;
		class E;
		class P;
		class V : public TVertex<V, E, P> {
		public:
			vecf3 pos;
		};
		class E : public TEdge<V, E, P> { };
		class P :public TPolygon<V, E, P> { };
	
	private:
		double Distance(V* v1, V* v2);
		double Cosine(V* v0, V* v1, V* v2);
		double Cotangent(V* v0, V* v1, V* v2);
		
	private:
		Ptr<TriMesh> triMesh;
		const Ptr<HEMesh<V>> heMesh; 
		Ptr< Boundary> boundary_ptr;
		Eigen::SparseMatrix<double> laplace_mat;
		Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
		std::vector<size_t> boundary_index;
		std::vector<pointf2> boundary_list;
		std::vector<pointf2>texcoords;
	};
	
}
