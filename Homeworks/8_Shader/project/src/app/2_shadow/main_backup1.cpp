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
void renderScene(const gl::Program& shader);
void renderCube(const gl::Program& shader);
void renderQuad(const gl::Program& shader);
// settings
unsigned int scr_width = 800;
unsigned int scr_height = 600;
bool have_shadow = true;
bool have_rotate = false;
// camera
Camera camera(pointf3(0.0f, 0.0f, 3.0f));
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
    // add Plane
    // ------------------------------------------------------------------
    std::vector<pointf3> positions(spot->positions);
    std::vector<pointf2> texcoords(spot->texcoords);
    std::vector<normalf> normals(spot->normals);
    std::vector<vecf3> tangents(spot->tangents);
    std::vector<GLuint> indices(spot->indices);
    size_t spot_point_size = positions.size();

    float sign_i = 1;
    float plane_size = 20.0f;
    for (size_t i = 0; i < 2; i++) {
        float sign_j = 1;
        for (size_t j = 0; j < 2; j++) {
            positions.push_back(pointf3{sign_i * plane_size, -0.57f, sign_j * plane_size});
            normals.push_back(normalf{0.0f, 1.0f, 0.0f});
            tangents.push_back(vecf3{1.0f, 0.0f, 0.0f});
            sign_j = -sign_j;
        }
        sign_i = -sign_i;
    }
    texcoords.push_back({plane_size, 0.0f});
    texcoords.push_back({plane_size, plane_size});
    texcoords.push_back({0.0f, 0.0f});
    texcoords.push_back({plane_size, plane_size});
    size_t two_tri[6] = {
        0, 1, 2,
        1 ,3, 2};
    for (auto i : two_tri)
        indices.push_back(i+spot_point_size);

    gl::VertexBuffer vb_pos(positions.size() * sizeof(pointf3), positions[0].data());
    gl::VertexBuffer vb_uv(texcoords.size() * sizeof(pointf2), texcoords[0].data());
    gl::VertexBuffer vb_n(normals.size() * sizeof(normalf), normals[0].data());
    gl::VertexBuffer vb_t(tangents.size() * sizeof(vecf3), tangents[0].data());
    gl::ElementBuffer eb(gl::BasicPrimitiveType::Triangles, indices.size() / 3, indices.data());


    gl::VertexArray::Format format;
    format.attrptrs.push_back(vb_pos.AttrPtr(3, gl::DataType::Float, false, sizeof(pointf3)));
    format.attrptrs.push_back(vb_uv.AttrPtr(2, gl::DataType::Float, false, sizeof(pointf2)));
    format.attrptrs.push_back(vb_n.AttrPtr(3, gl::DataType::Float, false, sizeof(normalf)));
    format.attrptrs.push_back(vb_t.AttrPtr(3, gl::DataType::Float, false, sizeof(vecf3)));
    format.eb = &eb;

    gl::VertexArray spot_plane({0, 1, 2, 3}, format);


    // add plane VAO
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
       // positions            // normals         // texcoords
       25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
       -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
       -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,

       25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
       -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
       25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f};
    // plane VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

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
    shadowmap.SetWrapFilter(gl::WrapMode::ClampToBorder, gl::WrapMode::ClampToBorder,
        gl::MinFilter::Nearest, gl::MagFilter::Nearest);
    rgbaf borderColor{ 1.f,1.f,1.f,1.f };
    gl::TexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor.data());
    //创建帧缓冲（深度缓冲）
    gl::FrameBuffer shadowFB;
    //将纹理附加到帧缓冲
    shadowFB.Attach(gl::FramebufferAttachment::DepthAttachment, &shadowmap);
    //=====================================================
    //unsigned int depthMapFBO;
    //glGenFramebuffers(1, &depthMapFBO);
    //// create depth texture
    //unsigned int depthMap;
    //glGenTextures(1, &depthMap);
    //glBindTexture(GL_TEXTURE_2D, depthMap);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_TEXTURE_SIZE, SHADOW_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //// attach depth texture as FBO's depth buffer
    //glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
        shadowFB.Bind();
        //glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        gl::Viewport({ 0,0 }, SHADOW_TEXTURE_SIZE, SHADOW_TEXTURE_SIZE);
        gl::Clear(gl::BufferSelectBit::DepthBufferBit);

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
        if (have_rotate)
            angle = 20.0f + 10.f * ((float)glfwGetTime() - startTime + LastGLFTime);
        else
            angle = 20.0f + 10.f * (LastGLFTime);
        pointf3 lightPos(3 * cos(to_radian(angle)), 4.0f, 3 * sin(to_radian(angle)));

        float near_plane = 0.1f, far_plane = 100.0f;
        //transformf lightProjection = transformf::perspective(to_radian(45.0f), (float)SHADOW_TEXTURE_SIZE / (float)SHADOW_TEXTURE_SIZE, near_plane, far_plane);
        transformf lightProjection = transformf::orthographic(-10.0f, 10.0f,-10.0f,10.0f, 0.1f, 10.0f);
        transformf lightView = transformf::look_at(lightPos, pointf3(0.0f, 0.1f, 0.0f), vecf3(0.0f, 1.0f, 0.0f));
        transformf lightSpaceMatrix = lightProjection * lightView;
        shadow_program.Use();
        shadow_program.SetMatf4("light_space_matrix", lightSpaceMatrix);
        //transformf model(pointf3{0, 0, 0}, quatf { vecf3(0.0f, 1.0f, 0.0f), to_radian(20.0f) });
        //shadow_program.SetMatf4("model", model);
        //spot_plane.Draw(&shadow_program);
        renderScene(shadow_program);

        //=================================

        gl::FrameBuffer::BindReset(); // default framebuffer
        gl::Viewport({ 0,0 }, scr_width, scr_height);
        gl::ClearColor({ ambient, 1.0f });
        gl::Clear(gl::BufferSelectBit::ColorBufferBit | gl::BufferSelectBit::DepthBufferBit);
        //Debug
        debug_depthmap_program.Use();
        debug_depthmap_program.Active(0, &shadowmap);
        debug_depthmap_program.SetFloat("near_plane", near_plane);
        debug_depthmap_program.SetFloat("far_plane", far_plane);
        //renderQuad();

        // 场景添加阴影
        light_shadow_program.Use();
        light_shadow_program.SetVecf3("camera_pos", camera.Position);

        // bind textures on corresponding texture units
        // 绑定目标纹理
        light_shadow_program.Active(0, &wood_texture);

        // 绑定深度纹理
        light_shadow_program.Active(1, &shadowmap);

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

        light_shadow_program.SetMatf4("model", model);
        spot_plane.Draw(&light_shadow_program);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    delete spot;
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);
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
void renderScene(const gl::Program& shader) {
    // floor
    transformf model = transformf(pointf3(1.0f));
    shader.SetMatf4("model", model);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // cubes
    model = transformf({0.0f, 1.5f, 0.0},scalef3(0.5f));
    shader.SetMatf4("model", model);
    renderCube(shader);
    model = transformf({2.0f, 0.0f, 1.0},scalef3(0.5f));
    shader.SetMatf4("model", model);
    renderCube(shader);
    model = transformf({-1.0f, 0.0f, 2.0}, scalef3(1.0f), quatf{vecf3{1.0, 0.0, 1.0}, to_radian(60.0f)});
    shader.SetMatf4("model", model);
    renderCube(shader);
}
// Cube
//  renderCube() renders a 1x1 3D cube in NDC.
//  -------------------------------------------------

void renderCube(const gl::Program& shader) {
    // initialize (if necessary)
    if (cubeVAO == 0) {
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
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

//Quad

void renderQuad(const gl::Program& shader)
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}