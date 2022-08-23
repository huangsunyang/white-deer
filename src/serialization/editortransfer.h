#pragma once

#include <imgui.h>

#include <glm/glm.hpp>

#include "components/renderer.h"
#include "components/transform.h"
#include "serialization/transferbase.h"

namespace WhiteDeer {
namespace Engine {

#define TRANSFER_COMPONENT(name, type_)  \
  if (comp->IsInstanceOf<type_>()) {     \
    Transfer(name, comp->Cast<type_>()); \
    return;                              \
  }

class EditorTransfer {
 public:
  void TransferComponent(const char* name, Component* comp) {
    TRANSFER_COMPONENT(name, Transform);
    TRANSFER_COMPONENT(name, Renderer);
  }

  template <typename T>
  void Transfer(const char* name, T* data) {
    TransferHelper(name, data);
  }

  template <typename T>
  void TransferBaseType(const string& name, T* data) {}

  template <>
  void TransferBaseType<glm::vec3>(const string& name, glm::vec3* data) {
    ImGui::DragFloat3(name.c_str(), &data->x, 0.05f, -10000.0f, 10000.0f, "%.2f", 1.0f);
  }

 protected:
  template <typename T>
  void TransferHelper(const char* name, T* data) {
    data->Transfer(this, name);
  }

  TRANSFER_SPECIALIZATION(int)
  TRANSFER_SPECIALIZATION(float)
  TRANSFER_SPECIALIZATION(bool)
  TRANSFER_SPECIALIZATION(std::string)
  TRANSFER_SPECIALIZATION(glm::vec3)
};

}  // namespace Engine
}  // namespace WhiteDeer