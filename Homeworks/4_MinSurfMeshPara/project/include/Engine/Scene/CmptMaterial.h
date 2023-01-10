#pragma once

#include <Engine/Scene/Component.h>

namespace Ubpa {
	class Material;

	class CmptMaterial : public Component {
	public:
		CmptMaterial(Ptr<SObj> sobj = nullptr, Ptr<Material> material = nullptr)
			: Component(sobj), material(material) { }

	public:
		static const Ptr<CmptMaterial> New(Ptr<SObj> sobj, Ptr<Material> material) {
			return Ubpa::New<CmptMaterial>(sobj, material);
		}

	protected:
		virtual ~CmptMaterial() = default;

	public:
		Ptr<Material> material;
	};
}
