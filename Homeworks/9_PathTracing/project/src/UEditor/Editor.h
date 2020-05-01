#pragma once

#include <UScene/core.h>

namespace Ubpa {
	class Editor {
	public:
		Read<Editor, Scene*> scene;

		static Editor& Instance() noexcept {
			static Editor instance;
			return instance;
		}

		void SetCamera(SObj* cameraobj);

		static void OnRegister();

	private:
		Editor();
		~Editor();
	};
}
