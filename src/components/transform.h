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
  glm::mat4 GetTranslationMatrix();
  glm::mat4 GetRotationMatrix();
  glm::mat4 GetModelMatrix();

  template <typename T>
  void Transfer(const T* transfer, const string& name) const {
    transfer->Transfer("position", m_position);
    transfer->Transfer("rotataion", m_rotataion);
  }

 protected:
  glm::vec3 m_position;
  glm::vec3 m_rotation;
};

}  // namespace Engine

}  // namespace WhiteDeer
