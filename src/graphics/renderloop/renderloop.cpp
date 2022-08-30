#include "graphics/renderloop/renderloop.h"

#include "components/camera.h"
#include "components/light.h"
#include "components/renderer.h"
#include "components/skybox.h"
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
  FrameBuffer::GetDefault()->Bind();
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  // create temp vertex array
  static GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  auto p_scene = SceneManager::GetCurrentScene();
  auto cameras = p_scene->GetComponentsInChildren<Camera>();
  for (auto camera : cameras) {
    RenderSingleCamera(camera);
  }
}

void RenderLoop::RenderSingleCamera(Camera* camera) {
  auto p_scene = SceneManager::GetCurrentScene();
  auto renderers = p_scene->GetComponentsInChildren<Renderer>();
  auto lights = p_scene->GetComponentsInChildren<Light>();
  auto skyboxs = p_scene->GetComponentsInChildren<SkyBox>();

  // find a directional light
  Light* light = nullptr;
  for (auto& _light : lights) {
    if (_light->GetType() == LightType_Direction) {
      light = _light;
    }
  }

  glm::mat4 lightMatrix(1.0f);
  bool hasShadowMap = light != nullptr && camera->HasShadowMap();
  // shadow map
  if (hasShadowMap) {
    auto shadowMap = camera->GetShadowMap();
    auto lightTransform = light->GetGameObject()->GetComponent<Transform>();
    auto range = camera->GetShadowMapRange();
    auto lightProjection = glm::ortho(-range, range, -range, range, 0.01f, 10.0f);
    lightMatrix = lightProjection * lightTransform->GetViewMatrix();
    FrameBuffer::GetOrLoad("shadowmap")->BindDepth(*shadowMap);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, shadowMap->GetWidth(), shadowMap->GetHeight());
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    for (auto renderer : renderers) {
      auto transform = renderer->GetGameObject()->GetComponent<Transform>();
      // todo: constant shadow map texture
      static shared_ptr<Program> program = Program::Load("package/shaders/shadowmap.vs", "package/shaders/shadowmap.fs");
      program->Use();
      program->SetUniformMatrix4fv("lightMatrix", lightMatrix);
      program->SetUniformMatrix4fv("model", transform->GetModelMatrix());
      renderer->Render();
    }
  }

  // todo: bind custom render texture
  FrameBuffer::GetDefault()->UnBind();
  FrameBuffer::GetDefault()->Bind();
  auto colorRT = FrameBuffer::GetDefault()->GetDefaultColorRT();
  camera->SetAspect(colorRT->GetWidth() * 1.0f / colorRT->GetHeight());
  glViewport(0, 0, colorRT->GetWidth(), colorRT->GetHeight());

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  for (auto renderer : renderers) {
    // set uniform variables
    auto material = renderer->GetMaterial();
    auto program = material->GetProgram();
    auto transform = renderer->GetGameObject()->GetComponent<Transform>();
    material->Use();
    program->SetUniformMatrix4fv("projection", camera->GetProjectionMatrix());
    program->SetUniformMatrix4fv("view", camera->GetViewMatrix());
    program->SetUniformMatrix4fv("model", transform->GetModelMatrix());
    program->SetUniform3f("u_viewPos", camera->GetPosition());
    program->SetUniformTexture("u_texture", *renderer->m_texture);
    program->SetUniform1i("u_hasShadowMap", hasShadowMap);
    program->SetUniform1i("u_pcfCount", camera->GetPcfCount());
    program->SetUniformMatrix4fv("lightMatrix", lightMatrix);
    if (hasShadowMap) {
      program->SetUniformTexture("u_shadowMap", *camera->GetShadowMap(), 1);
    }
    if (skyboxs.size() > 0) {
      program->SetUniformTexture("u_skybox", *skyboxs[0]->GetCubeMapTexture());
    }

    // light
    program->SetUniform1i("u_lightNum", int(lights.size()));
    for (int i = 0; i < lights.size(); i++) {
      lights[i]->Use(program, i);
    }

    renderer->Render();
  }

  // skybox
  if (skyboxs.size() > 0) {
    glDepthFunc(GL_LEQUAL);
    auto skybox = skyboxs[0];
    auto program = skybox->GetProgram();
    program->Use();
    program->SetUniformMatrix4fv("projection", camera->GetProjectionMatrix());
    program->SetUniformMatrix4fv("view",
                                 glm::mat4(glm::mat3(camera->GetViewMatrix())));
    program->SetUniformTexture("u_skybox", *skybox->GetCubeMapTexture());
    skybox->Render();
  }

  // postprocess
  glDisable(GL_DEPTH_TEST);
  camera->DoPostprocess(*colorRT);
}

}  // namespace Graphics
}  // namespace WhiteDeer
