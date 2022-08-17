#define NOMINMAX 1
#include <GL/gl3w.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>

#include "Tracy.hpp"
#include "application/application.h"
#include "camera/camera.h"
#include "editor/gui/testwindow.h"
#include "filesystem/filesystemmanager.h"
#include "graphics/opengl/glTexture.h"
#include "graphics/opengl/glmesh.h"
#include "graphics/opengl/glshader.h"
#include "jobsystem/workers.h"
#include "log/log.h"

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

    PrepareTriangle();
    WorkerGroup::GetJobWorkers()->ScheduleJob(
        [](void *i, int index) {
          ZoneScoped;
          using namespace std::chrono_literals;
          LOGD.printf("hello world 第%d in thread %d", index,
                      std::this_thread::get_id());
          //   std::this_thread::sleep_for(10ms);
        },
        nullptr, 10,
        [](void *data) {
          LOGE.printf("all jobs done!");
          FileSystemManager::GetInstance();
        });
  }

  virtual void Render() {
    FrameMark("Render");
    DrawTriangle();
  }

  GLuint VertexArrayID;
  GLuint VertexArrayID2;
  unsigned int EBO;
  Camera *camera;

  GLuint vertexbuffer;
  GLuint vertexbuffer2;
  shared_ptr<Program> p_program;
  shared_ptr<Program> p_program_vnt;
  shared_ptr<Texture> p_texture;
  shared_ptr<Texture> p_texture_f16;
  shared_ptr<Mesh> p_mesh;
  shared_ptr<Mesh> p_mesh_obj;

  void PrepareTriangle() {
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    p_mesh = Mesh::GetOrLoad("package/models/bun_zipper.ply");
    p_mesh_obj = Mesh::GetOrLoad("package/models/f16/f16.obj");
    p_program =
        Program::Load("package/shaders/test.vs", "package/shaders/test.fs");
    p_program_vnt =
        Program::Load("package/shaders/test_vnt.vs", "package/shaders/test_vnt.fs");
    p_texture = Texture::GetOrLoad("package/textures/flower-pattern.jpg");
    p_texture_f16 = Texture::GetOrLoad("package/models/f16/f16s.bmp");
    camera = CameraManager::GetInstance()->CreateCamera();
    camera->SetPos({0, 0, 2.0f});
    camera->SetTargetPos({0, 0.1f, 0});
  }

  void Wait1() {
    ZoneScoped;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1ms);
  }

  void Wait2() {
    ZoneScoped;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1ms);
  }

  void DrawTriangle() {
    ZoneScoped;
    glBindVertexArray(VertexArrayID);

    p_program->Use();
    // p_program->SetUniformTexture("myTexture", *p_texture);

    auto testWindow = TestWindow::GetInstance();
    p_program->SetUniformMatrix4fv("projection", camera->GetProjectionMatrix());
    p_program->SetUniformMatrix4fv("view", camera->GetViewMatrix());
    p_program->SetUniformMatrix4fv("model", glm::translate(glm::mat4(1.0f), glm::vec3(1,0,0)));

    p_mesh->Draw();

    glBindVertexArray(VertexArrayID);
    p_program_vnt->Use();
    p_program_vnt->SetUniformMatrix4fv("projection", camera->GetProjectionMatrix());
    p_program_vnt->SetUniformMatrix4fv("view", camera->GetViewMatrix());
    p_program_vnt->SetUniformMatrix4fv("model", glm::mat4(1.0f));
    p_program_vnt->SetUniformTexture("u_texture", *p_texture_f16);

    p_mesh_obj->Draw();
  }
};

int main(int, char **) { MainApplication::GetInstance()->RunForever(); }
