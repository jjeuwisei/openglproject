#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Shader{
  unsigned int ID;

  Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      
      std::stringstream vShaderStream, fShaderStream;
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
      vShaderFile.close();
      fShaderFile.close();
    } catch(std::ifstream::failure& e) {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* vShaderChar = vertexCode.c_str();
    const char* fShaderChar = fragmentCode.c_str();
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderChar, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
    std::cout << "ERROR::VERTEX_SHADER::COMPILATION" << std::endl;
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderChar, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
      std::cout << "ERROR::FRAGMENT_SHADER::COMPILATION" << std::endl;
      glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
      std::cout << "ERROR::PROGRAM_LINKING" << std::endl;
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
    }
  }

  void use() {
    glUseProgram(ID);
  }

  void setBool(const char* name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name), (int)value);
  }

  void setInt(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name), value);
  }

  void setFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name), value);
  }

  void setVec3(const char* name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
  }
  void setMat4(const char* name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
  }  
};

#endif
