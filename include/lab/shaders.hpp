#ifndef SHADERS_HPP
#define SHADERS_HPP

#include "glad/glad.h"
#include <glm/glm.hpp>

#include <string>
#include <memory>

class Shader final
{
 public:
  using shared = std::shared_ptr< Shader >;

  Shader(const std::string & vertexSourcePwd, const std::string & fragmentSourcePwd);
  ~Shader();

  void use();

  // utility uniform functions
  void setBool(const std::string & name, bool value) const;
  void setInt(const std::string & name, int value) const;
  void setFloat(const std::string & name, float value) const;
  void setVec2(const std::string & name, const glm::vec2 & value) const;
  void setVec2(const std::string & name, float x, float y) const;
  void setVec3(const std::string & name, const glm::vec3 & value) const;
  void setVec3(const std::string & name, float x, float y, float z) const;
  void setVec4(const std::string & name, const glm::vec4 & value) const;
  void setVec4(const std::string & name, float x, float y, float z, float w) const;
  void setMat2(const std::string & name, const glm::mat2 & mat) const;
  void setMat3(const std::string & name, const glm::mat3 & mat) const;
  void setMat4(const std::string & name, const glm::mat4 & mat) const;

  template < typename... Args >
  static shared makeShared(Args &&...);

 private:
  GLuint _program;

  void _checkCompileErrors(GLuint shader, std::string type);
};

template < typename... Args >
typename Shader::shared Shader::makeShared(Args &&... args)
{
  return std::make_shared< Shader >(std::forward< Args >(args)...);
}

#endif
