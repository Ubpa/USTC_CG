#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>;  // 包含glad来获取所有的必须OpenGL头文件

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
    // 程序ID
    unsigned int ID;

    // 构造器读取并构建着色器
    Shader(const char* vertexPath, const char* fragmentPath);
    Shader(const std::string vertexPath, const std::string fragmentPath);
    // 使用/激活程序
    void use();
    // 删除程序
    void deleteShader();
    // uniform工具函数
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
private:
    // 工具函数，用于检查编译/链接错误
    void checkCompileErrors(unsigned int shader, std::string type);
};


#endif