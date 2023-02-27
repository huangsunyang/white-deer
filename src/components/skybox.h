#pragma once

#include "components/component.h"
#include "graphics/opengl/glcubemaptexture.h"
#include "graphics/opengl/glmesh.h"

namespace WhiteDeer {
namespace Engine {

using namespace WhiteDeer::Graphics;

class SkyBox : public Component {
  DECLARE_WHITEDEER_TYPE(SkyBox)

 public:
  SkyBox() { m_mesh = Mesh::GetOrLoad("cube"); }
  auto GetCubeMapTexture() { return m_cubeMapTexture; }
  auto GetProgram() { return m_program; }
  void Render() { m_mesh->Draw(); }

  void SetCubeMap(std::initializer_list<string> args) {
    m_cubeMapTexture = CubeMapTexture::Create(args);
  }

  template <typename... Args>
  void SetProgram(Args... args) {
    m_program = Program::Load(args...);
  }

  template <typename T>
  void Transfer(T* transfer, const string& name) {}

 protected:
  shared_ptr<CubeMapTexture> m_cubeMapTexture;
  shared_ptr<Program> m_program;
  shared_ptr<Mesh> m_mesh;
};

}  // namespace Engine
}  // namespace WhiteDeer