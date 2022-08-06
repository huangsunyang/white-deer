#pragma once
#include <imgui.h>

#include <mutex>
#include <string>

#include "editor/gui/editor_gui.h"
#include "log/log.h"

namespace WhiteDeer {
namespace Editor {

using std::mutex;

struct ExampleAppLog {
  ImGuiTextBuffer Buf;
  ImGuiTextFilter Filter;
  ImVector<int> LineOffsets;  // Index to lines offset. We maintain this with
                              // AddLog() calls.
  bool AutoScroll;            // Keep scrolling if already at the bottom.

  ExampleAppLog() : AutoScroll(true) { Clear(); }
  void Clear();
  void AddLog(const char* fmt, ...);
  void Draw(const char* title, bool* p_open = NULL);
};

class LogWindow : public EditorGUI {
  friend class Singleton<LogWindow>;

 public:
  static ExampleAppLog* GetLogger() { return &log; }

 protected:
  LogWindow();
  static ExampleAppLog log;
  virtual void Render();
};

template <typename Formatter>
class ImGuiLogAppender : public plog::IAppender {
 public:
  virtual void write(const plog::Record& record) {
    ExampleAppLog* imguiLog = LogWindow::GetLogger();
    plog::util::nstring wstr = Formatter::format(record);
    std::string str(wstr.begin(), wstr.end());
    std::lock_guard lockgard(m_mtx);
    imguiLog->AddLog(str.c_str());
  }

 protected:
  mutex m_mtx;
};
}  // namespace Editor
}  // namespace WhiteDeer