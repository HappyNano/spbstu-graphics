#include "lab1/shaders.hpp"

#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>

GLuint addShared(const std::string & vertexSourcePwd, const std::string & fragmentSourcePwd)
{
  // Reading shaders
  std::string vertexSource, fragmentSource;
  std::ifstream vertexSource_fin(vertexSourcePwd), fragmentSource_fin(fragmentSourcePwd);
  if (!vertexSource_fin.is_open() || !fragmentSource_fin.is_open())
  {
    std::cerr << "Bad files\n";
    return 0;
  }
  vertexSource_fin >> std::noskipws;
  std::copy(std::istream_iterator< char >{ vertexSource_fin }, std::istream_iterator< char >{}, std::back_inserter(vertexSource));
  fragmentSource_fin >> std::noskipws;
  std::copy(std::istream_iterator< char >{ fragmentSource_fin }, std::istream_iterator< char >{}, std::back_inserter(fragmentSource));

  vertexSource_fin.close();
  fragmentSource_fin.close();

  // Create an empty vertex shader handle
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

  // Send the vertex shader source code to GL
  // Note that std::string's .c_str is NULL character terminated.
  const GLchar * source = (const GLchar *)vertexSource.c_str();
  glShaderSource(vertexShader, 1, &source, 0);

  // Compile the vertex shader
  glCompileShader(vertexShader);

  GLint isCompiled = 0;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector< GLchar > infoLog(maxLength);
    glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

    // We don't need the shader anymore.
    std::cout << "Iscompiled1 = false\n";
    glDeleteShader(vertexShader);

    // Use the infoLog as you see fit.

    // In this simple program, we'll just leave
    return 0;
  }

  // Create an empty fragment shader handle
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  // Send the fragment shader source code to GL
  // Note that std::string's .c_str is NULL character terminated.
  source = (const GLchar *)fragmentSource.c_str();
  glShaderSource(fragmentShader, 1, &source, 0);

  // Compile the fragment shader
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector< GLchar > infoLog(maxLength);
    glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

    // We don't need the shader anymore.
    glDeleteShader(fragmentShader);
    // Either of them. Don't leak shaders.
    glDeleteShader(vertexShader);

    // Use the infoLog as you see fit.
    std::cout << "Iscompiled2 = false\n";
    // In this simple program, we'll just leave
    return 0;
  }

  // Vertex and fragment shaders are successfully compiled.
  // Now time to link them together into a program.
  // Get a program object.
  GLuint program = glCreateProgram();

  // Attach our shaders to our program
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);

  // Link our program
  glLinkProgram(program);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
  if (isLinked == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector< GLchar > infoLog(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

    // We don't need the program anymore.
    glDeleteProgram(program);
    // Don't leak shaders either.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Use the infoLog as you see fit.
    std::cout << "IsLinked = false\n";
    // In this simple program, we'll just leave
    return 0;
  }

  // Always detach shaders after a successful link.
  // glDetachShader(program, vertexShader);
  // glDetachShader(program, fragmentShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  return program;
}