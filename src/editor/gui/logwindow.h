#pragma once
#include <imgui.h>
#include <plog/Formatters/TxtFormatter.h>

#include <codecvt>
#include <cstdlib>
#include <locale>
#include <mutex>
#include <string>

#include "editor/gui/editor_gui.h"
#include "editor/gui/textscroll.h"
#include "log/log.h"

namespace WhiteDeer {
namespace Editor {

using std::mutex;

template <typename Formatter>
class ImGuiLogAppender : public plog::IAppender {
 public:
  virtual void write(const plog::Record& record) {
    TextScroll* imguiLog = LogWindow::GetInstance()->GetLogger();
    plog::util::nstring wstr = Formatter::format(record);
    std::string str = plog::util::toNarrow(wstr, 0);
    std::lock_guard lockgard(m_mtx);
    imguiLog->AddLog(str.c_str());
  }

 protected:
  mutex m_mtx;
};

class LogWindow : public EditorWindow<LogWindow> {
  friend class Singleton<LogWindow>;

 public:
  TextScroll* GetLogger() { return &m_logscroll; }

 protected:
  LogWindow() {
    static ImGuiLogAppender<plog::TxtFormatter> appender;
    plog::init(plog::debug, &appender);
  }

  virtual void Render() {
    if (!m_showing) {
      return;
    }

    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of a rarely used feature: multiple calls to
    // Begin()/End() are appending to the _same_ window. Most of the contents of
    // the window will be added by the log.Draw() call.
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Log Window", &m_showing);
    if (ImGui::SmallButton("[Debug] Add 5 entries")) {
      static int counter = 0;
      const char* categories[3] = {"info", "warn", "error"};
      const char* words[] = {"Bumfuzzled",    "Cattywampus",  "Snickersnee",
                             "Abibliophobia", "Absquatulate", "Nincompoop",
                             "Pauciloquent"};
      for (int n = 0; n < 5; n++) {
        const char* category = categories[counter % IM_ARRAYSIZE(categories)];
        const char* word = words[counter % IM_ARRAYSIZE(words)];
        m_logscroll.AddLog(
            "[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
            ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
        counter++;
      }
    }
    ImGui::End();

    // Actually call in the regular Log helper (which will Begin() into the same
    // window as we just did)
    m_logscroll.Draw("Log Window", &m_showing);
  }

  TextScroll m_logscroll;
};

}  // namespace Editor
}  // namespace WhiteDeer