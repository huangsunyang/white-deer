#include "components/camera.h"

#include "components/transform.h"
#include "log/log.h"
#include "scene/gameobject.h"

namespace WhiteDeer {
namespace Engine {

glm::vec3 Camera::GetPosition() {
  return m_gameobject->GetComponent<Transform>()->GetPosition();
}

glm::vec3 Camera::GetZDirection() { return m_dir; }

glm::vec3 Camera::GetYDirection() {
  return glm::normalize(glm::cross(GetZDirection(), GetXDirection()));
}

glm::vec3 Camera::GetXDirection() {
  return glm::normalize(glm::cross(glm::vec3(0, 1, 0), GetZDirection()));
}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAtRH(GetPosition(), GetTargetPos(), glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix() {
  return glm::perspective(glm::radians(m_fov), m_aspect, 0.1f, 100.0f);
}

void Camera::Move(float x, float y, float z) {
  // move toward target
  auto m_pos = GetPosition();
  m_pos += z * m_speed * m_dir;
  m_pos += y * m_speed * GetYDirection();
  m_pos += x * m_speed * GetXDirection();
  m_gameobject->GetComponent<Transform>()->SetPosition(m_pos.x, m_pos.y,
                                                       m_pos.z);
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
}

shared_ptr<RenderTexture> Camera::GetShadowMap() {
  if (!HasShadowMap()) {
    return nullptr;
  }
  if (m_shadowMap == nullptr) {
    m_shadowMap =
        RenderTexture::Create(m_shadowMapWidth, m_shadowMapHeight, RT_DEPTH);
  }
  m_shadowMap->Resize(m_shadowMapWidth, m_shadowMapHeight);
  return m_shadowMap;
}

void Camera::DoPostprocess(const RenderTexture& texture) {
  if (m_postprocessType == PostprocessType_None && !m_enableGammaCorrection) {
    return;
  }

  if (m_postprocessSrc == nullptr) {
    m_postprocessSrc = RenderTexture::Create(texture.GetWidth(),
                                             texture.GetHeight(), RT_COLOR);
  }

  if (m_postprocessType != PostprocessType_None) {
    // post process
    m_postprocessSrc->CopyFrom(texture);
    auto postProcess = Postprocess::GetOrLoad(m_postprocessType);
    postProcess->Render(*m_postprocessSrc, texture);
  }

  // gamma correction
  if (m_enableGammaCorrection) {
    m_postprocessSrc->CopyFrom(texture);
    auto postProcess = Postprocess::GetOrLoad(PostprocessType_Gamma);
    postProcess->Render(*m_postprocessSrc, texture);
  }
}

}  // namespace Engine
}  // namespace WhiteDeer
