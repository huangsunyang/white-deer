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

shared_ptr<RenderTexture> Camera::GetPostprocessSrc(int index, int width,
                                                    int height) {
  while (index >= m_postprocessSrcs.size()) {
    m_postprocessSrcs.push_back(RenderTexture::Create(width, height, RT_COLOR));
  }
  auto& ret = m_postprocessSrcs[index];
  ret->Resize(width, height);
  ret->SetFormat(m_enableHDR ? RTF_FLOAT : RTF_AUTO);
  return ret;
}

void Camera::DoPostprocess(RenderTexture& targetRT) {
  if (m_postprocessType == PostprocessType_None && !m_enableGammaCorrection &&
      !m_enableHDR && !m_enableBloom) {
    return;
  }

  auto tempRT0 =
      GetPostprocessSrc(0, targetRT.GetWidth(), targetRT.GetHeight());

  // Bloom
  if (m_enableBloom) {
    // first extract light region
    auto postProcess = Postprocess::GetOrLoad(PostprocessType_ExtractLight);
    postProcess->Render(*tempRT0, targetRT);

    // temp0 --hblur--> temp1
    // temp1 --vblur--> temp0
    auto tempRT1 =
        GetPostprocessSrc(1, targetRT.GetWidth(), targetRT.GetHeight());
    tempRT1->CopyFrom(*tempRT0);
    for (int i = 0; i < m_bloomBlurCount; i++) {
      auto blurh = Postprocess::GetOrLoad(PostprocessType_BlurH);
      auto blurv = Postprocess::GetOrLoad(PostprocessType_BlurV);
      blurh->Render(*tempRT1, *tempRT0);
      blurv->Render(*tempRT0, *tempRT1);
    }

    auto bloom = Postprocess::GetOrLoad(PostprocessType_Bloom);
    bloom->Render(targetRT, targetRT, *tempRT0);
  }

  if (m_postprocessType != PostprocessType_None) {
    // post process
    tempRT0->CopyFrom(targetRT);
    auto postProcess = Postprocess::GetOrLoad(m_postprocessType);
    postProcess->Render(targetRT, *tempRT0);
  }

  // HDR
  if (m_enableHDR) {
    tempRT0->CopyFrom(targetRT);
    auto postProcess = Postprocess::GetOrLoad(PostprocessType_HDR);
    postProcess->Render(targetRT, *tempRT0);
  }

  // gamma correction
  if (m_enableGammaCorrection) {
    tempRT0->CopyFrom(targetRT);
    auto postProcess = Postprocess::GetOrLoad(PostprocessType_Gamma);
    postProcess->Render(targetRT, *tempRT0);
  }
}

}  // namespace Engine
}  // namespace WhiteDeer
