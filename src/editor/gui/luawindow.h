#pragma once

#include "editor/gui/editor_gui.h"

namespace WhiteDeer {
namespace Editor {

class LuaWindow : public EditorGUI {
 public:
  friend class Singleton<LuaWindow>;
  virtual void Render();

 protected:
  LuaWindow() {}
};

}  // namespace Editor
}  // namespace WhiteDeer