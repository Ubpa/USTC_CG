#include <UEngine/Engine.h>
#include <UEngine/SceneMngr.h>

#include <UScene/UScene.h>

#include <stdio.h>

#include <string>

using namespace Ubpa;
using namespace std;

namespace Ubpa::detail::dynamic_reflection {
    void ReflRegister_Rotater();
    void ReflRegister_ImGUIExample();
}

struct Rotater : Component {
    static void OnRegister() {
        detail::dynamic_reflection::ReflRegister_Rotater();
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

    static void OnRegister() {
        detail::dynamic_reflection::ReflRegister_ImGUIExample();
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

int main(int, char**)
{
    Engine::Instance().Init("Ubpa@2020 UEngine - 00 basic");
    CmptRegistrar::Instance().Register<Rotater, ImGUIExample>();

    Scene scene("scene");

    auto [sobj0, camera] = scene.CreateSObj<Cmpt::Camera>("sobj0");
    auto [sobj1, geo1, mat1] = scene.CreateSObj<Cmpt::Geometry, Cmpt::Material>("sobj1");
    auto [sobj2, geo2, mat2] = scene.CreateSObj<Cmpt::Geometry, Cmpt::Material>("sobj2");
    auto [sobj3, geo3, mat3, r3] = scene.CreateSObj<Cmpt::Geometry, Cmpt::Material, Rotater>("sobj3");
    auto [sobj4, light4] = scene.CreateSObj<Cmpt::Light>("sobj4");
    auto [sobj5, env] = scene.CreateSObj<Cmpt::Light>("sobj5");
    auto [sobj6, light6, geo6, r6] = scene.CreateSObj<Cmpt::Light, Cmpt::Geometry, Rotater>("sobj6");
    auto [sobj7, imgui_example] = scene.CreateSObj<ImGUIExample>("sobj7");

    string albedo_path = "../data/textures/rusted_iron/albedo.png";
    string roughness_path = "../data/textures/rusted_iron/roughness.png";
    string metalness_path = "../data/textures/rusted_iron/metallic.png";
    string normal_path = "../data/textures/rusted_iron/normal.png";
    string env_path = "../data/textures/newport_loft.hdr";
    auto albedo_texture = new Texture2D(albedo_path);
    auto roughness_texture = new Texture2D(roughness_path);
    auto metalness_texture = new Texture2D(metalness_path);
    auto normals_texture = new Texture2D(normal_path);
    auto env_texture = new Texture2D(env_path);

    geo1->SetPrimitive(new Sphere);
    geo2->SetPrimitive(new Square);
    geo3->SetPrimitive(new TriMesh(TriMesh::Type::Cube));
    auto brdf1 = new stdBRDF;
    auto brdf2 = new stdBRDF;
    auto brdf3 = new stdBRDF;
    brdf1->albedo_texture = albedo_texture;
    brdf1->roughness_texture = roughness_texture;
    brdf1->metalness_texture = metalness_texture;
    brdf1->normal_map = normals_texture;
    brdf2->albedo_texture = albedo_texture;
    brdf2->roughness_texture = roughness_texture;
    brdf2->metalness_texture = metalness_texture;
    brdf2->normal_map = normals_texture;
    brdf3->albedo_texture = albedo_texture;
    brdf3->roughness_texture = roughness_texture;
    brdf3->metalness_texture = metalness_texture;
    brdf3->normal_map = normals_texture;
    mat1->SetMaterial(brdf1);
    mat2->SetMaterial(brdf2);
    mat3->SetMaterial(brdf3);

    sobj0->Get<Cmpt::Position>()->value = { 0,0,8 };
    sobj1->Get<Cmpt::Position>()->value = { -4,0,0 };
    sobj1->Get<Cmpt::Scale>()->value = 2.f;
    sobj2->Get<Cmpt::Rotation>()->value = { vecf3{1,0,0}, to_radian(45.f) };
    sobj3->Get<Cmpt::Position>()->value = { 4,0,0 };
    sobj3->Get<Cmpt::Scale>()->value = { 1,2,1 };
    sobj3->Get<Cmpt::Rotation>()->value = { vecf3{1,2,1}.normalize(), to_radian(45.f) };
    camera->fov = to_radian(60.f);
    camera->ar = 1280 / static_cast<float>(720);

    light4->SetLight(new PointLight{ 100.f,{0.9f,0.9f,1.f} });
    sobj4->Get<Cmpt::Position>()->value = { 0,4,0 };

    light6->SetLight(new AreaLight{ 100.f, {1,0,1} });
    geo6->SetPrimitive(new Square);
    sobj6->Get<Cmpt::Position>()->value = { 0,3,0 };
    sobj6->Get<Cmpt::Rotation>()->value = quatf{ vecf3{1,0,0}, to_radian(180.f) } *sobj6->Get<Cmpt::Rotation>()->value;

    env->SetLight(new EnvLight(1.f, rgbf{ 1.f }, env_texture));

    SerializerJSON serializer;
    auto rst = serializer.Serialize(&scene);
    cout << rst << endl;

    SceneMngr::Instance().Active(&scene, sobj0);

    // Main loop
    Engine::Instance().Loop();

    // Cleanup
    Engine::Instance().CleanUp();

    return 0;
}

namespace Ubpa::detail::dynamic_reflection {
    void ReflRegister_Rotater() {
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
    void ReflRegister_ImGUIExample() {
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
