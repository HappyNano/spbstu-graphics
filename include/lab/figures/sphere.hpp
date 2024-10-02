#ifndef FIGURES_SPHERE_HPP
#define FIGURES_SPHERE_HPP

#include "lab/figures/figure.hpp"

#include <vector>

class Sphere final: public Figure
{
 public:
  Sphere(float radius);

  void render() override;

 private:
  size_t _vertices_count = 0;
  size_t _ind_count = 0;
  unsigned int IBO;
};

#endif
