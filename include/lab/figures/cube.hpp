#ifndef FIGURES_CUBE_HPP
#define FIGURES_CUBE_HPP

#include "lab/figures/figure.hpp"

class Cube final: public Figure
{
 public:
  Cube(float length);

  void render() override;
};

#endif
