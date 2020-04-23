#include "GenScene.h"
#include "Editor.h"

#include <UEngine/Engine.h>
#include <UScene/core.h>
#include <UEngine/SceneMngr.h>

using namespace Ubpa;
using namespace std;

int main(int, char**)
{
    Engine::Instance().Init("Ubpa@2020 UEditor v0.0.2");
    Editor::OnRegist();

    auto scene = GenScene(0);

    Editor::Instance().SetCamera(scene->root->GetSObjInTreeWith<Cmpt::Camera>());

    SceneMngr::Instance().Insert(scene);
    SceneMngr::Instance().Insert(Editor::Instance().scene);

    SceneMngr::Instance().Active(scene, Editor::Instance().scene->root->GetSObjInTreeWith<Cmpt::Camera>());

    Engine::Instance().Loop();

    Engine::Instance().CleanUp();

    delete scene;

    return 0;
}
