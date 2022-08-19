#include "camera/camera.h"

#include "log/log.h"

namespace WhiteDeer {
namespace Engine {
glm::vec3 Camera::GetZDirection() { return m_dir; }

glm::vec3 Camera::GetYDirection() {
  return glm::normalize(glm::cross(GetZDirection(), GetXDirection()));
}

glm::vec3 Camera::GetXDirection() {
  return glm::normalize(glm::cross(glm::vec3(0, 1, 0), GetZDirection()));
}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAtRH(m_pos, GetTargetPos(), glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix() {
  return glm::perspective(glm::radians(m_fov), m_aspect, 0.1f, 100.0f);
}

void Camera::Move(float x, float y, float z) {
  // move toward target
  m_pos += z * m_speed * m_dir;
  m_pos += y * m_speed * GetYDirection();
  m_pos += x * m_speed * GetXDirection();
}

void Camera::Rotate(float x, float y) {
  {
    // rotate around world-y axis
    glm::mat4 rotationMat(1);
    rotationMat = glm::rotate(rotationMat, x, glm::vec3(0.0, 1.0, 0.0));
    auto newdir = rotationMat * glm::vec4(m_dir, 1.0);
    m_dir = glm::normalize(glm::vec3(newdir.x, newdir.y, newdir.z));
  }

  {
    // rotate around local-x axis
    glm::mat4 rotationMat(1);
    rotationMat = glm::rotate(rotationMat, y, GetXDirection());
    auto newdir = rotationMat * glm::vec4(m_dir, 1.0);
    m_dir = glm::normalize(glm::vec3(newdir.x, newdir.y, newdir.z));
  }

  LOGE << m_dir.x << ' ' << m_dir.y << ' ' << m_dir.z;
}

}  // namespace Engine
}  // namespace WhiteDeer
