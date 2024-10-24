#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "glad/glad.h"
#include <string>

struct Texture2D final
{
  GLuint ID;

  void bind() const;
};

Texture2D loadTexture(const std::string & filename);

#endif
