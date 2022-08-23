#define NOMINMAX 1
#include <GL/gl3w.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>

#include "Tracy.hpp"
#include "application/application.h"
#include "camera/camera.h"
#include "components/renderer.h"
#include "editor/gui/testwindow.h"
#include "filesystem/filesystemmanager.h"
#include "graphics/opengl/glTexture.h"
#include "graphics/opengl/glmesh.h"
#include "graphics/opengl/glshader.h"
#include "jobsystem/workers.h"
#include "log/log.h"
#include "scene/scene.h"

using namespace WhiteDeer::Graphics;
using namespace WhiteDeer::Engine;
using namespace WhiteDeer::Editor;

class MainApplication : public Application {
 public:
  static Application *GetInstance() {
    static MainApplication app;
    return &app;
  }

  virtual void Start() {
    // everything must happen after Start
    Application::Start();

    auto scene = SceneManager::GetOrLoad("default");
    {
      auto obj = scene->AddChild("bunny");
      auto meshrenderer = obj->AddComponent<MeshRenderer>();
      meshrenderer->SetMesh("package/models/bun_zipper.ply");
      meshrenderer->SetShader("package/shaders/test.vs",
                              "package/shaders/test.fs");
    }

    {
      auto obj = scene->AddChild("f16");
      auto meshrenderer = obj->AddComponent<MeshRenderer>();
      meshrenderer->SetMesh("package/models/f16/f16.obj");
      meshrenderer->SetTexture("package/models/f16/f16s.bmp");
      meshrenderer->SetShader("package/shaders/test_vnt.vs",
                              "package/shaders/test_vnt.fs");
    }

    // todo: camera should be in scene
    Camera *camera = CameraManager::GetInstance()->CreateCamera();
    camera->SetPos({0, 0, 5.0f});
    camera->SetTargetPos({0, 0.1f, 0});
  }
};

int main(int, char **) { MainApplication::GetInstance()->RunForever(); }
