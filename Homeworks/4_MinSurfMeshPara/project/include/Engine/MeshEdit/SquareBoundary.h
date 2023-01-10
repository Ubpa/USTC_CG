#pragma once
#include<Engine/MeshEdit/Boundary.h>
namespace Ubpa {
	class SquareBoundary:
		public Boundary
	{
	public:
		SquareBoundary();
		~SquareBoundary();
	public:
		void Setting(std::vector<size_t> boundary_index, std::vector<pointf2>& boundary_list);
	};
}