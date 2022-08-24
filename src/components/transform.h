#pragma once

#include <glm/glm.hpp>
#include <string>

#include "components/component.h"

using std::string;

namespace WhiteDeer {
namespace Engine {

class Transform : public Component {
 public:
  void SetPosition(float, float, float);
  void SetRotation(float, float, float);
  void SetScale(float, float, float);
  glm::mat4 GetTranslationMatrix();
  glm::mat4 GetRotationMatrix();
  glm::mat4 GetScaleMatrix();
  glm::mat4 GetModelMatrix();

  template <typename T>
  void Transfer(T* transfer, const string& name) {
    transfer->Transfer("position", &m_position);
    transfer->Transfer("rotataion", &m_rotation);
    transfer->Transfer("scale", &m_scale);
  }

 protected:
  glm::vec3 m_position;
  glm::vec3 m_rotation;
  glm::vec3 m_scale{1.0f, 1.0f, 1.0f};
};

}  // namespace Engine

}  // namespace WhiteDeer
