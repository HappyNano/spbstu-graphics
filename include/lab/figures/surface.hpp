#ifndef FIGURES_SURFACE_HPP
#define FIGURES_SURFACE_HPP

#include "lab/figures/figure.hpp"

class Surface final: public Figure
{
 public:
  Surface(float size = 25.0f);

  float get_size() const noexcept;

 private:
  float _size;

  void _render() override;
};

#endif
