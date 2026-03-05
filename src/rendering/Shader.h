#pragma once
#include <string>
#include <glad/glad.h>

class Shader {
public:
    unsigned int ID;

    Shader(const std::string& vertPath, const std::string& fragPath);
    void use();
    void setMat4(const std::string& name, const float* value);
    void setVec3(const std::string& name, float x, float y, float z) {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
};