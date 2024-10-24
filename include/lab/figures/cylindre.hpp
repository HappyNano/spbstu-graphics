#ifndef FIGURES_CYLINDRE_HPP
#define FIGURES_CYLINDRE_HPP

#include "lab/figures/figure.hpp"

class Cylindre final: public Figure
{
 public:
  Cylindre(float radius, float height);

  float get_radius() const noexcept;
  float get_height() const noexcept;

 private:
  float _radius, _height;

  size_t _vertices_count = 0;

  void _render() override;
};

#endif
