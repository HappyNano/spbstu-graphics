#include "lab/figures/surface.hpp"

#include "glad/glad.h"

Surface::Surface()
{
  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float planeVertices[] = {
    // positions         // normals        // texcoords
    25.0f, 0.0f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,   //
    -25.0f, 0.0f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,   //
    -25.0f, 0.0f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f, //

    25.0f, 0.0f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,   //
    -25.0f, 0.0f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f, //
    25.0f, 0.0f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f  //
  };
  // plane VAO
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Surface::render()
{
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}
