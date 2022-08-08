#define NOMINMAX 1
#include <GL/gl3w.h>

#include "Tracy.hpp"
#include "application/application.h"
#include "filesystem/filesystemmanager.h"
#include "jobsystem/workers.h"
#include "log/log.h"
#include "graphics/opengl/glshader.h"

using WhiteDeer::Engine::Application;
using WhiteDeer::Engine::FileSystemManager;
using WhiteDeer::Engine::Job;
using WhiteDeer::Engine::WorkerGroup;
using namespace WhiteDeer::Graphics;

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

  GLuint vertexbuffer;
  shared_ptr<Program> p_program;

  void PrepareTriangle() {
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    };

    // This will identify our vertex buffer
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
                 g_vertex_buffer_data, GL_STATIC_DRAW);

    p_program = Program::Load("package/shaders/test.vs", "package/shaders/test.fs");
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
    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0,  // attribute 0. No particular reason for 0, but
                              // must match the layout in the shader.
                          3,  // size
                          GL_FLOAT,  // type
                          GL_FALSE,  // normalized?
                          0,         // stride
                          (void *)0  // array buffer offset
    );

    Wait1();
    Wait2();

    // Draw the triangle !
    // Starting from vertex 0; 3 vertices total -> 1 triangle
    p_program->Use();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
  }
};

int main(int, char **) { MainApplication::GetInstance()->RunForever(); }
