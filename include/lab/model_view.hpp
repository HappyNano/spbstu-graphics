#ifndef LAB_MODEL_VIEW_HPP
#define LAB_MODEL_VIEW_HPP

#include <glm/glm.hpp>

class ModelView final
{
 public:
  using this_t = ModelView;

  ModelView();
  ModelView(const this_t &) = default;
  ~ModelView() = default;

  auto translate(const glm::vec3 & pos) -> this_t &;
  auto rotateX(float angle) -> this_t &;
  auto rotateY(float angle) -> this_t &;
  auto rotateZ(float angle) -> this_t &;
  auto scale(const glm::vec3 & x) -> this_t &;

  void reset();

  glm::mat4 get_modelView() const noexcept;
  glm::vec3 get_pos() const noexcept;

 private:
  glm::mat4 _model; ///< Model View matrix
};

#endif
