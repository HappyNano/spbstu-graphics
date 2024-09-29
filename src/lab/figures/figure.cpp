#include "lab/figures/figure.hpp"

#include "glad/glad.h"

Figure::~Figure()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}
