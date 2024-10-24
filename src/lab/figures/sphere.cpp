#include "lab/figures/sphere.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>

#include <vector>
#include <cmath>

Sphere::Sphere(float radius):
  _radius{ radius }
{
  int stackCount = 64, sectorCount = 64;

  std::vector< float > vertices;
  std::vector< GLuint > indicesVector;

  float x, y, z, xy;                           // vertex position
  float nx, ny, nz, lengthInv = 1.0f / radius; // vertex normal
  float s, t;                                  // vertex texCoord

  float sectorStep = 2 * M_PI / sectorCount;
  float stackStep = M_PI / stackCount;
  float sectorAngle, stackAngle;

  for (int i = 0; i <= stackCount; ++i)
  {
    stackAngle = M_PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
    xy = radius * cosf(stackAngle);        // r * cos(u)
    z = radius * sinf(stackAngle);         // r * sin(u)

    // add (sectorCount+1) vertices per stack
    // first and last vertices have same position and normal, but different tex coords
    for (int j = 0; j <= sectorCount; ++j)
    {
      sectorAngle = j * sectorStep; // starting from 0 to 2pi

      // vertex position (x, y, z)
      x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
      y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)

      // normalized vertex normal (nx, ny, nz)
      nx = x * lengthInv;
      ny = y * lengthInv;
      nz = z * lengthInv;

      // vertex tex coord (s, t) range between [0, 1]
      s = (float)j / sectorCount;
      t = (float)i / stackCount;
      vertices.insert(vertices.end(), { x, y, z, nx, ny, nz, s, t });
    }
  }

  int k1, k2;
  for (int i = 0; i < stackCount; ++i)
  {
    k1 = i * (sectorCount + 1); // beginning of current stack
    k2 = k1 + sectorCount + 1;  // beginning of next stack

    for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
    {
      // 2 triangles per sector excluding first and last stacks
      // k1 => k2 => k1+1
      if (i != 0)
      {
        indicesVector.push_back(k1);
        indicesVector.push_back(k2);
        indicesVector.push_back(k1 + 1);
      }

      // k1+1 => k2 => k2+1
      if (i != (stackCount - 1))
      {
        indicesVector.push_back(k1 + 1);
        indicesVector.push_back(k2);
        indicesVector.push_back(k2 + 1);
      }
    }
  }

  _vertices_count = vertices.size() / 8;
  _ind_count = indicesVector.size();

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &IBO);
  // fill buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
  // link vertex attributes
  glBindVertexArray(VAO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, _ind_count * sizeof(GLuint), indicesVector.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Sphere::_render()
{
  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glDrawElements(GL_TRIANGLE_STRIP, _ind_count, // count
   GL_UNSIGNED_INT,                             // type
   (void *)0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

float Sphere::get_radius() const
{
  return _radius;
}
