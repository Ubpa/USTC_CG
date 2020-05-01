#pragma once

#include <string>

namespace Ubpa {
	class Scene;

	class SceneGenerator {
	public:
		static SceneGenerator& Instance() {
			static SceneGenerator instance;
			return instance;
		}

		Scene* GenScene(const std::string& path);
		// n : [0, 0]
		Scene* GenScene(size_t n);

		void PrintSerializedScene(Scene* scene);

	private:
		SceneGenerator();
	};
}
