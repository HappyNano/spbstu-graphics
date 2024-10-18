#include "lab/figures/model.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>

#include <stdio.h>
#include <cstring>
#include <stdexcept>
#include <string>

Model::Model(const std::filesystem::path & obj_path)
{
  auto vertices = _load_model(obj_path);
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

void Model::render()
{
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, _vertices_count);
  glBindVertexArray(0);
}

std::vector< float > Model::_load_model(const std::filesystem::path & obj_path)
{
  std::vector< float > vertices;

  std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  std::vector< glm::vec3 > temp_vertices;
  std::vector< glm::vec2 > temp_uvs;
  std::vector< glm::vec3 > temp_normals;

  FILE * file = fopen(obj_path.c_str(), "r");
  if (file == NULL)
  {
    getchar();
    throw std::logic_error("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details");
  }

  while (1)
  {
    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF)
    {
      break; // EOF = End Of File. Quit the loop.
    }

    // else : parse lineHeader

    if (strcmp(lineHeader, "v") == 0)
    {
      glm::vec3 vertex;
      std::ignore = fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      temp_vertices.push_back(vertex);
    }
    else if (strcmp(lineHeader, "vt") == 0)
    {
      glm::vec2 uv;
      std::ignore = fscanf(file, "%f %f\n", &uv.x, &uv.y);
      uv.y =
       -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
      temp_uvs.push_back(uv);
    }
    else if (strcmp(lineHeader, "vn") == 0)
    {
      glm::vec3 normal;
      std::ignore = fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      temp_normals.push_back(normal);
    }
    else if (strcmp(lineHeader, "f") == 0)
    {
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1],
       &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
      if (matches != 9)
      {
        fclose(file);
        throw std::logic_error("File can't be read by our simple parser :-( Try exporting with other options");
      }
      vertexIndices.push_back(vertexIndex[0]);
      vertexIndices.push_back(vertexIndex[1]);
      vertexIndices.push_back(vertexIndex[2]);
      uvIndices.push_back(uvIndex[0]);
      uvIndices.push_back(uvIndex[1]);
      uvIndices.push_back(uvIndex[2]);
      normalIndices.push_back(normalIndex[0]);
      normalIndices.push_back(normalIndex[1]);
      normalIndices.push_back(normalIndex[2]);
    }
    else
    {
      // Probably a comment, eat up the rest of the line
      char stupidBuffer[1000];
      std::ignore = fgets(stupidBuffer, 1000, file);
    }
  }

  // For each vertex of each triangle
  for (unsigned int i = 0; i < vertexIndices.size(); i++)
  {
    // Get the indices of its attributes
    unsigned int vertexIndex = vertexIndices[i];
    unsigned int uvIndex = uvIndices[i];
    unsigned int normalIndex = normalIndices[i];

    // Get the attributes thanks to the index
    glm::vec3 vertex = temp_vertices[vertexIndex - 1];
    glm::vec2 uv = temp_uvs[uvIndex - 1];
    glm::vec3 normal = temp_normals[normalIndex - 1];

    // Put the attributes in buffers
    vertices.insert(vertices.end(), { vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z, uv.x, uv.y });
  }
  fclose(file);
  return vertices;
}
