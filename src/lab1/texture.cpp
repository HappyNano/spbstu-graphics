
#include "lab1/texture.hpp"

#include <iostream>
#include <fstream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint loadTexture(const std::string & filename)
{
  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Load the texture data
  int width, height, nrChannels;
  unsigned char * data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
  if (data)
  {
    GLenum format;
    if (nrChannels == 1)
    {
      format = GL_RED;
    }
    else if (nrChannels == 3)
    {
      format = GL_RGB;
    }
    else if (nrChannels == 4)
    {
      format = GL_RGBA;
    }
    std::cout << "Loaded image with width = " << width << " and height = " << height << " and format = " << format << '\n';
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cerr << "Failed to load texture: " << filename << std::endl;
  }

  stbi_image_free(data);
  return textureID;
}
