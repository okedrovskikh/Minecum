#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    unsigned int ID;
    Shader();
    Shader(const char* vertexPath, const char* fragmentPath);
    Shader(unsigned int& vertex, const char* fragmentPath);
    Shader(const char* vertexPath, unsigned int& fragment);
    Shader(unsigned int& vertex, unsigned int& fragment);
    void use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    ~Shader();
};

unsigned int VertexShader(const char* vertexPath);
unsigned int FragmentShader(const char* fragmentPath);
void ShaderProgram(unsigned int& ID, unsigned int vertex, unsigned int fragment);