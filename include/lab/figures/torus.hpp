#ifndef FIGURES_TORUS_HPP
#define FIGURES_TORUS_HPP

#include "lab/figures/figure.hpp"

#include <vector>

class Torus final: public Figure
{
 public:
  Torus(float radius_in, float radius_out);

  void render() override;

 private:
  size_t vert_count = 0;
  std::vector< float > vertices;
};

#endif
