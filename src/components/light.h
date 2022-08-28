#pragma once
#include <memory.h>

#include <glm/glm.hpp>

#include "components/component.h"
#include "graphics/opengl/glshader.h"
#include "utils/common/enum.h"

using std::shared_ptr;
using namespace WhiteDeer::Graphics;

DECALRE_ENUM(LightType, LightType_Direction, LightType_Point, LightType_Spot)

namespace WhiteDeer {
namespace Engine {

class Light : public Component {
  DECLARE_COMPONENT(Light)

 public:
  glm::vec3 GetColor() { return m_color; }
  void SetColor(float r, float g, float b) { m_color = glm::vec3(r, g, b); }

  void Use(shared_ptr<Program>, int);

  template <typename T>
  void Transfer(T* transfer, const string& name) {
    transfer->Transfer("type", &m_type);
    transfer->Transfer("color", &m_color);

    if (m_type != LightType_Direction) {
      transfer->Transfer("constant", &m_constant);
      transfer->Transfer("linear", &m_linear);
      transfer->Transfer("quadratic", &m_quadratic);
      if (m_type == LightType_Spot) {
        transfer->Transfer("cutoff", &m_cutoff);
        transfer->Transfer("outer cutoff", &m_outerCutoff);
      }
    }
  }

 protected:
  LightType m_type{LightType_Direction};

  glm::vec3 m_color{1.0f, 1.0f, 1.0f};

  // point light
  float m_constant{1.0f};
  float m_linear{0.09f};
  float m_quadratic{0.032f};

  // spot light
  float m_cutoff{12.5f};
  float m_outerCutoff{20.0f};
};

}  // namespace Engine
}  // namespace WhiteDeer