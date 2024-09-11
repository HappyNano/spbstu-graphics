#include "lab1/material.hpp"

MaterialConf::MaterialConf(const std::array< GLfloat, 4 > & _ambient,
 const std::array< GLfloat, 4 > & _diffuse,
 const std::array< GLfloat, 4 > & _specular,
 const std::array< GLfloat, 1 > & _shininess):
  ambient{ _ambient[0], _ambient[1], _ambient[2], _ambient[3] },
  diffuse{ _diffuse[0], _diffuse[1], _diffuse[2], _diffuse[3] },
  specular{ _specular[0], _specular[1], _specular[2], _specular[3] },
  shininess{ _shininess[0] }
{}
