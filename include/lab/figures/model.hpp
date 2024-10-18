#ifndef FIGURES_MODEL_HPP
#define FIGURES_MODEL_HPP

#include "lab/figures/figure.hpp"

#include <filesystem>
#include <vector>

class Model final: public Figure
{
 public:
  Model(const std::filesystem::path & obj_path);

  void render() override;

 private:
  size_t _vertices_count = 0;

  std::vector< float > _load_model(const std::filesystem::path & obj_path);
};

#endif
