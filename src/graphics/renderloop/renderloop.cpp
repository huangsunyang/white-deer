#include "graphics/renderloop/renderloop.h"

#include "camera/camera.h"
#include "components/light.h"
#include "components/renderer.h"
#include "components/transform.h"
#include "editor/gui/gamewindow.h"
#include "graphics/opengl/glframebuffer.h"
#include "scene/scene.h"

namespace WhiteDeer {
namespace Graphics {

using namespace WhiteDeer::Engine;
using namespace WhiteDeer::Editor;

void RenderLoop::DoForwardRenderLoop() {
  // clear default framebuffer
  FrameBuffer::GetInstance()->Bind();
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  // create temp vertex array
  static GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  auto cameras = CameraManager::GetInstance()->GetCameras();
  for (auto camera : cameras) {
    RenderSingleCamera(camera);
  }
}

void RenderLoop::RenderSingleCamera(Camera* camera) {
  // todo: bind custom render texture
  FrameBuffer::GetInstance()->Bind();
  auto colorRT = FrameBuffer::GetInstance()->GetDefaultColorRT();
  camera->SetAspect(colorRT->GetWidth() * 1.0f / colorRT->GetHeight());
  glViewport(0, 0, colorRT->GetWidth(), colorRT->GetHeight());

  auto p_scene = SceneManager::GetCurrentScene();
  auto renderers = p_scene->GetComponentsInChildren<Renderer>();
  auto lights = p_scene->GetComponentsInChildren<Light>();

  for (auto renderer : renderers) {
    // set uniform variables
    auto material = renderer->GetMaterial();
    auto program = material->GetProgram();
    auto transform = renderer->GetGameObject()->GetComponent<Transform>();
    material->Use();
    program->SetUniformMatrix4fv("projection", camera->GetProjectionMatrix());
    program->SetUniformMatrix4fv("view", camera->GetViewMatrix());
    program->SetUniformMatrix4fv("model", transform->GetModelMatrix());
    program->SetUniformTexture("u_texture", *renderer->m_texture);
    program->SetUniform3f("u_viewPos", camera->GetPos());

    // light
    program->SetUniform1i("u_lightNum", int(lights.size()));
    for (int i = 0; i < lights.size(); i++) {
      lights[i]->Use(program, i);
    }

    renderer->Render();
  }
}

}  // namespace Graphics
}  // namespace WhiteDeer
