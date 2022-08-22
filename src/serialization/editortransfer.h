#pragma once

#include <imgui.h>

#include "serialization/transferbase.h"

namespace WhiteDeer {
namespace Engine {

class EditorTransfer : public TransferBase {
 public:
  template <int>
  void TransferBaseType(const string& name, const int* data) {}
};

}  // namespace Engine
}  // namespace WhiteDeer