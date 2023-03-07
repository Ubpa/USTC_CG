#include "GenScene.h"
#include "Editor.h"

#include <UEngine/Engine.h>
#include <UScene/core.h>
#include <UEngine/SceneMngr.h>

using namespace Ubpa;
using namespace std;

int main(int argc, char** argv)
{
    Engine::Instance().Init("Ubpa@2020 UEditor v0.0.7");
    Editor::OnRegister();

    const char* path = argc == 2 ? argv[1] : "../data/models/uscene.json";
    Scene* scene = SceneGenerator::Instance().GenScene(path);
    //Scene* scene = SceneGenerator::Instance().GenScene(0);

    Editor::Instance().SetCamera(scene->root->GetSObjInTreeWith<Cmpt::Camera>());

    SceneMngr::Instance().Insert(scene);
    SceneMngr::Instance().Insert(Editor::Instance().scene);

    SceneMngr::Instance().Active(scene, Editor::Instance().scene->root->GetSObjInTreeWith<Cmpt::Camera>());

    Engine::Instance().Loop();

    Engine::Instance().CleanUp();

    SceneGenerator::Instance().PrintSerializedScene(scene);

    delete scene;

    return 0;
}
