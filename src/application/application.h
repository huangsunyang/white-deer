#pragma once

#include "utils/common/singleton.h"
#include "graphics/opengl/glframebuffer.h"
#include "graphics/opengl/glshader.h"
#include "graphics/opengl/glmesh.h"

struct GLFWwindow;

namespace WhiteDeer {
namespace Engine {
using namespace WhiteDeer::Utils;
using namespace WhiteDeer::Graphics;

class Application : public Singleton<Application> {
public:
  void RunForever();
  GLFWwindow *GetWindow() { return m_window; }
  int GetWindowWidth() { return m_width; }
  int GetWindowHeight() { return m_height; }

protected:
  virtual void Render() {};

  void MainLoop();
  void RenderImGui();

  bool InitGlewEnv();
  bool InitGlw3Env();
  bool InitImGui();
  void InitInput();

  virtual void Start();
  virtual void Terminate();

  GLFWwindow *m_window;
  shared_ptr<FrameBuffer> m_framebuffer;

  int m_width = 1920;
  int m_height = 1080;
};

} // namespace Engine
}; // namespace WhiteDeer