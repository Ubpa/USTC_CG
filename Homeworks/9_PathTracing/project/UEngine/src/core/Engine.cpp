#pragma once

#include <UEngine/Engine.h>

// Include glfw3.h after OpenGL definitions
#include <GLFW/glfw3.h>

#include "../_deps/imgui_impl/imgui_impl_opengl3.h"
#include "../_deps/imgui_impl/imgui_impl_glfw.h"

#include <UScene/UScene.h>

#include <URTR/DeferredRenderer.h>

using namespace Ubpa;
using namespace std;

Engine::Engine() {
    Scene::OnRegister();
}

bool Engine::Init(const std::string& title) {
    auto glfw_error_callback = [](int error, const char* description) {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    };

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return false;

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    window = glfwCreateWindow(1280, 720, title.c_str(), NULL, NULL);
    if (window == NULL)
        return false;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return false;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    rtr = new DeferredRenderer;

    return true;
}

void Engine::Loop() {
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto& io = ImGui::GetIO();
        gl::Viewport({ 0,0 },
            static_cast<GLsizei>(io.DisplaySize.x),
            static_cast<GLsizei>(io.DisplaySize.y));

        for (auto scene : SceneMngr::Instance().toStartScenes)
            scene->Start();

        if (SceneMngr::Instance().actived_scene)
            SceneMngr::Instance().actived_scene->Update();
        for (auto scene : SceneMngr::Instance().updatetingScenes) {
            if (scene == SceneMngr::Instance().actived_scene)
                continue;
            scene->Update();
        }
        for (auto scene : SceneMngr::Instance().toStartScenes) {
            if (scene == SceneMngr::Instance().actived_scene)
                continue;
            scene->Update();
        }
        for (auto scene : SceneMngr::Instance().toStopScenes)
            scene->Stop();
        SceneMngr::Instance().Update();

        if (SceneMngr::Instance().actived_scene
            && SceneMngr::Instance().main_camera_sobj
            && io.DisplaySize[0] != 0
            && io.DisplaySize[1] != 0)
        {
            rtr->Render(SceneMngr::Instance().actived_scene,
                SceneMngr::Instance().main_camera_sobj,
                static_cast<size_t>(io.DisplaySize[0]), static_cast<size_t>(io.DisplaySize[1])
            );
        }

        for (const auto& imguiCommand : imguiCommands)
            imguiCommand();
        imguiCommands.clear();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // final stop
    for (auto scene : SceneMngr::Instance().updatetingScenes)
        scene->Stop();
}

void Engine::CleanUp() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    window = nullptr;
}

void Engine::AddIMGUICommand(const std::function<void()>& command) {
    lock_guard<mutex> guard(imguiCommands_mutex);
    imguiCommands.push_back(command);
}
