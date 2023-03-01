#include "application/application.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <cassert>
#include <cstdio>

#include "components/camera.h"
#include "editor/gui/editorgui.h"
#include "editor/gui/gamewindow.h"
#include "filesystem/filesystemmanager.h"
#include "graphics/renderloop/renderloop.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "log/log.h"
#include "ticktime/timemanager.h"
#include "physics/physicsmanager.h"

namespace WhiteDeer {
namespace Engine {

using WhiteDeer::Editor::EditorGUIManager;
using WhiteDeer::Editor::GameWindow;

void Application::RunForever() {
  Start();

  do {
    MainLoop();
  } while (!glfwWindowShouldClose(m_window));

  Terminate();
}

static void glfw_error_callback(int error, const char *description) {
  LOGE.printf("Glfw Error %d: %s", error, description);
}

bool Application::InitGlewEnv() {
  glfwSetErrorCallback(glfw_error_callback);

  if (!glfwInit()) return false;

  // GL 3.2 + GLSL 130
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

  // Create window with graphics context
  m_window =
      glfwCreateWindow(m_width, m_height, "White Deer Engine", NULL, NULL);
  if (m_window == NULL) return false;
  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1);  // Enable vsync
  return true;
}

bool Application::InitGlw3Env() {
  if (gl3wInit()) {
    LOGE.printf("failed to initialize OpenGL");
    return false;
  }
  if (!gl3wIsSupported(3, 2)) {
    LOGE.printf("OpenGL 3.2 not supported");
    return false;
  }
  LOGD.printf("OpenGL %s, GLSL %s", glGetString(GL_VERSION),
              glGetString(GL_SHADING_LANGUAGE_VERSION));
  return true;
}

bool Application::InitImGui() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup Platform/Renderer backends
  const char *glsl_version = "#version 130";
  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  EditorGUIManager::GetInstance()->InitEditors();
  return true;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {}

void keyboard_callback() {}

void Application::InitInput() {
  // glfwSetCursorPosCallback(m_window, mouse_callback);
}

void Application::Start() {
  assert(InitGlewEnv());
  assert(InitGlw3Env());
  assert(InitImGui());

  InitInput();
}

void Application::Terminate() {
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void Application::MainLoop() {
  glfwPollEvents();
  TimeManager::GetInstance()->Tick();

  SceneManager::GetCurrentScene()->Update();

  PhysicsManager::GetInstance()->Update();

  // todo: render to be tick func
  RenderLoop::DoRenderLoop();

  // clear color and render editor gui
  FrameBuffer::GetDefault()->UnBind();
  //   glfwGetFramebufferSize(m_window, &m_width, &m_height);
  //   glDisable(GL_DEPTH_TEST);
  GLfloat whiteColor[3] = {0.0, 0.0, 0.0};
  glClearBufferfv(GL_COLOR, 0, whiteColor);
  EditorGUIManager::GetInstance()->Render();

  glfwSwapBuffers(m_window);
}

}  // namespace Engine

}  // namespace WhiteDeer