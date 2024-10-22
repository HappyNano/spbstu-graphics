#ifndef FIGURES_CONE_HPP
#define FIGURES_CONE_HPP

#include "lab/figures/figure.hpp"

#include <vector>

class Cone final: public Figure
{
 public:
  Cone(float radius, float height);

  void render() override;

 private:
  size_t _vertices_count = 0;
};

#endif
