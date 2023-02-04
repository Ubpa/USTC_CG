#include <UGL/UGL>
#include <UGM/UGM>

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "../../tool/Camera.h"
#include "../../tool/SimpleLoader.h"
#include "ANN/ANN.h"
#include <iostream>

using namespace Ubpa;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
gl::Texture2D loadTexture(char const* path);
gl::Texture2D genDisplacementmap(const SimpleLoader::OGLResources* resources);
void fillHole(float data[], std::vector<float>& displacements, const SimpleLoader::OGLResources* resources, int width, int height, int K = 10, int threshold = 10);

// settings
unsigned int scr_width = 800;
unsigned int scr_height = 600;
float displacement_bias = 0.f;
float displacement_scale = 1.f;
float displacement_lambda = 0.2f;
bool have_denoise = false;
bool have_rotate = true;

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


//keyboard
bool SpacePressState = false;
bool EnterPressState = false;
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
    GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "HW8 - denoise", NULL, NULL);
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
    gl::Shader vs(gl::ShaderType::VertexShader, "../data/shaders/p3t2n3_denoise.vert");
    gl::Shader fs(gl::ShaderType::FragmentShader, "../data/shaders/light.frag");
    gl::Program program(&vs, &fs);
    rgbf ambient{ 0.2f,0.2f,0.2f };
    program.SetTex("albedo_texture", 0);
    program.SetTex("displacementmap", 1);
    program.SetVecf3("point_light_pos", { 0,5,0 });
    program.SetVecf3("point_light_radiance", { 100,100,100 });
    program.SetVecf3("ambient_irradiance", ambient);
    program.SetFloat("roughness", 0.5f );
    program.SetFloat("metalness", 0.f);

    // load model
    // ------------------------------------------------------------------
    auto spot = SimpleLoader::LoadObj("../data/models/spot_triangulated_good.obj", true);
    // world space positions of our cubes
	int instance_size = 1;
    pointf3 instancePositions[] = {
        pointf3(0.0f,  0.0f,  0.0f),
        //pointf3(2.0f,  5.0f, -15.0f),
        //pointf3(-1.5f, -2.2f, -2.5f),
        //pointf3(-3.8f, -2.0f, -12.3f),
        //pointf3(2.4f, -0.4f, -3.5f),
        //pointf3(-1.7f,  3.0f, -7.5f),
        //pointf3(1.3f, -2.0f, -2.5f),
        //pointf3(1.5f,  2.0f, -2.5f),
        //pointf3(1.5f,  0.2f, -1.5f),
        //pointf3(-1.3f,  1.0f, -1.5f)
    };

    // load and create a texture 
    // -------------------------
    gl::Texture2D spot_albedo = loadTexture("../data/textures/spot_albedo.png");

    gl::Texture2D displacementmap = genDisplacementmap(spot);

    // render loop
    // -----------
    float angle=0;
    startTime = (float)glfwGetTime();
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
        gl::ClearColor({ ambient, 1.0f });
        gl::Clear(gl::BufferSelectBit::ColorBufferBit | gl::BufferSelectBit::DepthBufferBit); // also clear the depth buffer now!

        program.SetVecf3("camera_pos", camera.Position);

        // bind textures on corresponding texture units
        program.Active(0, &spot_albedo);
        program.Active(1, &displacementmap);

        // pass projection matrix to shader (note that in this case it could change every frame)
        transformf projection = transformf::perspective(to_radian(camera.Zoom), (float)scr_width / (float)scr_height, 0.1f, 100.f);
        program.SetMatf4("projection", projection);

        // camera/view transformation
        program.SetMatf4("view", camera.GetViewMatrix());
        program.SetFloat("displacement_bias", displacement_bias);
        program.SetFloat("displacement_scale", displacement_scale);
        program.SetFloat("displacement_lambda", displacement_lambda);
        program.SetBool("have_denoise", have_denoise);

        // render spots
        for (unsigned int i = 0; i < instance_size; i++)
        {   
            // calculate the model matrix for each object and pass it to shader before drawing
            if (have_rotate) {
                angle = 20.0f * i + 10.f * ((float)glfwGetTime()-startTime + LastGLFTime);
            }
            else {
                angle = 20.0f * i + 10.f * (LastGLFTime);
            }
            transformf model(instancePositions[i], quatf{ vecf3(1.0f, 0.3f, 0.5f), to_radian(angle) });
            program.SetMatf4("model", model);
            spot->va->Draw(&program);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    delete spot;

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
        have_denoise = !have_denoise;
    }
    else if ((glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) && EnterPressState) {
        EnterPressState = false;
    }
    if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)&& !SpacePressState) {
        SpacePressState = true;
        if (have_rotate) {
            LastGLFTime = (float)glfwGetTime() - startTime + LastGLFTime;
        }
        else {
            startTime = (float)glfwGetTime();
        }
        have_rotate = !have_rotate;
    }else if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)&& SpacePressState) {
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

gl::Texture2D genDisplacementmap(const SimpleLoader::OGLResources* resources) {
   float* displacementData = new float[1024 * 1024]();
   // TODO: HW8 - 1_denoise | genDisplacementmap
   // 1. set displacementData with resources's positions, indices, normals, ...
   // 2. change global variable: displacement_bias, displacement_scale, displacement_lambda
   std::cout<<"object have "<<resources->positions.size()<<" points"<<std::endl;
   // ...
   std::map<size_t, std::vector<size_t>> adj;//衔接点
   std::vector<pointf3> sum_list_p3(resources->positions.size(), pointf3(0,0,0));
   std::vector<vecf3> delta_list_v3(resources->positions.size(), vecf3(0,0,0));
   std::vector<unsigned int> connect_num_list(resources->positions.size(), 0);
   std::vector<float> displacements(resources->positions.size(), 0);
   const auto& positions(resources->positions);  // 顶点要改变找到最佳值
   const auto& normals = resources->normals;
   const auto& texcoords = resources->texcoords;
	const auto& indices = resources->indices;
   float min_scale = FLT_MAX;
   float max_scale = FLT_MIN;
   float error = 0;
   for(size_t i=0; i < positions.size(); i++){
       for(size_t j=0; j < positions.size(); j++){
           if(i == j) 
               continue;
           else if(positions[i].distance(positions[j]) < 1e-5){
               auto iter = adj.find(i);
               if(iter == adj.end()){
                   adj.insert(std::make_pair(i, std::vector<size_t>()));
               }
               adj[i].push_back(j);
           }
       }
   }
   for(int i = 0; i < indices.size(); i+=3){
       size_t idx0 = indices[i];
       size_t idx1 = indices[i + 1];
       size_t idx2 = indices[i + 2];
       const auto& p0 = positions[idx0];
       const auto& p1 = positions[idx1];
       const auto& p2 = positions[idx2];
       auto v01 = p1 - p0;
       auto v02 = p2 - p0;
       auto v12 = p2 - p1;
       //除以2是因为每个边都会被计算两次
       sum_list_p3[idx0] += (-v01 - v02) / 2;
       sum_list_p3[idx1] += (v01 - v12) / 2;
       sum_list_p3[idx2] += (v02 + v12) / 2;
       connect_num_list[idx0] += 1;
       connect_num_list[idx1] += 1;
       connect_num_list[idx2] += 1;
   }
   for(int i = 0; i < resources->positions.size(); i++){
       //num product
       delta_list_v3[i] = sum_list_p3[i].cast_to<vecf3>() / connect_num_list[i];

       //dot
       float disp = delta_list_v3[i].dot(normals[i].cast_to<vecf3>());

       ////衔接点偏移的平均值:消除割裂
       auto iter = adj.find(i);
       if(iter != adj.end()){
           for(auto& idx : iter->second){
               delta_list_v3[idx] = (sum_list_p3[idx]).cast_to<vecf3>() / connect_num_list[idx];
               disp += delta_list_v3[idx].dot(normals[idx].cast_to<vecf3>());
           }
           disp /= (iter->second.size() + 1);
       }
       displacements[i] = disp;
       if (disp < min_scale)
           min_scale = disp;
       if (disp > max_scale)
           max_scale = disp;
       error += disp * displacement_lambda;
   }
   displacement_scale = (max_scale - min_scale);
   displacement_bias = min_scale;
   displacement_lambda = 0.8;
   std::cout << "bias: " << displacement_bias << std::endl;
   std::cout << "scale: " << displacement_scale << std::endl;
	std::cout << "error: " << error << std::endl;
	// 将displacements的值映射到[0, 1]之间
   for(auto& d : displacements){
       d = (d - displacement_bias) / displacement_scale;
   }
   fillHole(displacementData, displacements, resources, 1024, 1024, 1, 100);
   // for (size_t i = 0; i < resources->texcoords.size(); i++) {
   //     int x = int(resources->texcoords[i][0] * 1024);
   //     int y = int(resources->texcoords[i][1] * 1024);
   //     int pos = 1024 * y + x;
   //     displacementData[pos] = displacements[i];
   // }
   gl::Texture2D displacementmap;
   displacementmap.SetImage(0, gl::PixelDataInternalFormat::Red, 1024, 1024, gl::PixelDataFormat::Red, gl::PixelDataType::Float, displacementData);
   displacementmap.SetWrapFilter(gl::WrapMode::Repeat, gl::WrapMode::Repeat,
       gl::MinFilter::Linear, gl::MagFilter::Linear);
   stbi_uc* stbi_data = new stbi_uc[1024 * 1024];
   for (size_t i = 0; i < 1024 * 1024; i++)
       stbi_data[i] = static_cast<stbi_uc>(std::clamp(displacementData[i] * 255.f, 0.f, 255.f));
   stbi_write_png("../data/1_denoise_displacement_map.png", 1024, 1024, 1, stbi_data, 1024);
   delete[] stbi_data;
   delete[] displacementData;
   return displacementmap;
}

void fillHole(float data[], std::vector<float>& displacements, const SimpleLoader::OGLResources* resources, int width, int height, int K, int threshold) {
    int real_count = K;
    ANNpointArray dataPts = annAllocPts(width * height, 2);
    int nPts = resources->texcoords.size();
    //std::vector<pointi2> coords(resources->texcoords.size(), pointi2(0,0));
    //std::vector < std::vector<bool>> paint_mask;
    //paint_mask.resize(width, std::vector<bool>(height, false));
    std::map<pointi2, float> data_map;
    for (size_t i = 0; i < nPts; i++) {
        dataPts[i][0] = int(resources->texcoords[i][0] * width);
        dataPts[i][1] = int(resources->texcoords[i][1] * height);
        int pos = width * dataPts[i][1] + dataPts[i][0];
        data_map[pointi2(dataPts[i][0], dataPts[i][1])] = displacements[i];
    }
    
    ANNkd_tree* kdtree = new ANNkd_tree(dataPts, nPts, 2);
    
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            float val = 0;
            float dist_sum = 0;
            ANNpoint pt = annAllocPt(2);
            ANNidxArray index = new ANNidx[K];
            ANNdistArray dist = new ANNdist[K];
            pt[0] = i;
            pt[1] = j;
            kdtree->annkSearch(pt, K, index, dist, 0);

            real_count = K;
            for (int m = 0; m < K; m++) {
                int x = dataPts[index[m]][0];
                int y = dataPts[index[m]][1];
                if (fabs(x - i) <= threshold && fabs(y - j) <= threshold)//如果邻居的距离差别不是太大
                {
                    val += data_map[pointi2(x, y)];
                    //val += data_map[pointi2(x, y)]/dist[m];
                    //dist_sum += 1/dist[m];
                }
                else {
                    real_count--;
                }
            }
            if (real_count > 0) {  // 对邻居点的像素作平均近似
                data[j * width + i] = (val / real_count);
                //data[j * width + i] = val / dist_sum;
            }
            delete[] index;
            delete[] dist;
        }
    }
    delete kdtree;

}
