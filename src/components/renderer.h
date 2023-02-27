#pragma once

#include "components/component.h"
#include "graphics/opengl/glTexture.h"
#include "graphics/opengl/glmaterial.h"
#include "graphics/renderloop/renderloop.h"
#include "utils/common/physics.h"

namespace WhiteDeer {

namespace Graphics {
class Mesh;
}

namespace Engine {

using namespace Graphics;
using namespace Utils;

class Renderer : public Component {
  DECLARE_WHITEDEER_TYPE(Renderer)
  friend class RenderLoop;

 public:
  virtual shared_ptr<Material> GetMaterial() { return m_material; }
  virtual AABB GetLocalAABB();
  virtual AABB GetWorldAABB();

  template <typename T>
  void Transfer(T* transfer, const string& name) {
    transfer->Transfer("show bounding box", &m_showBoundingBox);
    transfer->Transfer("material", m_material.get());
  }

 protected:
  virtual void Render() {}

  bool m_showBoundingBox = true;

  shared_ptr<Texture> m_texture;
  shared_ptr<Mesh> m_mesh;
  shared_ptr<Mesh> m_boundingMesh;
  shared_ptr<Material> m_material;
};

class MeshRenderer : public Renderer {
  DECLARE_WHITEDEER_TYPE(MeshRenderer)

 public:
  template <typename... Args>
  void SetMesh(Args... args) {
    m_mesh = Mesh::GetOrLoad(args...);
  }

  template <typename... Args>
  void SetMaterial(Args... args) {
    m_material = shared_ptr<Material>(new Material(args...));
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