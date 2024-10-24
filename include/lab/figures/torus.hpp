#ifndef FIGURES_TORUS_HPP
#define FIGURES_TORUS_HPP

#include "lab/figures/figure.hpp"

class Torus final: public Figure
{
 public:
  Torus(float radius_in, float radius_out);

 private:
  size_t _vertices_count = 0;

  void _render() override;
};

#endif
