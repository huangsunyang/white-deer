#pragma once
#include <imgui.h>

#include <glm/glm.hpp>

#include "components/camera.h"
#include "editor/gui/editorgui.h"
#include "graphics/opengl/glrendertexture.h"
#include "graphics/opengl/glshader.h"
#include "scene/scene.h"
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

  float GetWidth() { return m_width; }
  float GetHeight() { return m_height; }

  virtual void Render() {
    string name = "Game Windows";
    ImGui::Begin(
        name.c_str(), &m_showing,
        ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);

    // todo: move to render loop
    auto p_scene = SceneManager::GetCurrentScene();
    auto cameras = p_scene->GetComponentsInChildren<Camera>();
    if (cameras.size() > 0) {
      ProcessInput(cameras[0]);
    }

    // todo: why always sth. outside window?
    ImVec2 wsize = ImGui::GetWindowSize();
    m_width = wsize.x;
    m_height = wsize.y;
    auto colorRT = FrameBuffer::GetDefault()->GetDefaultColorRT();
    ImGui::Image((ImTextureID)(size_t)colorRT->GetHandle(), wsize, ImVec2(0, 1),
                 ImVec2(1, 0));
    ImGui::End();

    if (cameras[0]->HasShadowMap()) {
      ImGui::Begin("shadowmap", &m_showing);
      auto shadowMap = cameras[0]->GetShadowMap();
      ImGui::Image((ImTextureID)(size_t)shadowMap->GetHandle(),
                   ImVec2((float)shadowMap->GetWidth(), (float)shadowMap->GetHeight()),
                   ImVec2(0, 1), ImVec2(1, 0));
      ImGui::End();
    }
  };

 protected:
  void ProcessInput(Camera* camera) {
    if (!ImGui::IsWindowFocused()) {
      return;
    }

    ImVec2 wsize = ImGui::GetWindowSize();
    auto io = ImGui::GetIO();

    // rotate camera
    auto left = ImGuiMouseButton_Left;
    if (io.MouseDown[left]) {
      camera->Rotate(-io.MouseDelta.x / wsize.x * 2,
                     io.MouseDelta.y / wsize.y * 2);
    }

    // move camera x y direction
    auto middle = ImGuiMouseButton_Middle;
    if (io.MouseDown[middle]) {
      camera->Move(io.MouseDelta.x / wsize.x * 20,
                   io.MouseDelta.y / wsize.y * 20, 0);
    }

    // move camera back and forth
    if (io.MouseWheel != 0) {
      camera->Move(0, 0, io.MouseWheel * 2);
    }
  }

  float m_width;
  float m_height;
};

}  // namespace Editor
}  // namespace WhiteDeer