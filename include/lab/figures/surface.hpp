#ifndef FIGURES_SURFACE_HPP
#define FIGURES_SURFACE_HPP

#include "lab/figures/figure.hpp"

class Surface final: public Figure
{
 public:
  Surface(float size = 25.0f);

  void render() override;
};

#endif
