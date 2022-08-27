#pragma once

#include <imgui.h>

#include <glm/glm.hpp>

#include "components/light.h"
#include "components/renderer.h"
#include "components/transform.h"
#include "serialization/transferbase.h"

namespace WhiteDeer {
namespace Engine {

#define TRANSFER_COMPONENT(name, type_)    \
  if (comp->IsInstanceOf<type_>()) {       \
    if (ImGui::CollapsingHeader(#type_)) { \
      Transfer(name, comp->Cast<type_>()); \
    }                                      \
    return;                                \
  }

static const float FLOAT_MIN = -10000.0f;
static const float FLOAT_MAX = 10000.0f;
static const float FLOAT_STEP = 0.05f;

class EditorTransfer {
 public:
  void TransferComponent(const string& name, Component* comp) {
    TRANSFER_COMPONENT(name, Transform);
    TRANSFER_COMPONENT(name, Renderer);
    TRANSFER_COMPONENT(name, Light);
  }

  template <typename T>
  void Transfer(const string& name, T* data) {
    if constexpr(std::is_enum_v<T>) {
      auto itemstrings = GetEnumItems<T>();
      vector<const char *> items;
      for (auto& v : itemstrings) {
        items.push_back(v.c_str());
      }
      ImGui::Combo(name.c_str(), (int*)data, items.data(), (int)items.size());
    } else {
      data->Transfer(this, name);
    }
  }

  template <>
  void Transfer<float>(const string& name, float* data) {
    ImGui::DragFloat(name.c_str(), data, FLOAT_STEP, FLOAT_MIN, FLOAT_MAX,
                     "%.2f", 1.0f);
  }

  template <>
  void Transfer<glm::vec3>(const string& name, glm::vec3* data) {
    if (name.find("color") != string::npos) {
      ImGui::ColorEdit3(name.c_str(), &data->x, ImGuiColorEditFlags_Float);
    } else {
      ImGui::DragFloat3(name.c_str(), &data->x, FLOAT_STEP, FLOAT_MIN,
                        FLOAT_MAX, "%.2f", 1.0f);
    }
  }
};

}  // namespace Engine
}  // namespace WhiteDeer