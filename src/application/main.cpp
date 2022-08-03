#include <GL/gl3w.h>

#include "application/application.h"
#include "jobsystem/workers.h"

using WhiteDeer::Engine::Application;
using WhiteDeer::Engine::Job;
using WhiteDeer::Engine::WorkerGroup;

class MainApplication : public Application {
  virtual void Render() { DrawTriangle(); }

  void DrawTriangle() {
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    };

    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
                 g_vertex_buffer_data, GL_STATIC_DRAW);

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
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0,
                 3);  // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
  }
};

int main(int, char **) {
  WorkerGroup group("test-worker", 4);
  group.ScheduleJob(
      [](void *i, int index) {
        using namespace std::chrono_literals;
        printf("hello world %d in thread %zd\n", index,
               std::this_thread::get_id());
        std::this_thread::sleep_for(3000ms);
      },
      nullptr, 10, [](void *data) { printf("all jobs done!\n"); });
  //   using namespace std::chrono_literals;
  //   std::this_thread::sleep_for(300ms);
//   group.StopAllWorkers();
  MainApplication::GetInstance().RunForever();
}
