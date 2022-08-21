#pragma once

#include <glm/glm.hpp>

#include "components/component.h"

namespace WhiteDeer {
namespace Engine {

class Transform : public Component {
 public:
  void SetPosition(float, float, float);
  void SetRotation(float, float, float);
  glm::mat4 GetTranslationMatrix();
  glm::mat4 GetRotationMatrix();
  glm::mat4 GetModelMatrix();

 protected:
  glm::vec3 m_position;
  glm::vec3 m_rotation;
};

}  // namespace Engine

}  // namespace WhiteDeer
