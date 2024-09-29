#include "lab1/shaders.hpp"

#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>

Shader::Shader(const std::string & vertexSourcePwd, const std::string & fragmentSourcePwd)
{
  // Reading source of shaders
  std::ifstream vertexSource_fin, fragmentSource_fin;

  vertexSource_fin.open(vertexSourcePwd);
  fragmentSource_fin.open(fragmentSourcePwd);

  std::string vertexSource, fragmentSource;
  vertexSource_fin >> std::noskipws;
  std::copy(std::istream_iterator< char >{ vertexSource_fin }, std::istream_iterator< char >{}, std::back_inserter(vertexSource));
  fragmentSource_fin >> std::noskipws;
  std::copy(std::istream_iterator< char >{ fragmentSource_fin }, std::istream_iterator< char >{}, std::back_inserter(fragmentSource));

  vertexSource_fin.close();
  fragmentSource_fin.close();

  // Creating shaders
  const GLchar * vSource = (const GLchar *)vertexSource.c_str();
  const GLchar * fSource = (const GLchar *)fragmentSource.c_str();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vSource, 0);
  glCompileShader(vertexShader);
  _checkCompileErrors(vertexShader, "VERTEX");

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fSource, 0);
  glCompileShader(fragmentShader);
  _checkCompileErrors(fragmentShader, "FRAGMENT");

  _program = glCreateProgram();
  glAttachShader(_program, vertexShader);
  glAttachShader(_program, fragmentShader);
  glLinkProgram(_program);
  _checkCompileErrors(_program, "PROGRAM");

  // Before exit
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);
}

Shader::~Shader()
{}

void Shader::use()
{
  glUseProgram(_program);
}

// utility uniform functions
// ------------------------------------------------------------------------
void Shader::setBool(const std::string & name, bool value) const
{
  glUniform1i(glGetUniformLocation(_program, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string & name, int value) const
{
  glUniform1i(glGetUniformLocation(_program, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string & name, float value) const
{
  glUniform1f(glGetUniformLocation(_program, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string & name, const glm::vec2 & value) const
{
  glUniform2fv(glGetUniformLocation(_program, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string & name, float x, float y) const
{
  glUniform2f(glGetUniformLocation(_program, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string & name, const glm::vec3 & value) const
{
  glUniform3fv(glGetUniformLocation(_program, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string & name, float x, float y, float z) const
{
  glUniform3f(glGetUniformLocation(_program, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string & name, const glm::vec4 & value) const
{
  glUniform4fv(glGetUniformLocation(_program, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string & name, float x, float y, float z, float w) const
{
  glUniform4f(glGetUniformLocation(_program, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string & name, const glm::mat2 & mat) const
{
  glUniformMatrix2fv(glGetUniformLocation(_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string & name, const glm::mat3 & mat) const
{
  glUniformMatrix3fv(glGetUniformLocation(_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string & name, const glm::mat4 & mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::_checkCompileErrors(GLuint shader, std::string type)
{
  GLint success;
  GLchar infoLog[1024];
  if (type != "PROGRAM")
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- \n";
      throw std::logic_error("Bad compiling");
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- \n";
      throw std::logic_error("Bad compiling");
    }
  }
}