#pragma once

namespace WhiteDeer {

namespace Engine {
class Camera;
}

namespace Graphics {

class RenderLoop {
 public:
  static void DoRenderLoop() {
    // todo: use config
    if (true) {
      DoForwardRenderLoop();
    } else {
      DoDeferredRenderLoop();
    }
  }

  static void DoForwardRenderLoop();
  static void DoDeferredRenderLoop() {}

 protected:
  static void RenderSingleCamera(Engine::Camera *);
};

}  // namespace Graphics
}  // namespace WhiteDeer