#pragma once
#include <imgui.h>

#include <glm/glm.hpp>

#include "camera/camera.h"
#include "editor/gui/editor_gui.h"
#include "graphics/opengl/glframebuffer.h"
#include "graphics/opengl/glshader.h"
#include "utils/common/singleton.h"

using std::string;
using std::vector;
using WhiteDeer::Graphics::FrameBuffer;
using WhiteDeer::Utils::Singleton;
using namespace WhiteDeer::Engine;

namespace WhiteDeer {
namespace Editor {

class GameWindow : public EditorWindow<GameWindow> {
 public:
  friend class Singleton<GameWindow>;
  virtual void Render() {
    for (int i = 0; i < m_framebuffers.size(); i++) {
      string name = "Game Windows";
      if (i > 0) {
        name += std::to_string(i);
      }
      ImGui::Begin(name.c_str(), &m_showing);
      ImVec2 wsize = ImGui::GetWindowSize();

      // todo: move to render loop
      CameraManager::GetInstance()->GetDefaultCamera()->SetAspect(wsize.x / wsize.y);
      m_framebuffers[0]->Resize((int)wsize.x, (int)wsize.y);

      ImGui::Image((ImTextureID)(size_t)m_framebuffers[0]->GetTextureHandle(),
                   wsize, ImVec2(0, 1), ImVec2(1, 0));
      ImGui::End();
    }
  };

  void AddView(shared_ptr<FrameBuffer> f) { m_framebuffers.push_back(f); }

 protected:
  vector<shared_ptr<FrameBuffer>> m_framebuffers;
};

}  // namespace Editor
}  // namespace WhiteDeer