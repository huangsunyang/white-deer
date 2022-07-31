#pragma once
#include "editor/gui/editor_gui.h"

namespace WhiteDeer {
namespace Editor {
class TestEditor : public EditorGUI {
protected:
  bool m_show_demo_window = true;
  bool m_show_another_window = false;
  ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  virtual void Render();
};
} // namespace Editor
} // namespace WhiteDeer