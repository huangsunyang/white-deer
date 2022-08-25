#pragma once
#include "graphics/opengl/glshader.h"

namespace WhiteDeer {
namespace Graphics {

class Material {
 public:
  template <typename... Args>
  Material(const Args... paths) {
    m_program = Program::Load(paths...);
  }
  shared_ptr<Program> GetProgram() { return m_program; }

  void Use() {
    m_program->Use();
    m_program->SetUniform3f("u_material.ambient", m_ambient);
    m_program->SetUniform3f("u_material.diffuse", m_diffuse);
    m_program->SetUniform3f("u_material.specular", m_specular);
    m_program->SetUniform1f("u_material.shininess", m_shininess);
  }

  template <typename T>
  void Transfer(T* transfer, const string& name) {
    transfer->Transfer("ambient", &m_ambient);
    transfer->Transfer("diffuse", &m_diffuse);
    transfer->Transfer("specular", &m_specular);
    transfer->Transfer("shininess", &m_shininess);
  }

 protected:
  glm::vec3 m_ambient{0.1f, 0.1f, 0.1f};
  glm::vec3 m_diffuse{1.0f, 1.0f, 1.0f};
  glm::vec3 m_specular{1.0f, 1.0f, 1.0f};
  float m_shininess{64.0f};

  shared_ptr<Program> m_program;
};

}  // namespace Graphics
}  // namespace WhiteDeer