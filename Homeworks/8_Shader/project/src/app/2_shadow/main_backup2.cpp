#include <UGL/UGL>
#include <UGM/UGM>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../../tool/Camera.h"
#include "../../tool/SimpleLoader.h"

#include <iostream>

using namespace Ubpa;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
gl::Texture2D loadTexture(char const* path);
void renderScene(gl::Program& shader, const gl::VertexArray& va);
// settings
unsigned int scr_width = 800;
unsigned int scr_height = 600;
bool have_shadow = true;
bool have_rotate = false;
// camera
Camera camera(pointf3(0.0f, 2.0f, 5.0f));
float lastX = scr_width / 2.0f;
float lastY = scr_height / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float LastGLFTime = 0.0f;
float startTime = 0.0f;
float deltaGLFTime = 0.0f;

// keyboard
bool SpacePressState = false;
bool EnterPressState = false;

//plane
unsigned int planeVAO;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "HW8 - shadow", NULL, NULL);
    if (window == NULL)
    {
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    gl::Enable(gl::Capability::DepthTest);



    // build and compile our shader zprogram
    // ------------------------------------
    gl::Shader p3t2n3_vs(gl::ShaderType::VertexShader, "../data/shaders/p3t2n3.vert");
    gl::Shader p3_vs(gl::ShaderType::VertexShader, "../data/shaders/p3.vert");
    gl::Shader debug_depthmap_vs(gl::ShaderType::VertexShader, "../data/shaders/debug_depthmap.vert");

    gl::Shader light_shadow_fs(gl::ShaderType::FragmentShader, "../data/shaders/light_shadow.frag");
    gl::Shader empty_fs(gl::ShaderType::FragmentShader, "../data/shaders/empty.frag");
    gl::Shader debug_depthmap_fs(gl::ShaderType::FragmentShader, "../data/shaders/debug_depthmap.frag");

    gl::Program light_shadow_program(&p3t2n3_vs, &light_shadow_fs);
    gl::Program shadow_program(&p3_vs, &empty_fs);
    gl::Program debug_depthmap_program(&debug_depthmap_vs, &debug_depthmap_fs);

    rgbf ambient{0.2f, 0.2f, 0.2f};
    light_shadow_program.Use();
    light_shadow_program.SetTex("albedo_texture", 0);
    light_shadow_program.SetTex("shadowmap", 1);
    light_shadow_program.SetVecf3("point_light_radiance", { 100,100,100 });
    light_shadow_program.SetVecf3("ambient_irradiance", ambient);
    light_shadow_program.SetFloat("roughness", 0.5f );
    light_shadow_program.SetFloat("metalness", 0.f);

    debug_depthmap_program.Use();
    debug_depthmap_program.SetTex("depthmap", 0);

    // load model
    // ------------------------------------------------------------------
    auto spot = SimpleLoader::LoadObj("../data/models/spot_triangulated_good.obj");
    // Plane
    // ------------------------------------------------------------------
    float planeVertices[] = {
        // positions            // normals         // texcoords
        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,

        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
        25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f};
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
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,      // top-left
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,    // bottom-right
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,     // top-right
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,    // bottom-right
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,      // top-left
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,     // bottom-left
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


    // quad 
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

    // load and create a texture
    // -------------------------
    gl::Texture2D spot_albedo = loadTexture("../data/textures/spot_albedo.png");
    gl::Texture2D wood_texture = loadTexture("../data/textures/wood.png");
    // shadow buffer
    // -------------------------
    // TODO: create a framebuffer object
    const size_t SHADOW_TEXTURE_SIZE = 1024;
    gl::Texture2D shadowmap;
    //创建深度纹理
    shadowmap.SetImage(0, gl::PixelDataInternalFormat::DepthComponent, SHADOW_TEXTURE_SIZE, SHADOW_TEXTURE_SIZE,
        gl::PixelDataFormat::DepthComponent, gl::PixelDataType::Float, 0);
    //设置纹理参数
    //shadowmap.SetWrapFilter(gl::WrapMode::Repeat,
    //    gl::WrapMode::Repeat,
    //    gl::MinFilter::Nearest,
    //    gl::MagFilter::Nearest);
    shadowmap.SetWrapFilter(gl::WrapMode::ClampToBorder, 
        gl::WrapMode::ClampToBorder,
        gl::MinFilter::Nearest,
        gl::MagFilter::Nearest);
    rgbaf borderColor{ 1.f,1.f,1.f,1.f };
    gl::TexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor.data());
    //创建帧缓冲（深度缓冲）
    gl::FrameBuffer shadowFB;
    //将纹理附加到帧缓冲
    shadowFB.Attach(gl::FramebufferAttachment::DepthAttachment, &shadowmap);
    if (!shadowFB.IsComplete()) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        return -1;
    }
    
    //=========================================================
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //=====================================================
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
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

        // [ shadow ]
        //shadowFB.Bind();
        gl::ClearColor({ ambient, 1.0f });
        gl::Clear(gl::BufferSelectBit::ColorBufferBit|gl::BufferSelectBit::DepthBufferBit);
        
        
        // TODO: HW8 - 2_Shadow | generate shadow map
        // 1. set shadow_program's uniforms: model, view, projection, ...
        //   - projection: transformf::perspective(...)
        // 2. draw scene : spot->va->Draw(&shadow_program)
        //   - 10 spots
        //   - (optional) plane : receive shadow
        // -------
        // ref: https://learnopengl-cn.github.io/05%20Advanced%20Lighting/03%20Shadows/01%20Shadow%20Mapping/

        // ... (your codes)
        float angle;
        pointf3 lightPos;
        if (have_rotate) {
            angle = 20.0f + 10.f * ((float)glfwGetTime() - startTime + LastGLFTime);
            lightPos = pointf3(4 * cos(to_radian(angle)), 5.0f, 5 * sin(to_radian(angle)));
        }
        else
            lightPos = pointf3(-2.0f, 4.0f, -1.0f);

        float near_plane = 0.1f, far_plane = 100.0f;
        //transformf lightProjection = transformf::perspective(to_radian(45.0f), (float)SHADOW_TEXTURE_SIZE / (float)SHADOW_TEXTURE_SIZE, near_plane, far_plane);
        transformf lightProjection = transformf::orthographic(-10.0f, 10.0f,-10.0f,10.0f, 0.1f, 7.5f);
        transformf lightView = transformf::look_at(lightPos, pointf3(0.0f, 0.0f, 0.0f), vecf3(0.0f, 1.0f, 0.0f));
        transformf lightSpaceMatrix = lightProjection * lightView;
        //---------------------------------
        gl::Viewport({ 0,0 }, SHADOW_TEXTURE_SIZE, SHADOW_TEXTURE_SIZE);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        //---------------------------------
        shadow_program.Use();
        shadow_program.SetMatf4("light_space_matrix", lightSpaceMatrix);
        
        renderScene(shadow_program,cube);
        transformf model(pointf3(0.0f));
        shadow_program.SetMatf4("model", model);
        plane.Draw(&shadow_program);
        //model = transformf(pointf3{0, 0, 0}, quatf { vecf3(0.0f, 1.0f, 0.0f), to_radian(20.0f) });
        //shadow_program.SetMatf4("model", model);
        //spot->va->Draw(&shadow_program);
        //=================================

        // reset viewport
        gl::FrameBuffer::BindReset(); // default framebuffer
        gl::Viewport({ 0,0 }, scr_width, scr_height);
        gl::ClearColor({ ambient, 1.0f });
        gl::Clear(gl::BufferSelectBit::ColorBufferBit | gl::BufferSelectBit::DepthBufferBit);


        //// 场景添加阴影
        light_shadow_program.Use();
        light_shadow_program.SetVecf3("camera_pos", camera.Position);

        // bind textures on corresponding texture units
        // 绑定目标纹理
        light_shadow_program.Active(0, &wood_texture);

        // 绑定深度纹理
        //light_shadow_program.Active(1, &shadowmap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        // pass projection matrix to shader (note that in this case it could change every frame)
        transformf projection = transformf::perspective(to_radian(camera.Zoom), (float)scr_width / (float)scr_height, 0.1f, 100.f);
        light_shadow_program.SetMatf4("projection", projection);

        // camera/view transformation
        light_shadow_program.SetMatf4("view", camera.GetViewMatrix());

        // TODO: HW8 - 2_Shadow | set uniforms about shadow
        light_shadow_program.SetBool("have_shadow", have_shadow);
        // near plane, far plane, projection, ...

        light_shadow_program.SetVecf3("point_light_pos", lightPos);

        light_shadow_program.SetMatf4("light_space_matrix", lightSpaceMatrix);

        light_shadow_program.SetFloat("far_plane", far_plane);
        light_shadow_program.SetFloat("near_plane", near_plane);

        //renderScene(light_shadow_program, cube);
        //model = transformf(pointf3(0.0f));
        //light_shadow_program.SetMatf4("model", model);
        //plane.Draw(&light_shadow_program);
        
        //light_shadow_program.SetMatf4("model", model);
        //spot->va->Draw(&light_shadow_program);

        // Debug
        // --------------------------------------
        debug_depthmap_program.Use();
        //debug_depthmap_program.Active(0, &shadowmap);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        debug_depthmap_program.SetFloat("near_plane", near_plane);
        debug_depthmap_program.SetFloat("far_plane", far_plane);
        quad.Draw(&debug_depthmap_program);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    delete spot;
    // ------------------------------------------------------------------------
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
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
        have_shadow = !have_shadow;
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
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    gl::Viewport({ 0, 0 }, width, height);
    scr_width = width;
    scr_height = height;
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    camera.ProcessMouseMovement(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

gl::Texture2D loadTexture(char const* path)
{
    gl::Texture2D tex;
    tex.SetWrapFilter(gl::WrapMode::Repeat, gl::WrapMode::Repeat, gl::MinFilter::Linear, gl::MagFilter::Linear);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    gl::PixelDataFormat c2f[4] = {
        gl::PixelDataFormat::Red,
        gl::PixelDataFormat::Rg,
        gl::PixelDataFormat::Rgb,
        gl::PixelDataFormat::Rgba
    };
    gl::PixelDataInternalFormat c2if[4] = {
        gl::PixelDataInternalFormat::Red,
        gl::PixelDataInternalFormat::Rg,
        gl::PixelDataInternalFormat::Rgb,
        gl::PixelDataInternalFormat::Rgba
    };
    if (data)
    {
        tex.SetImage(0, c2if[nrChannels - 1], width, height, c2f[nrChannels - 1], gl::PixelDataType::UnsignedByte, data);
        tex.GenerateMipmap();
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return tex;
}

// renders the 3D scene
// --------------------
void renderScene(gl::Program& shader, const gl::VertexArray& va) {
    // floor
    transformf model;
    // cubes
    model = transformf({0.0f, 1.5f, 0.0f},scalef3(0.5f));
    shader.SetMatf4("model", model);
    va.Draw(&shader);
    model = transformf({2.0f, 0.0f, 1.0f},scalef3(0.5f));
    shader.SetMatf4("model", model);
    va.Draw(&shader);
    model = transformf({-1.0f, 0.0f, 2.0f}, scalef3(0.25f), quatf{vecf3{1.0, 0.0, 1.0}, to_radian(60.0f)});
    shader.SetMatf4("model", model);
    va.Draw(&shader);
}
