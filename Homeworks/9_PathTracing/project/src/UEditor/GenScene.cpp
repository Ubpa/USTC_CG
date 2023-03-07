#include "GenScene.h"

#include <UEngine/Engine.h>
#include <_deps/imgui/imgui.h>

#include <UScene/core.h>
#include <UScene/tool.h>

#include "Cmpt/PathTracerAgency.h"

#include <fstream>
#include <sstream>
#include <string>

using namespace Ubpa;
using namespace std;


namespace Ubpa::detail::dynamic_reflection {
    void ReflRegist_Rotater();
    void ReflRegist_ImGUIExample();
}

struct Rotater : Component {
    static void OnRegist() {
        detail::dynamic_reflection::ReflRegist_Rotater();
    }

    void OnUpdate(Cmpt::Rotation* rot) const {
        rot->value = quatf{ vecf3{1.f}, to_radian(1.f) } *rot->value;
    }
};

class ImGUIExample : Component {
public:
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    static void OnRegist() {
        detail::dynamic_reflection::ReflRegist_ImGUIExample();
    }
    void OnUpdate() {
        Engine::Instance().AddIMGUICommand([this]() {
            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // 3. Show another simple window.
            if (show_another_window)
            {
                ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                    show_another_window = false;
                ImGui::End();
            }
            });
    }
};

namespace Ubpa::detail::dynamic_reflection {
    void ReflRegist_Rotater() {
        Reflection<Rotater>::Instance() // name : struct ::Rotater
            ;
        if constexpr (std::is_base_of_v<Component, Rotater>) {
            Reflection<Rotater>::Instance().RegisterConstructor([](SObj* sobj) {
                if constexpr (std::is_base_of_v<Component, Rotater>) {
                    if constexpr (Ubpa::detail::SObj_::IsNecessaryCmpt<Rotater>)
                        return sobj->Get<Rotater>();
                    else
                        return sobj->GetOrAttach<Rotater>();
                };
                });
        }
    }
}
namespace Ubpa::detail::dynamic_reflection {
    void ReflRegist_ImGUIExample() {
        Reflection<ImGUIExample>::Instance() // name : class ::ImGUIExample
            .Register(&ImGUIExample::show_demo_window, "show_demo_window") //  bool
            .Register(&ImGUIExample::show_another_window, "show_another_window") //  bool
            ;
        if constexpr (std::is_base_of_v<Component, ImGUIExample>) {
            Reflection<ImGUIExample>::Instance().RegisterConstructor([](SObj* sobj) {
                if constexpr (std::is_base_of_v<Component, ImGUIExample>) {
                    if constexpr (Ubpa::detail::SObj_::IsNecessaryCmpt<ImGUIExample>)
                        return sobj->Get<ImGUIExample>();
                    else
                        return sobj->GetOrAttach<ImGUIExample>();
                };
                });
        }
    }
}

SceneGenerator::SceneGenerator() {
    CmptRegistrar::Instance().Register<Rotater, ImGUIExample, Cmpt::PathTracerAgency>();
}

void SceneGenerator::PrintSerializedScene(Scene* scene) {
    SerializerJSON serializer;
    cout << serializer.Serialize(scene) << endl;
}

Scene* SceneGenerator::GenScene(const string& path) {
    std::ifstream t(path);
    if (!t.is_open())
        return nullptr;
    std::stringstream buffer;
    buffer << t.rdbuf();

    auto data = buffer.str();

    DeserializerJSON deserializer;
    return deserializer.DeserializeScene(data);
}

namespace Ubpa::detail::GenScene_ {
	Scene* GenScene_0() {
        auto scene = new Scene("scene");

        auto [camera_obj, camera, agency] = scene->CreateSObj<Cmpt::Camera, Cmpt::PathTracerAgency>("camera_obj");
        auto [cornellbox] = scene->CreateSObj<>("cornellbox");
        auto [wall_left, geo_wall_left, mat_wall_left] = scene->CreateSObj<Cmpt::Geometry, Cmpt::Material>("wall_left", cornellbox);
        auto [wall_right, geo_wall_right, mat_wall_right] = scene->CreateSObj<Cmpt::Geometry, Cmpt::Material>("wall_right", cornellbox);
        auto [wall_up, geo_wall_up, mat_wall_up] = scene->CreateSObj<Cmpt::Geometry, Cmpt::Material>("wall_up", cornellbox);
        auto [wall_down, geo_wall_down, mat_wall_down] = scene->CreateSObj<Cmpt::Geometry, Cmpt::Material>("wall_down", cornellbox);
        auto [wall_back, geo_wall_back, mat_wall_back] = scene->CreateSObj<Cmpt::Geometry, Cmpt::Material>("wall_back", cornellbox);
        auto [rectlight_obj, geo_rectlight, rectlight] = scene->CreateSObj<Cmpt::Geometry, Cmpt::Light>("rectlight");
        // scene->CreateSObj<ImGUIExample>("imguiExample");
        auto [env_obj, env_light] = scene->CreateSObj<Cmpt::Light>("env light");
        auto env_texture = new Texture2D{ "../data/textures/newport_loft.hdr" };
        env_texture->inv_v = true;
        env_light->SetLight(new EnvLight(1.f, rgbf{ 1.f }, env_texture));


        camera_obj->Get<Cmpt::Position>()->value = { 0,0,0 };

        // wall
        geo_wall_left->SetPrimitive(new Square);
        geo_wall_right->SetPrimitive(new Square);
        geo_wall_up->SetPrimitive(new Square);
        geo_wall_down->SetPrimitive(new Square);
        geo_wall_back->SetPrimitive(new Square);

        mat_wall_left->SetMaterial(new stdBRDF{ rgbf{0.8,0.2,0.2} });
        mat_wall_right->SetMaterial(new stdBRDF{ rgbf{0.2,0.2,0.8} });
        mat_wall_up->SetMaterial(new stdBRDF{ rgbf{0.8f} });
        mat_wall_down->SetMaterial(new stdBRDF{ rgbf{0.8f} });
        mat_wall_back->SetMaterial(new stdBRDF{ rgbf{0.8f} });

        wall_left->Get<Cmpt::Position>()->value = { -1,0,0 };
        wall_left->Get<Cmpt::Rotation>()->value = { vecf3{0,0,1},to_radian(-90.f) };

        wall_right->Get<Cmpt::Position>()->value = { 1,0,0 };
        wall_right->Get<Cmpt::Rotation>()->value = { vecf3{0,0,1},to_radian(90.f) };

        wall_up->Get<Cmpt::Position>()->value = { 0,1,0 };
        wall_up->Get<Cmpt::Rotation>()->value = { vecf3{0,0,1},to_radian(180.f) };

        wall_down->Get<Cmpt::Position>()->value = { 0,-1,0 };

        wall_back->Get<Cmpt::Position>()->value = { 0,0,-1 };
        wall_back->Get<Cmpt::Rotation>()->value = { vecf3{1,0,0},to_radian(90.f) };

        cornellbox->Get<Cmpt::Position>()->value = { 0,0,-6 };
        cornellbox->Get<Cmpt::Scale>()->value = { 2,2,2 };

        auto [metal_ball, metal_ball_geo, metal_ball_mat] = scene->CreateSObj<Cmpt::Geometry, Cmpt::Material>("metal_ball", cornellbox);
        metal_ball_geo->SetPrimitive(new Sphere);
        metal_ball->Get<Cmpt::Scale>()->value = 0.4f;
        metal_ball->Get<Cmpt::Position>()->value = { -0.5f,-0.6f,-0.2f };
        auto metal = new stdBRDF;
        metal->albedo_factor = { 1.f, 0.8f, 0.2f };
        metal->roughness_factor = 0.1f;
        metal_ball_mat->SetMaterial(metal);

        auto [plastic_ball, plastic_ball_geo, plastic_ball_mat] = scene->CreateSObj<Cmpt::Geometry, Cmpt::Material>("plastic_ball", cornellbox);
        plastic_ball_geo->SetPrimitive(new Sphere);
        plastic_ball->Get<Cmpt::Scale>()->value = 0.4f;
        plastic_ball->Get<Cmpt::Position>()->value = { 0.5f,-0.6f,0.2f };
        auto plastic = new stdBRDF;
        plastic->albedo_factor = { 0.9f, 0.9f, 0.9f };
        plastic->roughness_factor = 0.6f;
        plastic->metalness_factor = 0.f;
        plastic_ball_mat->SetMaterial(plastic);

        rectlight->SetLight(new AreaLight{ 50.f, {1,1,1} });
        geo_rectlight->SetPrimitive(new Square);
        rectlight_obj->Get<Cmpt::Position>()->value = { 0,1.9f,-6 };
        rectlight_obj->Get<Cmpt::Scale>()->value = { 0.5f,0.5f,0.5f };
        rectlight_obj->Get<Cmpt::Rotation>()->value = quatf{ vecf3{1,0,0}, to_radian(180.f) };

        auto [cube, geo_cube, mat_cube] = scene->CreateSObj<Cmpt::Geometry, Cmpt::Material>("cube", cornellbox);
        auto cube_BRDF = new stdBRDF;
        cube_BRDF->albedo_factor = { 0.1f, 0.63f, 0.84f };
        cube_BRDF->roughness_factor = 0.5f;
        mat_cube->SetMaterial(cube_BRDF);
        geo_cube->SetPrimitive(new TriMesh("../data/models/cube.obj"));
        cube->Get<Cmpt::Scale>()->value = { 0.15f,0.1f,0.15f };
        cube->Get<Cmpt::Position>()->value = { -0.2f,-0.9f,0.65f };

        SceneGenerator::Instance().PrintSerializedScene(scene);

        return scene;
	}
}

Scene* SceneGenerator::GenScene(size_t n) {
    assert(n < 1);

	using Func = Scene *();
	Func* funcs[] = {
		&detail::GenScene_::GenScene_0
	};

	return funcs[n]();
}
