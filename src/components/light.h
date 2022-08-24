#pragma once
#include <glm/glm.hpp>

#include "components/component.h"

namespace WhiteDeer {
namespace Engine {

enum LightType {
  POINT,
};

class Light : public Component {
 public:
  float GetStrength() { return m_strength; }
  void SetStrength(float v) { m_strength = v; }
  glm::vec3 GetColor() { return m_color; }
  void SetColor(float r, float g, float b) { m_color = glm::vec3(r, g, b); }

  template <typename T>
  void Transfer(T* transfer, const string& name) {
    transfer->Transfer("color", &m_color);
    transfer->Transfer("strength", &m_strength);
  }

 protected:
  glm::vec3 m_color{1.0f, 1.0f, 1.0f};
  float m_strength{1.0f};
};

}  // namespace Engine
}  // namespace WhiteDeer