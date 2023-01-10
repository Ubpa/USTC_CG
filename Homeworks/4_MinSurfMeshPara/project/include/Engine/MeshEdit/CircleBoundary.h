#pragma once
#pragma once
#include<Engine/MeshEdit/Boundary.h>
namespace Ubpa {
	class CircleBoundary :public Boundary
	{
	public:
		CircleBoundary();
		~CircleBoundary();
	public:
		void Setting(std::vector<size_t> boundary_index, std::vector<pointf2>& boundary_list);
	};
}