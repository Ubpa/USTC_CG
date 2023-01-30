#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "shader.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int main() {

     // glfw: initialize and configure
     // ------------------------------
     glfwInit();
     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

     // glfw window creation
     // --------------------
     GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
     if (window == NULL) {
         std::cout << "Failed to create GLFW window" << std::endl;
         glfwTerminate();
         return -1;
     }
     glfwMakeContextCurrent(window);
     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

     // glad: load all OpenGL function pointers
     // ---------------------------------------
     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
         std::cout << "Failed to initialize GLAD" << std::endl;
         return -1;
     }

     // build and compile our shader program
     // ------------------------------------
     // main.cpp
     Shader ourShader("..\\shaders\\shader.vs", "..\\shaders\\shader.fs");  // you can name your shader files however you like

     // set up vertex data (and buffer(s)) and configure vertex attributes
     // ------------------------------------------------------------------
     // 更多属性 位置+颜色
     float vertices[] = {
         // positions         // colors
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // bottom right
         -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
         0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f     // top

     };

     unsigned int VBO, VAO;
     glGenVertexArrays(1, &VAO);
     glGenBuffers(1, &VBO);
     // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
     glBindVertexArray(VAO);

     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

     // 设置顶点属性指针，告诉OpenGL如何解析顶点数据
     // 参数1：顶点属性的位置值，对应顶点着色器中layout(location = 0)的位置值
     // 参数2：顶点属性的大小，vec3是3个值
     // 参数3：顶点属性的类型
     // 参数4：是否希望数据被标准化，如果设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间
     // 参数5：步长，连续顶点属性组之间的间隔
     // 参数6：偏移量，位置数据在缓冲中起始位置的偏移量
     // position attribute
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(0);
     // color attribute
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
     glEnableVertexAttribArray(1);

     // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
     // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
     // glBindVertexArray(0);

     // as we only have a single shader, we could also just activate our shader once beforehand if we want to
     ourShader.use();

     // render loop
     // -----------
     while (!glfwWindowShouldClose(window)) {
         // input
         // -----
         processInput(window);

         // render
         // ------
         glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
         glClear(GL_COLOR_BUFFER_BIT);

         // shader
         // ------
         ourShader.use();

         // render the triangle
         glBindVertexArray(VAO);//绑定VAO
         glDrawArrays(GL_TRIANGLES, 0, 3);

         // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
         // -------------------------------------------------------------------------------
         glfwSwapBuffers(window);
         glfwPollEvents();
     }

     // optional: de-allocate all resources once they've outlived their purpose:
     // ------------------------------------------------------------------------
     glDeleteVertexArrays(1, &VAO);
     glDeleteBuffers(1, &VBO);
     ourShader.deleteShader();

     // glfw: terminate, clearing all previously allocated GLFW resources.
     // ------------------------------------------------------------------
     glfwTerminate();
     return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
