#include "lab/figures/cone.hpp"

#include "glad/glad.h"

#include <cmath>

Cone::Cone(float radius, float height)
{
  const int sides = 128;

  std::vector< float > vertices;

  // put side vertices to arrays
  const float theta = 2. * M_PI / (float)sides;
  float c = std::cos(theta), s = std::sin(theta);
  // coordinates on top of the circle, on xz plane
  float x2 = radius, z2 = 0;
  // make the strip
  for (int i = 0; i <= sides; i++)
  {
    // texture coord
    const float tx = (float)i / sides;
    // normal
    const float nf = 1. / std::sqrt(x2 * x2 + z2 * z2), xn = x2 * nf, zn = z2 * nf;
    vertices.insert(vertices.end(), { x2, 0.f, z2, xn, 0.f, zn, tx, 0.f });
    vertices.insert(vertices.end(), { 0.0f, height, 0.0f, 0.0f, 1.0f, 0.0f, tx, 1.0f });

    // next position
    const float x3 = x2;
    x2 = c * x2 - s * z2;
    z2 = s * x3 + c * z2;
  }

  _vertices_count = vertices.size() / 8;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
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
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Cone::render()
{
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertices_count);
  glBindVertexArray(0);
}
