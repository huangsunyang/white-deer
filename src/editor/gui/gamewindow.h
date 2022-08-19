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

      // todo: why always sth. outside window?
      ImGui::Begin(
          name.c_str(), &m_showing,
          ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
      ImVec2 wsize = ImGui::GetWindowSize();

      // todo: move to render loop
      Camera* camera = CameraManager::GetInstance()->GetDefaultCamera();
      ProcessInput(camera);
      camera->SetAspect(wsize.x / wsize.y);
      m_framebuffers[0]->Resize((int)wsize.x, (int)wsize.y);

      ImGui::Image((ImTextureID)(size_t)m_framebuffers[0]->GetTextureHandle(),
                   wsize, ImVec2(0, 1), ImVec2(1, 0));
      ImGui::End();
    }
  };

  void AddView(shared_ptr<FrameBuffer> f) { m_framebuffers.push_back(f); }

 protected:
  void ProcessInput(Camera* camera) {
    ImVec2 wsize = ImGui::GetWindowSize();
    auto io = ImGui::GetIO();

    // rotate camera
    auto left = ImGuiMouseButton_Left;
    if (io.MouseDown[left]) {
      camera->Rotate(-io.MouseDelta.x / wsize.x * 2, io.MouseDelta.y / wsize.y * 2);
    }

    // move camera x y direction
    auto middle = ImGuiMouseButton_Middle;
    if (io.MouseDown[middle]) {
      LOGE << io.MouseDelta.x << ' ' << io.MouseDelta.y;
      camera->Move(io.MouseDelta.x / wsize.x * 20, io.MouseDelta.y / wsize.y * 20, 0);
    }

    // move camera back and forth
    if (io.MouseWheel != 0) {
      camera->Move(0, 0, io.MouseWheel * 2);
    }
  }

  vector<shared_ptr<FrameBuffer>> m_framebuffers;
};

}  // namespace Editor
}  // namespace WhiteDeer