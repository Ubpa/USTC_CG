#include <UEngine/SceneMngr.h>

#include <UScene/core/Scene.h>

using namespace Ubpa;

bool SceneMngr::Insert(Scene* scene) {
	if (updatetingScenes->find(scene) != updatetingScenes->end()
		|| toStopScenes->find(scene) != toStopScenes->end())
		return false;

	toStartScenes->insert(scene);
	return true;
}

bool SceneMngr::Erase(Scene* scene) {
	if (updatetingScenes->find(scene) == updatetingScenes->end())
		return false;

	toStopScenes->insert(scene);
	return true;
}

bool SceneMngr::Active(Scene* scene, SObj* main_camera_sobj) {
	if (toStopScenes->find(scene) != toStopScenes->end())
		return false;
	if (updatetingScenes->find(scene) == updatetingScenes->end()
		&& toStartScenes->find(scene) == toStartScenes->end())
		Insert(scene);
	actived_scene.val = scene;
	this->main_camera_sobj.val = main_camera_sobj;
	return true;
}

void SceneMngr::Update() {
	for (auto scene : toStartScenes)
		updatetingScenes->insert(scene);
	toStartScenes->clear();

	for (auto scene : toStopScenes) {
		updatetingScenes->erase(scene);
		if (actived_scene == scene) {
			actived_scene.val = nullptr;
			main_camera_sobj.val = nullptr;
		}
	}
	toStopScenes->clear();
}
