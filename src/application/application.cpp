#include <cassert>
#include <cstdio>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include "application/application.h"
#include "editor/gui/editor_gui.h"

namespace WhiteDeer {
namespace Engine {

using WhiteDeer::Editor::EditorGUIManager;

void Application::RunForever() {
  Start();

  do {
    MainLoop();
  } while (!glfwWindowShouldClose(m_window));

  Terminate();
}

static void glfw_error_callback(int error, const char *description) {
  // todo: use logger
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool Application::InitGlewEnv() {
  glfwSetErrorCallback(glfw_error_callback);

  if (!glfwInit())
    return false;

  // GL 3.2 + GLSL 130
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

  // Create window with graphics context
  m_window =
      glfwCreateWindow(m_width, m_height, "White Deer Engine", NULL, NULL);
  if (m_window == NULL)
    return false;
  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1); // Enable vsync
  return true;
}

bool Application::InitGlw3Env() {
  if (gl3wInit()) {
    fprintf(stderr, "failed to initialize OpenGL\n");
    return false;
  }
  if (!gl3wIsSupported(3, 2)) {
    fprintf(stderr, "OpenGL 3.2 not supported\n");
    return false;
  }
  printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
         glGetString(GL_SHADING_LANGUAGE_VERSION));
  return true;
}

bool Application::InitImGui() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  const char *glsl_version = "#version 130";
  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  EditorGUIManager::GetInstance().InitEditors();
  return true;
}

void Application::Start() {
  assert(InitGlewEnv());
  assert(InitGlw3Env());
  assert(InitImGui());
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

  Render();

  // render editor gui
  EditorGUIManager::GetInstance().Render();

  glfwGetFramebufferSize(m_window, &m_width, &m_height);
  glViewport(0, 0, m_width, m_height);
  glfwSwapBuffers(m_window);
}

} // namespace Engine

} // namespace WhiteDeer