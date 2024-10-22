#include "lab/figures/cube.hpp"

#include "glad/glad.h"

Cube::Cube(float length):
  _length{ length }
{
  float vertices[] = {
    // back face
    -(length / 2.0f), -(length / 2.0f), -(length / 2.0f), 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
    (length / 2.0f), (length / 2.0f), -(length / 2.0f), 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
    (length / 2.0f), -(length / 2.0f), -(length / 2.0f), 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
    (length / 2.0f), (length / 2.0f), -(length / 2.0f), 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
    -(length / 2.0f), -(length / 2.0f), -(length / 2.0f), 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
    -(length / 2.0f), (length / 2.0f), -(length / 2.0f), 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left

    // front face
    -(length / 2.0f), -(length / 2.0f), (length / 2.0f), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
    (length / 2.0f), (length / 2.0f), (length / 2.0f), 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // bottom-right
    (length / 2.0f), -(length / 2.0f), (length / 2.0f), 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // top-right
    (length / 2.0f), (length / 2.0f), (length / 2.0f), 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
    -(length / 2.0f), -(length / 2.0f), (length / 2.0f), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top-left
    -(length / 2.0f), (length / 2.0f), (length / 2.0f), 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // bottom-left

    // left face
    -(length / 2.0f), (length / 2.0f), (length / 2.0f), -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
    -(length / 2.0f), -(length / 2.0f), -(length / 2.0f), -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top-left
    -(length / 2.0f), (length / 2.0f), -(length / 2.0f), -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // bottom-left
    -(length / 2.0f), -(length / 2.0f), -(length / 2.0f), -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
    -(length / 2.0f), (length / 2.0f), (length / 2.0f), -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
    -(length / 2.0f), -(length / 2.0f), (length / 2.0f), -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // top-right

    // right face
    (length / 2.0f), (length / 2.0f), (length / 2.0f), 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-left
    (length / 2.0f), -(length / 2.0f), -(length / 2.0f), 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
    (length / 2.0f), (length / 2.0f), -(length / 2.0f), 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-right
    (length / 2.0f), -(length / 2.0f), -(length / 2.0f), 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
    (length / 2.0f), (length / 2.0f), (length / 2.0f), 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-left
    (length / 2.0f), -(length / 2.0f), (length / 2.0f), 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-left

    // bottom face
    -(length / 2.0f), -(length / 2.0f), -(length / 2.0f), 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
    (length / 2.0f), -(length / 2.0f), -(length / 2.0f), 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
    (length / 2.0f), -(length / 2.0f), (length / 2.0f), 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
    (length / 2.0f), -(length / 2.0f), (length / 2.0f), 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
    -(length / 2.0f), -(length / 2.0f), (length / 2.0f), 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
    -(length / 2.0f), -(length / 2.0f), -(length / 2.0f), 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right

    // top face
    -(length / 2.0f), (length / 2.0f), -(length / 2.0f), 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
    (length / 2.0f), (length / 2.0f), (length / 2.0f), 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
    (length / 2.0f), (length / 2.0f), -(length / 2.0f), 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
    (length / 2.0f), (length / 2.0f), (length / 2.0f), 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
    -(length / 2.0f), (length / 2.0f), -(length / 2.0f), 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
    -(length / 2.0f), (length / 2.0f), (length / 2.0f), 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // bottom-left
  };
  _planes = {
    Plane{ glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3((length / 2.0f), -(length / 2.0f), -(length / 2.0f)) }, // back face
    Plane{ glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3((length / 2.0f), (length / 2.0f), (length / 2.0f)) },    // front face
    Plane{ glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-(length / 2.0f), (length / 2.0f), -(length / 2.0f)) }, // left face
    Plane{ glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3((length / 2.0f), (length / 2.0f), -(length / 2.0f)) },   // right face
    Plane{ glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3((length / 2.0f), -(length / 2.0f), (length / 2.0f)) },  // bottom face
    Plane{ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3((length / 2.0f), (length / 2.0f), -(length / 2.0f)) }    // top face
  };
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // fill buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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

void Cube::render()
{
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

std::vector< Plane > Cube::getPlanes() const
{
  return _planes;
}

float Cube::getLength() const noexcept
{
  return _length;
}
