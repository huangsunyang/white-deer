#include "components/transform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace WhiteDeer {
namespace Engine {

glm::mat4 Transform::GetModelMatrix() {
  return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
}

glm::mat4 Transform::GetTranslationMatrix() {
  return glm::translate(glm::mat4(1.0f), m_position);
}

glm::mat4 Transform::GetScaleMatrix() {
  return glm::scale(glm::mat4(1.0f), m_scale);
}

glm::mat4 Transform::GetRotationMatrix() {
  auto rotate_x =
      glm::rotate(glm::mat4(1.0f), m_rotation.x, glm::vec3(1, 0, 0));
  auto rotate_y =
      glm::rotate(glm::mat4(1.0f), m_rotation.y, glm::vec3(0, 1, 0));
  auto rotate_z =
      glm::rotate(glm::mat4(1.0f), m_rotation.z, glm::vec3(0, 0, 1));
  return rotate_z * rotate_y * rotate_x;
}

}  // namespace Engine
}  // namespace WhiteDeer