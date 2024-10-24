#include "lab/model_view.hpp"

#include <glm/ext.hpp>

ModelView::ModelView():
  _model{ 1.0f }
{}
auto ModelView::translate(const glm::vec3 & pos) -> this_t &
{
  _model = glm::translate(_model, pos);
  return *this;
}
auto ModelView::rotateX(float angle) -> this_t &
{
  _model = glm::rotate(_model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
  return *this;
}
auto ModelView::rotateY(float angle) -> this_t &
{
  _model = glm::rotate(_model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
  return *this;
}
auto ModelView::rotateZ(float angle) -> this_t &
{
  _model = glm::rotate(_model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
  return *this;
}
auto ModelView::scale(const glm::vec3 & x) -> this_t &
{
  _model = glm::scale(_model, x);
  return *this;
}

void ModelView::reset()
{
  _model = glm::mat4(1.0f);
}

glm::mat4 ModelView::get_modelView() const noexcept
{
  return _model;
}
glm::vec3 ModelView::get_pos() const noexcept
{
  return glm::vec3(_model[3]);
}
