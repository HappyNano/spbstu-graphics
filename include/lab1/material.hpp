#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <array>

#include <GL/freeglut.h>

struct MaterialConf
{
  MaterialConf() = default;
  MaterialConf(const std::array< GLfloat, 4 > & ambient,
   const std::array< GLfloat, 4 > & diffuse,
   const std::array< GLfloat, 4 > & specular,
   const std::array< GLfloat, 1 > & shinisess);

  GLfloat ambient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
  GLfloat specular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
  GLfloat shininess[1] = { 0.0f };
};

#endif
