#include "camera/camera.h"

namespace WhiteDeer {
namespace Engine {
glm::vec3 Camera::GetZDirection() {
  auto dir = m_pos - m_target;
  if (dir.length == 0) {
    return glm::vec3(0, 0, -1);
  }
  return glm::normalize(dir);
}

glm::vec3 Camera::GetYDirection() {
  return glm::normalize(glm::cross(GetZDirection(), GetXDirection()));
}

glm::vec3 Camera::GetXDirection() {
  return glm::normalize(glm::cross(glm::vec3(0, 1, 0), GetZDirection()));
}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAtRH(m_pos, m_target, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix() {
  return glm::perspective(glm::radians(m_fov), m_aspect, 0.1f, 100.0f);
}

}  // namespace Engine
}  // namespace WhiteDeer
