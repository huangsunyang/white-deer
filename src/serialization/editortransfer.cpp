#include "serialization/editortransfer.h"

#include "components/component.h"

namespace WhiteDeer {
namespace Engine {

void EditorTransfer::TransferComponent(const string& name, Component* comp) {
  if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Framed)) {
    comp->VirtualTransfer(this, name);
    ImGui::TreePop();
  }
}

}  // namespace Engine
}  // namespace WhiteDeer