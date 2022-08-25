#pragma once

#include <glm/glm.hpp>
#include <string>

#include "components/component.h"

using std::string;

namespace WhiteDeer {
namespace Engine {

class Transform : public Component {
 public:
  void SetPosition(float x, float y, float z) { m_position = {x, y, z}; }
  void SetRotation(float x, float y, float z) { m_rotation = {x, y, z}; }
  void SetScale(float x, float y, float z) { m_scale = {x, y, z}; }

  glm::mat4 GetTranslationMatrix();
  glm::mat4 GetRotationMatrix();
  glm::mat4 GetScaleMatrix();
  glm::mat4 GetModelMatrix();

  glm::vec3 GetXDirection();
  glm::vec3 GetYDirection();
  glm::vec3 GetZDirection();

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
