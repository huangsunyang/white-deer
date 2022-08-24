#pragma once

#include "components/component.h"
#include "graphics/opengl/glTexture.h"
#include "graphics/opengl/glshader.h"
#include "graphics/renderloop/renderloop.h"

namespace WhiteDeer {

namespace Graphics {
class Mesh;
}

namespace Engine {

using namespace Graphics;

class Renderer : public Component {
  friend class RenderLoop;

 public:
  virtual shared_ptr<Program> GetShader() { return m_shader; }

  template <typename T>
  void Transfer(T* transfer, const string& name) {}

 protected:
  virtual void Render() {}

  shared_ptr<Texture> m_texture;
  shared_ptr<Mesh> m_mesh;
  shared_ptr<Program> m_shader;
};

class MeshRenderer : public Renderer {
 public:
  template <typename... Args>
  void SetMesh(Args... args) {
    m_mesh = Mesh::GetOrLoad(args...);
  }

  template <typename... Args>
  void SetShader(Args... args) {
    m_shader = Program::Load(args...);
  }

  template <typename... Args>
  void SetTexture(Args... args) {
    m_texture = Texture::GetOrLoad(args...);
  }

 protected:
  virtual void Render();
};

}  // namespace Engine
}  // namespace WhiteDeer