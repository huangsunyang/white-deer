
#include "antlr4-runtime.h"
#include "GLSLParser.h"
#include "GLSLLexer.h"
#include "GLSLParserBaseListener.h"

#define NOMINMAX 1
#include <GL/gl3w.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>

#include "Tracy.hpp"
#include "application/application.h"
#include "components/camera.h"
#include "components/light.h"
#include "components/renderer.h"
#include "components/skybox.h"
#include "components/luascript.h"
#include "editor/gui/testwindow.h"
#include "filesystem/filesystemmanager.h"
#include "graphics/opengl/glTexture.h"
#include "graphics/opengl/glmesh.h"
#include "graphics/opengl/glshader.h"
#include "jobsystem/workers.h"
#include "log/log.h"
#include "scene/scene.h"


using namespace antlr4;

using namespace WhiteDeer::Graphics;
using namespace WhiteDeer::Engine;
using namespace WhiteDeer::Editor;

class MyListener: public GLSLParserBaseListener
{
public:
  virtual void enterInit_declarator_list(GLSLParser::Init_declarator_listContext * ctx) override {
    LOGE << ctx->getText();
  }
};

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
      meshrenderer->SetMaterial("package/shaders/test.vs",
                                "package/shaders/test.fs");
    }

    {
      auto obj = scene->AddChild("f16");
      auto meshrenderer = obj->AddComponent<MeshRenderer>();
      meshrenderer->SetMesh("package/models/f16/f16.obj");
      meshrenderer->SetTexture("package/models/f16/f16s.bmp");
      meshrenderer->SetMaterial("package/shaders/test_vnt.shader");
      auto luascript = obj->AddComponent<LuaScript>();
      luascript->SetScript("f16");
    }

    {
      auto obj = scene->AddChild("f161");
      auto meshrenderer = obj->AddComponent<MeshRenderer>();
      meshrenderer->SetMesh("package/models/f16/f16.obj");
      meshrenderer->SetTexture("package/models/f16/f16s.bmp");
      meshrenderer->SetMaterial("package/shaders/test_vnt.vs",
                                "package/shaders/test_vnt.fs");
      auto transform = obj->GetComponent<Transform>();
      transform->SetPosition(-1, 1, -1);
    }

    {
      auto light = scene->AddChild("light");
      auto meshrenderer = light->AddComponent<MeshRenderer>();
      meshrenderer->SetMesh("cube");
      meshrenderer->SetTexture("package/textures/flower-pattern.jpg");
      meshrenderer->SetMaterial("package/shaders/test_vt.vs",
                                "package/shaders/cube.fs");
      auto lightcomp = light->AddComponent<Light>();
      //   auto lightcomp2 = light->AddComponent<Light>();
      auto transform = light->GetComponent<Transform>();
      transform->SetPosition(-2.5f, 1.5f, -3.0f);
      transform->SetRotation(0.5f, 1.2f, 0.0f);
      transform->SetScale(0.1f, 0.1f, 0.5f);
    }

    {
      auto skybox = scene->AddChild("skybox");
      auto skyboxcomp = skybox->AddComponent<SkyBox>();
      skyboxcomp->SetCubeMap({
          "package/textures/skybox/iceriver/posx.jpg",
          "package/textures/skybox/iceriver/negx.jpg",
          "package/textures/skybox/iceriver/posy.jpg",
          "package/textures/skybox/iceriver/negy.jpg",
          "package/textures/skybox/iceriver/posz.jpg",
          "package/textures/skybox/iceriver/negz.jpg",
      });
      skyboxcomp->SetProgram("package/shaders/skybox.vs",
                             "package/shaders/skybox.fs");
    }

    {
      auto object = scene->AddChild("camera");
      auto transform = object->GetComponent<Transform>();
      // currently camera only use transforms position
      // but use camera's own direction
      auto camera = object->AddComponent<Camera>();
      transform->SetPosition(0, 0, 5.0f);
      camera->SetTargetPos({0, 0.1f, 0});
    }

    {
      auto plane = scene->AddChild("plane");
      auto transform = plane->GetComponent<Transform>();
      transform->SetScale(155, 0.1f, 155);
      transform->SetPosition(0, -2, 0);
      auto meshrenderer = plane->AddComponent<MeshRenderer>();
      meshrenderer->SetMesh("cube");
      meshrenderer->SetTexture("package/textures/flower-pattern.jpg");
      meshrenderer->SetMaterial("package/shaders/test_vt.vs",
                                "package/shaders/cube.fs");
    }

    std::fstream stream;
    auto path = GetLocalFileSystem()->ToAbsolute("package/shaders/test_vnt.shader");
    stream.open(path);
    ANTLRInputStream input(stream);
    GLSLLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    GLSLParser parser(&tokens);

    tree::ParseTree *tree = parser.translation_unit();
    auto s = tree->toStringTree(&parser);
    tree::ParseTreeWalker walker;
    walker.walk(new MyListener(), tree);
    LOGE << s;
  }
};

int main(int, char **) { MainApplication::GetInstance()->RunForever(); }
