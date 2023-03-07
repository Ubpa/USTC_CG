
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/model.h>
#include <learnopengl/shader.h>
#include "../../tool/Camera.h"

#include "../../tool/SimpleLoader.h"


#include <iostream>
using namespace Ubpa;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
gl::Texture2D loadTexture(const char* path, gl::WrapMode wrapmode = gl::WrapMode::Repeat);
void renderScene(gl::Program& shader,const std::vector<gl::VertexArray*> va_list);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(pointf3(0.0f, 2.0f, 5.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;  // time between current frame and last frame
float lastFrame = 0.0f;

float LastGLFTime = 0.0f;
float startTime = 0.0f;
float deltaGLFTime = 0.0f;

// Control
bool shadows = true;
bool have_rotate = false;

// keyboard
bool SpacePressState = false;
bool EnterPressState = false;

//Texture
std::vector<gl::Texture2D*> textures_list;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hw8_3_Shadow", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    // gl::Shader shadowmap_vs(gl::ShaderType::VertexShader ,"../data/shaders/p3t2n3.vert");
    // gl::Shader shadowmap_fs(gl::ShaderType::FragmentShader, "../data/shaders/light_shadow.frag");
    gl::Shader shadowmap_vs(gl::ShaderType::VertexShader, "../data/shaders/shadow_mapping.vert");
    gl::Shader shadowmap_fs(gl::ShaderType::FragmentShader, "../data/shaders/shadow_mapping.frag");
    gl::Shader depthmap_vs(gl::ShaderType::VertexShader, "../data/shaders/depthmap.vs");
    gl::Shader depthmap_fs(gl::ShaderType::FragmentShader, "../data/shaders/depthmap.fs");
    gl::Shader debugDepthQuad_vs(gl::ShaderType::VertexShader, "../data/shaders/debug_depthmap.vs");
    gl::Shader debugDepthQuad_fs(gl::ShaderType::FragmentShader, "../data/shaders/debug_depthmap.fs");

    gl::Program shader_program(&shadowmap_vs, &shadowmap_fs);
    gl::Program depthmap_program(&depthmap_vs, &depthmap_fs);
    gl::Program debugDepthQuad_program(&debugDepthQuad_vs, &debugDepthQuad_fs);
    float r = (float)135 / 255;
    float g = (float)206 / 255;
    float b = (float)235 / 255;
    rgbf ambient{r,g,b};
    shader_program.Use();
    shader_program.SetTex("shadowmap", 0);
    shader_program.SetTex("diffuseTexture", 1);
    shader_program.SetVecf3("point_light_radiance", {100, 100, 100});
    shader_program.SetVecf3("ambient_irradiance", ambient);
    shader_program.SetFloat("roughness", 0.5f);
    shader_program.SetFloat("metalness", 0.f);

    debugDepthQuad_program.Use();
    debugDepthQuad_program.SetTex("depthmap", 0);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<gl::VertexArray*> va_list;

    // spot
    auto spot = SimpleLoader::LoadObj("../data/models/spot_triangulated_good.obj");
    va_list.push_back(spot->va);
    // plane
    // ------------------------------------------------------------------
    float planeVertices[] = {
        // positions            // normals         // texcoords
        1.0f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.8f, 0.0f,
        -1.0f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.2f, 0.0f,
        -1.0f, -0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 0.2f, 0.6f,

        1.0f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.8f, 0.0f,
        -1.0f, -0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 0.2f, 0.6f,
        1.0f, -0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 0.8f, 0.6f };
    std::vector<pointf3> positions;
    std::vector<normalf> normals;
    std::vector<pointf2> texcoords;
    std::vector<unsigned> indices;

    for (size_t i = 0; i < 6; i++) {
        positions.push_back(pointf3{planeVertices[i * 8], planeVertices[i * 8 + 1], planeVertices[i * 8 + 2]});
        normals.push_back(normalf{planeVertices[i * 8 + 3], planeVertices[i * 8 + 4], planeVertices[i * 8 + 5]});
        texcoords.push_back(pointf2{planeVertices[i * 8 + 6], planeVertices[i * 8 + 7]});
        indices.push_back(i);
    }

    gl::VertexBuffer plane_pos(positions.size() * sizeof(pointf3), positions[0].data());
    gl::VertexBuffer plane_uv(texcoords.size() * sizeof(pointf2), texcoords[0].data());
    gl::VertexBuffer plane_n(normals.size() * sizeof(normalf), normals[0].data());
    gl::ElementBuffer plane_eb(gl::BasicPrimitiveType::Triangles, indices.size() / 3, indices.data());

    gl::VertexArray::Format plane_format;
    plane_format.attrptrs.push_back(plane_pos.AttrPtr(3, gl::DataType::Float, false, sizeof(pointf3)));
    plane_format.attrptrs.push_back(plane_uv.AttrPtr(2, gl::DataType::Float, false, sizeof(pointf2)));
    plane_format.attrptrs.push_back(plane_n.AttrPtr(3, gl::DataType::Float, false, sizeof(normalf)));

    plane_format.eb = &plane_eb;

    gl::VertexArray plane({0, 1, 2}, plane_format);
    va_list.push_back(&plane);
    // cube
    // ------------------------------------------------------------------
    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    // top-right
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,   // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    // top-right
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,   // top-left
        // front face
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,   // bottom-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // top-right
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // top-left
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
        // left face
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-right
        -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-right
        // right face
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-left
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-right
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-left
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
        // bottom face
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // top-right
        1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,   // top-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,    // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,    // bottom-left
        -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // top-right
        // top face
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,    // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,    // bottom-right
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f    // bottom-left
    };
    positions.clear();
    texcoords.clear();
    normals.clear();
    indices.clear();
    for (size_t i = 0; i < sizeof(vertices) / sizeof(float); i += 8) {
        positions.push_back(pointf3(vertices[i], vertices[i + 1], vertices[i + 2]));
        normals.push_back(normalf(vertices[i + 3], vertices[i + 4], vertices[i + 5]));
        texcoords.push_back(pointf2(vertices[i + 6], vertices[i + 7]));
    }
    for (size_t i = 0; i < positions.size(); i++) {
        indices.push_back(i);
    }
    gl::VertexBuffer cub_pos(positions.size() * sizeof(pointf3), positions[0].data());
    gl::VertexBuffer cub_uv(positions.size() * sizeof(pointf2), texcoords[0].data());
    gl::VertexBuffer cub_n(positions.size() * sizeof(normalf), normals[0].data());
    gl::ElementBuffer cub_eb(gl::BasicPrimitiveType::Triangles, indices.size() / 3, indices.data());

    gl::VertexArray::Format cub_format;
    cub_format.attrptrs.push_back(cub_pos.AttrPtr(3, gl::DataType::Float, false, sizeof(pointf3)));
    cub_format.attrptrs.push_back(cub_uv.AttrPtr(2, gl::DataType::Float, false, sizeof(pointf2)));
    cub_format.attrptrs.push_back(cub_n.AttrPtr(3, gl::DataType::Float, false, sizeof(normalf)));
    cub_format.eb = &cub_eb;

    gl::VertexArray cube({0, 1, 2}, cub_format);
    va_list.push_back(&cube);

    float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    positions.clear();
    texcoords.clear();
    indices.clear();
    for (size_t i = 0; i < sizeof(quadVertices) / sizeof(float); i += 5) {
        positions.push_back(pointf3(quadVertices[i], quadVertices[i + 1], quadVertices[i + 2]));
        texcoords.push_back(pointf2(quadVertices[i + 3], quadVertices[i + 4]));
    }
    size_t two_tri[6] = {
        0, 1, 2,
        1, 3, 2};
    for (auto i : two_tri)
        indices.push_back(i);

    gl::VertexBuffer quad_pos(positions.size() * sizeof(pointf3), positions[0].data());
    gl::VertexBuffer quad_uv(positions.size() * sizeof(pointf2), texcoords[0].data());
    gl::ElementBuffer quad_eb(gl::BasicPrimitiveType::Triangles, indices.size() / 3, indices.data());

    gl::VertexArray::Format quad_format;
    quad_format.attrptrs.push_back(quad_pos.AttrPtr(3, gl::DataType::Float, false, sizeof(pointf3)));
    quad_format.attrptrs.push_back(quad_uv.AttrPtr(2, gl::DataType::Float, false, sizeof(pointf2)));
    quad_format.eb = &quad_eb;

    gl::VertexArray quad({0, 1}, quad_format);


    // load textures
    // -------------
    gl::Texture2D spot_albedo, wood_texture, grass_texture;
    spot_albedo = loadTexture("../data/textures/spot_albedo.png");
    wood_texture = loadTexture("../data/textures/wood.png");
    grass_texture = loadTexture("../data/textures/grass.png");
    textures_list.push_back(&spot_albedo);
    textures_list.push_back(&wood_texture);
    textures_list.push_back(&grass_texture);
    //==================================================================================================================
    const size_t SHADOW_TEXTURE_SIZE = 1024;
    gl::Texture2D shadowmap;
    //创建深度纹理
    shadowmap.SetImage(0, gl::PixelDataInternalFormat::DepthComponent, SHADOW_TEXTURE_SIZE, SHADOW_TEXTURE_SIZE,
    gl::PixelDataFormat::DepthComponent, gl::PixelDataType::Float, 0);
    //设置纹理参数
    shadowmap.SetWrapFilter(gl::WrapMode::ClampToBorder, gl::WrapMode::ClampToBorder,
    gl::MinFilter::Nearest, gl::MagFilter::Nearest);
    rgbaf border{ 1.f,1.f,1.f,1.f };
    gl::TexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border.data());
    //创建帧缓冲（深度缓冲）
    gl::FrameBuffer shadowFB;
    //将纹理附加到帧缓冲
    shadowFB.Attach(gl::FramebufferAttachment::DepthAttachment, &shadowmap);

    //==================================================================================================================
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------

        gl::ClearColor({ambient, 1.0f});
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        transformf lightProjection, lightView;
        transformf lightSpaceMatrix;

        float angle;
        if (have_rotate)
            angle = 20.0f + 10.f * ((float)glfwGetTime() - startTime + LastGLFTime);
        else
            angle = 20.0f + 10.f * (LastGLFTime);
        pointf3 lightPos(3 * cos(to_radian(angle)), 5.0f, 3 * sin(to_radian(angle)));

        float near_plane = 1.f, far_plane = 100.0f;
        //lightProjection = glm::perspective(glm::radians(45.0f), static_cast<float>(SHADOW_WIDTH) / static_cast<float>(SHADOW_HEIGHT), near_plane, far_plane);
        lightProjection = transformf::orthographic(-15.0f, 15.0f, -15.0f, 15.0f, 0.1f, 10.0f);
        lightView = transformf::look_at(lightPos, pointf3(0.0f, 0.1f, 0.0f), vecf3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        depthmap_program.Use();
        depthmap_program.SetMatf4("light_space_matrix", lightSpaceMatrix);

        // glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        // glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        // glClear(GL_DEPTH_BUFFER_BIT);
        shadowFB.Bind();
        // glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        gl::Viewport({0, 0}, SHADOW_TEXTURE_SIZE, SHADOW_TEXTURE_SIZE);
        gl::Clear(gl::BufferSelectBit::DepthBufferBit);

        renderScene(depthmap_program,va_list);
        //=================================
        // // reset viewport
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gl::FrameBuffer::BindReset();  // default framebuffer
        gl::Viewport({0, 0}, SCR_WIDTH, SCR_WIDTH);
        gl::ClearColor({ambient, 1.0f});
        gl::Clear(gl::BufferSelectBit::ColorBufferBit | gl::BufferSelectBit::DepthBufferBit);

        // 2. render scene as normal using the generated depth/shadow map
        // --------------------------------------------------------------
        //// 场景添加阴影
        shader_program.Use();
        shader_program.SetVecf3("camera_pos", camera.Position);

        // bind textures on corresponding texture units
        // 绑定目标纹理
        //shader_program.Active(0, &spot_albedo);

        // 绑定深度纹理
        shader_program.Active(0, &shadowmap);

        // pass projection matrix to shader (note that in this case it could change every frame)
        transformf projection = transformf::perspective(to_radian(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);
        shader_program.SetMatf4("projection", projection);

        // camera/view transformation
        shader_program.SetMatf4("view", camera.GetViewMatrix());

        // TODO: HW8 - 2_Shadow | set uniforms about shadow
        shader_program.SetBool("have_shadow", shadows);
        // near plane, far plane, projection, ...

        shader_program.SetVecf3("point_light_pos", lightPos);

        shader_program.SetMatf4("light_space_matrix", lightSpaceMatrix);

        shader_program.SetFloat("far_plane", far_plane);
        shader_program.SetFloat("near_plane", near_plane);

        renderScene(shader_program, va_list);

        // render Depth map to quad for visual debugging
        // ---------------------------------------------
        // debugDepthQuad.use();
        // debugDepthQuad.setFloat("near_plane", near_plane);
        // debugDepthQuad.setFloat("far_plane", far_plane);
        // glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, depthMap);
        //renderQuad();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    glfwTerminate();
    return 0;
}

// renders the 3D scene
// --------------------
void renderScene(gl::Program& shader, const std::vector<gl::VertexArray*> va_list) {
    transformf model;
    gl::VertexArray* spot = va_list[0];
    gl::VertexArray* plane = va_list[1];
    gl::VertexArray* cube = va_list[2];
    gl::Texture2D* spot_albedo = textures_list[0];
    gl::Texture2D* wood_texture = textures_list[1];
    gl::Texture2D* grass_texture = textures_list[2];
    // floor
    shader.Active(1, grass_texture);
    for (int i = -25; i <= 25; i++) {
        for(int j = -25; j <= 25; j++) {
            model = transformf(pointf3{i * 2.0f, 0.0f, j * 2.0f});
            shader.SetMatf4("model", model);
            plane->Draw(&shader);
        }
    }
    // model = transformf(pointf3(0.0f));
    // shader.SetMatf4("model", model);
    // plane->Draw(&shader);
    // cubes
    shader.Active(1, wood_texture);
    model = transformf({0.0f, 1.5f, 0.0f}, scalef3(0.5f));
    shader.SetMatf4("model", model);
    cube->Draw(&shader);
    model = transformf({2.0f, 0.0f, 1.0f}, scalef3(0.5f));
    shader.SetMatf4("model", model);
    cube->Draw(&shader);
    model = transformf({-1.0f, 0.0f, 2.0f}, scalef3(0.25f), quatf{vecf3{1.0, 0.0, 1.0}, to_radian(60.0f)});
    shader.SetMatf4("model", model);
    cube->Draw(&shader);
    // spot
    shader.Active(1, spot_albedo);
    model = transformf{pointf3{-2.0f, 0.1f, 3.0}};
    shader.SetMatf4("model", model);
    spot->Draw(&shader);
}


// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::DOWN, deltaTime);

    if ((glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) && !EnterPressState) {
        EnterPressState = true;
        shadows = !shadows;
    } else if ((glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) && EnterPressState) {
        EnterPressState = false;
    }

    if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && !SpacePressState) {
        SpacePressState = true;
        if (have_rotate) {
            LastGLFTime = (float)glfwGetTime() - startTime + LastGLFTime;
        } else {
            startTime = (float)glfwGetTime();
        }
        have_rotate = !have_rotate;
    } else if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) && SpacePressState) {
        SpacePressState = false;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;  // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
gl::Texture2D loadTexture(char const* path, gl::WrapMode wrapmode) {
    gl::Texture2D tex;
    tex.SetWrapFilter(gl::WrapMode::Repeat, gl::WrapMode::Repeat, gl::MinFilter::Linear, gl::MagFilter::Linear);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    gl::PixelDataFormat c2f[4] = {
        gl::PixelDataFormat::Red,
        gl::PixelDataFormat::Rg,
        gl::PixelDataFormat::Rgb,
        gl::PixelDataFormat::Rgba};
    gl::PixelDataInternalFormat c2if[4] = {
        gl::PixelDataInternalFormat::Red,
        gl::PixelDataInternalFormat::Rg,
        gl::PixelDataInternalFormat::Rgb,
        gl::PixelDataInternalFormat::Rgba};
    if (data) {
        tex.SetImage(0, c2if[nrChannels - 1], width, height, c2f[nrChannels - 1], gl::PixelDataType::UnsignedByte, data);
        tex.GenerateMipmap();
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return tex;
}
