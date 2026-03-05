#pragma once
#include <string>

class Shader {
public:
    unsigned int ID;

    Shader(const std::string& vertPath, const std::string& fragPath);
    void use();
    void setMat4(const std::string& name, const float* value);
};