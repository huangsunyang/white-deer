#pragma once
#include <imgui.h>

namespace WhiteDeer {
namespace Editor {

struct TextScroll {
  ImGuiTextBuffer Buf;
  ImGuiTextFilter Filter;
  ImVector<int> LineOffsets;  // Index to lines offset. We maintain this with
                              // AddLog() calls.
  bool AutoScroll;            // Keep scrolling if already at the bottom.

  TextScroll() : AutoScroll(true) { Clear(); }
  void Clear();
  void AddLog(const char* fmt, ...);
  void Draw(const char* title, bool* p_open = NULL);
};

}  // namespace Editor
}  // namespace WhiteDeer