#include "lab/figures/torus.hpp"

#include "glad/glad.h"

#include <cmath>

Torus::Torus(float radius_in, float radius_out)
{
  const size_t nR = 8, nr = 8;

  // float du = 2 * M_PI / nR;
  // float dv = 2 * M_PI / nr;

  // for (size_t i = 0; i < nR; i++)
  // {
  //   float u = i * du;
  //   for (size_t j = 0; j <= nr; j++)
  //   {
  //     float v = (j % nr) * dv;

  //     for (size_t k = 0; k < 2; k++)
  //     {
  //       float uu = u + k * du;
  //       // compute vertex
  //       float x = (radius_out + radius_in * cos(v)) * cos(uu);
  //       float y = (radius_out + radius_in * cos(v)) * sin(uu);
  //       float z = radius_in * sin(v);

  //       // compute normal
  //       float nx = cos(v) * cos(uu);
  //       float ny = cos(v) * sin(uu);
  //       float nz = sin(v);

  //       // compute texture coords
  //       float tx = uu / (2 * M_PI);
  //       float ty = v / (2 * M_PI);

  //       // add vertex
  //       vertices.insert(vertices.end(), { x, y, z, nx, ny, nz, tx, ty });
  //     }
  //     // incr angle
  //     v += dv;
  //   }
  // }

  // glGenVertexArrays(1, &VAO);
  // glGenBuffers(1, &VBO);
  // // fill buffer
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
  // // link vertex attributes
  // glBindVertexArray(VAO);
  // glEnableVertexAttribArray(0);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  // glEnableVertexAttribArray(1);
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  // glEnableVertexAttribArray(2);
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);

  std::vector< float > _vertices, _normals, _texCoords;

  float du = 2 * M_PI / nR;
  float dv = 2 * M_PI / nr;

  for (size_t i = 0; i < nR; i++)
  {

    float u = i * du;

    for (size_t j = 0; j <= nr; j++)
    {

      float v = (j % nr) * dv;

      for (size_t k = 0; k < 2; k++)
      {
        float uu = u + k * du;
        // compute vertex
        float x = (radius_out + radius_in * cos(v)) * cos(uu);
        float y = (radius_out + radius_in * cos(v)) * sin(uu);
        float z = radius_in * sin(v);

        // add vertex
        _vertices.push_back(x);
        _vertices.push_back(y);
        _vertices.push_back(z);

        // compute normal
        float nx = cos(v) * cos(uu);
        float ny = cos(v) * sin(uu);
        float nz = sin(v);

        // add normal
        _normals.push_back(nx);
        _normals.push_back(ny);
        _normals.push_back(nz);

        // compute texture coords
        float tx = uu / (2 * M_PI);
        float ty = v / (2 * M_PI);

        // add tex coords
        _texCoords.push_back(tx);
        _texCoords.push_back(ty);
      }

      // incr angle
      v += dv;
    }
  }

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  vert_count = _vertices.size() / 3;

  GLuint buffer[3];

  // create buffers
  glGenBuffers(3, buffer);

  // vertices

  // Initialize the first buffer
  glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);
  // Bind it to the vertex array - offset zero, stride = sizeof(vec3)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  // Tell OpenGL what the format of the attribute is
  glEnableVertexAttribArray(0);

  // normals

  // Initialize the first buffer
  glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _normals.size(), _normals.data(), GL_STATIC_DRAW);
  // Bind it to the vertex array - offset zero, stride = sizeof(vec3)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  // Tell OpenGL what the format of the attribute is
  glEnableVertexAttribArray(1);

  // tex coords

  // Initialize the first buffer
  glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _texCoords.size(), _texCoords.data(), GL_STATIC_DRAW);
  // Bind it to the vertex array - offset zero, stride = sizeof(vec2)
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  // Tell OpenGL what the format of the attribute is
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void Torus::render()
{
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, vert_count);
  glBindVertexArray(0);
}
