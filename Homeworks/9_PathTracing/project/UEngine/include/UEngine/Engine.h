#pragma once


#include "SceneMngr.h"
#include "_deps/imgui/imgui.h"

#define UBPA_UGL_GLAD 1
#include <UGL/UGL.h>

struct GLFWwindow;

#include <functional>
#include <mutex>

namespace Ubpa {
	class DeferredRenderer;

	class Engine {
	public:
		static Engine& Instance() {
			static Engine instance;
			return instance;
		}

		bool Init(const std::string& title);

		// life cycle
		// 1. start SceneMngr::toStartScenes
		// 2. update SceneMngr::actived_scene
		// 3. update SceneMngr::updatetingScenes (exclude actived_scene)
		// 4. update SceneMngr::toStartScenes (exclude actived_scene)
		// 5. stop SceneMngr::toStopScenes
		// 6. SceneMngr::Update
		// - insert scene of toStartScenes to updatetingScenes
		// - clear toStartScenes
		// - erase scene of toStopScenes from updatetingScenes
		// - clear toStopScenes (may reset actived_scene)
		// 7. Render SceneMngr::actived_scene
		// 8. Run IMGUICommand
		// 9. Render IMGUI
		void Loop();

		void CleanUp();

		void AddIMGUICommand(const std::function<void()>& command);

	private:
		Engine();

		std::vector<std::function<void()>> imguiCommands;
		std::mutex imguiCommands_mutex;

		DeferredRenderer* rtr{ nullptr };
		GLFWwindow* window{ nullptr };
	};
}
