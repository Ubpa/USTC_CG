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
		};

		OGLResources* LoadObj(const std::string& path);
	}
}