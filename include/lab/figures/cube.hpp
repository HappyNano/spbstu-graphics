#ifndef FIGURES_CUBE_HPP
#define FIGURES_CUBE_HPP

#include "lab/figures/figure.hpp"

#include <glm/glm.hpp>

#include <vector>

struct Plane
{
  glm::vec3 normal, pos;
};

class Cube final: public Figure
{
 public:
  Cube(float length);

  void render() override;

  std::vector< Plane > getPlanes() const;
  float getLength() const noexcept;

 private:
  float _length;
  std::vector< Plane > _planes;
};

#endif
