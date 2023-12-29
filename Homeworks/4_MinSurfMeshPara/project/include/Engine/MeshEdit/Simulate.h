#pragma once

#include <Basic/HeapObj.h>
//#include <Engine/Primitive/MassSpring.h>
#include <UGM/UGM>
#include <Eigen/Dense>
#include <Eigen/Sparse>
namespace Ubpa {
	class Simulate : public HeapObj {
	public:
		enum Method {
			kEuler,
			kPD,
	}method;
		
	public:
		Simulate(const std::vector<pointf3>& plist,
			const std::vector<unsigned>& elist) {
			method = kEuler;
			edgelist = elist;

			//positions = std::vector<pointf3>(plist);
			positions_backup = std::vector<pointf3>(plist);
		};
	public:
		static const Ptr<Simulate> New(const std::vector<pointf3>& plist,
			const std::vector<unsigned> &elist) {
			return Ubpa::New<Simulate>(plist, elist);
		}
	public:
		// 对外接口 IO
		// clear cache data
		void Clear();
		// init cache data (eg. half-edge structure) for Run()
		bool Init();
		// call it after Init()
		bool Run();
		
		const std::vector<pointf3>& GetPositions() const { return positions; };	
		const float GetStiff() { return stiff; };
		void SetStiff(float k) { stiff = k; Init();};
		const float GetTimeStep() { return h; };
		void SetTimeStep(float k) { h = k; Init();};
		std::vector<unsigned>& GetFix() { return this->fixed_id; };
		void SetFix(const std::vector<unsigned>& f) { this->fixed_id = f; Init();};
		const std::vector<pointf3>& GetVelocity() { return velocity; };
		//void SetVelocity(const std::vector<pointf3>& v) { velocity = v; };
		void Restore();
		void SetLeftFix();
		void SetRightFix();
		void SetUpFix();
		void SetDownFix();
		//void AddFix();
		void ClearFix();
		void SetEulerMethod() { 
			method = kEuler; 
			h = 0.0003f;
			Init();
		};
		void SetPDMethod() { 
			method = kPD; 
			h = 0.003f;
			Init();
		}
		
	private:
		// kernel part of the algorithm
		void SimulateOnce();

		void EulerMethod();

		void InitY();
		
		void NewtonIteration(std::vector<pointf3>& cur_positions);

		float NewtonIteration();

		void UpdateStep(const std::vector<pointf3>& cur_positions);

		float UpdateStep();
		
		void ProjectiveDynamics();

		void UpdateVelocity(const std::vector<pointf3>&, const std::vector<pointf3>&);

		void UpdateVelocity();

		void InitMat();

		void UpdateD();
		
		float UpdateX();
		
		float IterationOnceOptimize();
	private:
		
		void Pos2Mat(const std::vector<pointf3>& pos, Eigen::MatrixXf& mat);
		
		void Mat2Pos(const Eigen::MatrixXf& mat, std::vector<pointf3>& pos);
		
	private:
		int count = 0;

		float h = 0.0003f;
		float stiff = 100.00f;
		float mass = 1;
		float gravity = 10;

		std::vector<unsigned> fixed_id;  //fixed point id
		std::vector<pointf3> fixed_coords;	//fixed point coordinate
		
		//外力
		std::vector<vecf3> force_ext;
		size_t select_index = 0;

		//mesh data
		std::vector<unsigned> edgelist;//边坐标，奇数偶数索引对应一条边上的两个点
		std::vector<float> length_list;//边长



		
		//simulation data
		std::vector<pointf3> positions;//位置
		std::vector<pointf3> velocity;//速度
		std::vector<pointf3> positions_backup;
	private:

		std::vector<pointf3> y_list;
		std::vector<Eigen::Vector3f> step_list;
		Eigen::MatrixXf step_vec;
		
		Eigen::SparseMatrix<float> L_spmat;
		Eigen::SparseMatrix<float> J_spmat;
		Eigen::SparseMatrix<float> K_spmat;
		Eigen::SparseMatrix<float> A_optim_spmat;
				
		Eigen::MatrixXf d_vec;
		Eigen::MatrixXf x_vec;
		Eigen::MatrixXf bb_vec;
		Eigen::MatrixXf y_vec;
		Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>> solver;
	};
}
