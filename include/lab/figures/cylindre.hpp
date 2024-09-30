#ifndef FIGURES_CYLINDRE_HPP
#define FIGURES_CYLINDRE_HPP

#include "lab/figures/figure.hpp"

#include <vector>

class Cylindre final: public Figure
{
 public:
  Cylindre(float radius, float height);

  void render() override;

 private:
  size_t _vertices_count = 0;
};

#endif
