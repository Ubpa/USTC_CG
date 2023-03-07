#pragma once

#include <UDP/Basic/Read.h>

#include <set>

namespace Ubpa {
	class Scene;
	class SObj;

	class SceneMngr {
	public:
		static SceneMngr& Instance() noexcept {
			static SceneMngr instance;
			return instance;
		}

		// life cycle
		// 1. start toStartScenes
		// 2. update actived_scene
		// 3. update updatetingScenes (exclude actived_scene)
		// 4. update toStartScenes (exclude actived_scene)
		// 5. stop toStopScenes
		// 6. SceneMngr::Update
		// - insert scene of toStartScenes to updatetingScenes
		// - clear toStartScenes
		// - erase scene of toStopScenes from updatetingScenes
		// - clear toStopScenes (may reset actived_scene)

		Read<SceneMngr, std::set<Scene*>> toStartScenes;
		Read<SceneMngr, std::set<Scene*>> updatetingScenes;
		Read<SceneMngr, std::set<Scene*>> toStopScenes;

		// rendering
		Read<SceneMngr, Scene*> actived_scene{ nullptr };
		Read<SceneMngr, SObj*> main_camera_sobj{ nullptr };

		// toStartScenes
		bool Insert(Scene* scene);
		// updatetingScenes -> toStopScenes
		bool Erase(Scene* scene);
		// set actived_scene
		bool Active(Scene* scene, SObj* main_camera_sobj);

		void Update();
	};
}
