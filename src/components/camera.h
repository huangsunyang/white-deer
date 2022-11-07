#pragma once

#include <utils/common/singleton.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "components/component.h"
#include "graphics/opengl/glrendertexture.h"
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

  bool EnableFrustrumCulling() { return m_enableFrustrumCulling; }
  void Cull(vector<Renderer*>& renderers);

  void Move(float, float, float);
  void Rotate(float, float);

  bool HasShadowMap() { return m_hasShadowMap; }
  shared_ptr<RenderTexture> GetShadowMap();
  float GetShadowMapRange() { return m_shadowMapRange; }
  int GetPcfCount() { return m_pcfCount; }

  void DoPostprocess(RenderTexture&);
  bool EnableHDR() { return m_enableHDR; }

  template <typename T>
  void Transfer(T* transfer, const string& name) {
    transfer->Transfer("dir", &m_dir);
    transfer->Transfer("fov", &m_fov);
    transfer->Transfer("aspect", &m_aspect);
    transfer->Transfer("speed", &m_speed);
    transfer->Transfer("frustrum culling", &m_enableFrustrumCulling);
    transfer->Transfer("postprocess", &m_postprocessType);
    transfer->Transfer("gamma correction", &m_enableGammaCorrection);
    transfer->Transfer("HDR", &m_enableHDR);
    transfer->Transfer("bloom", &m_enableBloom);
    transfer->Transfer("bloom blur", &m_bloomBlurCount);
    transfer->Transfer("enable shadowmap", &m_hasShadowMap);
    if (m_hasShadowMap) {
      transfer->Transfer("enable pcf", &m_pcfCount);
      transfer->Transfer("shadowmap width", &m_shadowMapWidth);
      transfer->Transfer("shadowmap height", &m_shadowMapHeight);
      transfer->Transfer("shadowmap range", &m_shadowMapRange);
    }
  }

 protected:
  shared_ptr<RenderTexture> GetPostprocessSrc(int idx, int width, int height);

  glm::vec3 m_dir = {0, 0, -1};
  float m_fov = 45.0f;
  float m_aspect = 1920.0f / 1080.0f;
  float m_speed = 0.1f;

  // culling
  bool m_enableFrustrumCulling;

  // shadow map
  bool m_hasShadowMap = true;
  bool m_pcfCount = 1;
  shared_ptr<RenderTexture> m_shadowMap = nullptr;
  int m_shadowMapWidth = 1024;
  int m_shadowMapHeight = 1024;
  float m_shadowMapRange = 4.0f;

  // postprocess
  bool m_enableHDR = false;
  bool m_enableBloom = false;
  int m_bloomBlurCount = 0;
  bool m_enableGammaCorrection = true;
  vector<shared_ptr<RenderTexture>> m_postprocessSrcs;
  PostprocessType m_postprocessType{PostprocessType_None};
};

}  // namespace Engine
}  // namespace WhiteDeer