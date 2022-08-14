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
  shared_ptr<Texture> p_texture;

  void PrepareTriangle() {
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // An array of 3 vectors which represents 3 vertices
    float g_vertex_buffer_data[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

    unsigned int indices[] = {
        // 注意索引从0开始!
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形

        0, 1, 3,  // 第一个三角形
        1, 2, 3,  // 第二个三角形
    };

    // This will identify our vertex buffer
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
                 g_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                //  GL_STATIC_DRAW);

    // second draw
    glGenVertexArrays(1, &VertexArrayID2);
    glBindVertexArray(VertexArrayID2);

    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data2[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    };

    // This will identify our vertex buffer
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer2);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data2),
                 g_vertex_buffer_data2, GL_STATIC_DRAW);
    glVertexAttribPointer(0,  // attribute 0. No particular reason for 0, but
                              // must match the layout in the shader.
                          3,  // size
                          GL_FLOAT,  // type
                          GL_FALSE,  // normalized?
                          0,         // stride
                          (void *)0  // array buffer offset
    );

    p_program =
        Program::Load("package/shaders/test.vs", "package/shaders/test.fs");
    p_texture = Texture::Load("package/textures/flower-pattern.jpg");
    camera = CameraManager::GetInstance()->CreateCamera();
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
    glBindVertexArray(VertexArrayID);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // Draw the triangle !
    p_program->Use();
    p_program->SetUniformTexture("myTexture", *p_texture);
    glm::mat4 trans(1.0f);
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(1.0, 0.0, 0.0));
    // trans = glm::scale(trans, glm::vec3(0.1, 0.5, 0.5));

    auto testWindow = TestWindow::GetInstance();
    p_program->SetUniformMatrix4fv("projection", camera->GetProjectionMatrix());
    p_program->SetUniformMatrix4fv("view", camera->GetViewMatrix());
    p_program->SetUniformMatrix4fv("model", glm::mat4(1.0f));

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

    glBindVertexArray(VertexArrayID2);
    p_program->Use();
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
    glEnableVertexAttribArray(0);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
  }
};

int main(int, char **) { MainApplication::GetInstance()->RunForever(); }
