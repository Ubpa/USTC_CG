#include <UEngine/Engine.h>
#include <UEngine/SceneMngr.h>

#include <UScene/UScene.h>

#include <stdio.h>

#include <string>
#include <fstream>

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

int main(int argn, char** args)
{
    if (argn != 2) {
        cout << "usage:" << args[0] << " <file-path>" << endl;
        return 1;
    }
    cout << args[0] << endl << args[1] << endl;
    
    Engine::Instance().Init("Ubpa@2020 UEngine - 01 uscene");
    CmptRegistrar::Instance().Register<Rotater, ImGUIExample>();
    ifstream usceneFile(args[1], ios::in | ios::ate);
    if (!usceneFile.is_open()) {
        cout << "ERROR:" << endl
            << "\t" << "open file (" << args[1] << ") fail" << endl;
        return 1;
    }

    size_t size = usceneFile.tellg();
    usceneFile.seekg(0, ios::beg);
    char* buffer = new char[size + 1];
    buffer[size] = 0;
    usceneFile.read(buffer, size);
    usceneFile.close();

    DeserializerJSON deserializer;
    auto scene = deserializer.DeserializeScene(buffer);
    delete[] buffer;
    if (!scene)
        return 1;

    SceneMngr::Instance().Active(scene, scene->root->GetSObjInTreeWith<Cmpt::Camera>());

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
