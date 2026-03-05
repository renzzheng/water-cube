// GLSL shader management

#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertPath, const std::string& fragPath) {
    // read vertex shader file
    std::ifstream vertFile(vertPath);
    std::stringstream vertStream;
    vertStream << vertFile.rdbuf();
    std::string vertCode = vertStream.str();
    const char* vertSrc = vertCode.c_str();

    // read fragment shader file
    std::ifstream fragFile(fragPath);
    std::stringstream fragStream;
    fragStream << fragFile.rdbuf();
    std::string fragCode = fragStream.str();
    const char* fragSrc = fragCode.c_str();

    // compile vertex shader
    unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertSrc, nullptr);
    glCompileShader(vert);

    // compile fragment shader
    unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragSrc, nullptr);
    glCompileShader(frag);

    // link into a shader program
    ID = glCreateProgram();
    glAttachShader(ID, vert);
    glAttachShader(ID, frag);
    glLinkProgram(ID);

    // cleanup
    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setMat4(const std::string& name, const float* value) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}