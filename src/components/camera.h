#pragma once

#include <utils/common/singleton.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "components/component.h"
#include "graphics/postprocess.h"
#include "graphics/renderloop/renderloop.h"

namespace WhiteDeer {
namespace Engine {

using std::vector;
using WhiteDeer::Utils::Singleton;
using namespace WhiteDeer::Graphics;

class Camera : public Component {
  DECLARE_COMPONENT(Camera)
  friend class WhiteDeer::Graphics::RenderLoop;

 public:
  glm::vec3 GetPosition();
  glm::vec3 GetZDirection();
  glm::vec3 GetYDirection();
  glm::vec3 GetXDirection();
  glm::mat4 GetViewMatrix();
  glm::mat4 GetProjectionMatrix();

  void SetAspect(float aspect) { m_aspect = aspect; }
  void SetTargetPos(glm::vec3 p) { m_dir = glm::normalize(p - GetPosition()); }
  glm::vec3 GetTargetPos() { return m_dir + GetPosition(); }

  void Move(float, float, float);
  void Rotate(float, float);

  void DoPostprocess(const Texture&);

  template <typename T>
  void Transfer(T* transfer, const string& name) {
    transfer->Transfer("dir", &m_dir);
    transfer->Transfer("fov", &m_fov);
    transfer->Transfer("aspect", &m_aspect);
    transfer->Transfer("speed", &m_speed);
    transfer->Transfer("postprocess", &m_postprocessType);
  }

 protected:
  glm::vec3 m_dir = {0, 0, -1};
  float m_fov = 45.0f;
  float m_aspect = 1920.0f / 1080.0f;
  float m_speed = 0.1f;

  PostprocessType m_postprocessType{PostprocessType_Inversion};
};

}  // namespace Engine
}  // namespace WhiteDeer