#pragma once

#include <UGL/UGL>

namespace Ubpa {
	namespace SimpleLoader {
		class OGLResources {
		public:
			~OGLResources();
			gl::VertexArray* va{ nullptr };
			std::map<std::string, gl::VertexBuffer*> name2vb;
			gl::ElementBuffer* eb{ nullptr };
			std::vector<pointf3> positions;
			std::vector<normalf> normals;
			std::vector<pointf2> texcoords;
			std::vector<unsigned> indices;
			std::vector<vecf3> tangents;
		};

		// noise: position[p] += 0.05 * Xi * normal[p], Xi is random in [0, 1]
		OGLResources* LoadObj(const std::string& path, bool noise = false);
	}
}