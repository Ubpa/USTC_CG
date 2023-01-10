#pragma once

#include <Engine/Scene/Component.h>
#include <Engine/Scene/CmptSimu/MassSpring.h>
#include <vector>


namespace Ubpa {
	class CmptSimulate : public Component {
	public:
		/*CmptSimulate(
			Ptr<SObj> sobj, Ptr<Simulate> simu,float stiff = 1000.0f)
			: Component(sobj), simulate(simu),stiffness(stiff) { }*/
		CmptSimulate(
			Ptr<SObj> sobj = nullptr, Ptr<Primitive> primitive = nullptr)
			: Component(sobj), primitive(primitive) { }


	public:
		/*static const Ptr<CmptSimulate> New(Ptr<SObj> sobj, Ptr<Simulate> simu, float stiff = 1000.0f) {
			return Ubpa::New<CmptSimulate>(sobj, simu, stiff);
		}*/
		static const  Ptr<CmptSimulate> New(Ptr<SObj> sobj, Ptr<Primitive> primitive) {
			return Ubpa::New<CmptSimulate>(sobj, primitive);
		}
	protected:
		virtual ~CmptSimulate() = default;

	public:
		void Init(std::vector<unsigned>& fix) { SetFix(fix); };
		// theta is in radian

	public:
		const std::vector<unsigned>& GetFix() const { return fix_id; };
		float GetStiff() const { return CastTo<MassSpring>(primitive)->GetSimu()->GetStiff(); }
		Ptr<Primitive> GetMesh() {
			return primitive;
		}// return CastTo<MassSpring>(primitive); }

	public:
		void SetStiff(float stiff) { CastTo<MassSpring>(primitive)->GetSimu()->SetStiff(stiff); }
		void SetFix(std::vector<unsigned>& fix) { fix_id = fix; }
		void Restore() { CastTo<MassSpring>(primitive)->GetSimu()->Restore(); }
		
		void SetLeftFix() { CastTo<MassSpring>(primitive)->GetSimu()->SetLeftFix(); }
		void SetRightFix() { CastTo<MassSpring>(primitive)->GetSimu()->SetRightFix(); }
		void SetUpFix() { CastTo<MassSpring>(primitive)->GetSimu()->SetUpFix(); }
		void SetDownFix() { CastTo<MassSpring>(primitive)->GetSimu()->SetDownFix(); }
		void ClearFix() { CastTo<MassSpring>(primitive)->GetSimu()->ClearFix(); }
		
		void SetEulerMethod() { CastTo<MassSpring>(primitive)->GetSimu()->SetEulerMethod(); }
		void SetPDMethod() { CastTo<MassSpring>(primitive)->GetSimu()->SetPDMethod(); }
		//void AddFix() { CastTo<MassSpring>(primitive)->GetSimu()->AddFix(); };

	private:
		std::vector<unsigned> fix_id;
		float stiffness = 1000;
		Ptr<Primitive> primitive;
	};
}
