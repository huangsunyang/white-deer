#include "components/renderer.h"

#include "graphics/opengl/glmesh.h"
#include "scene/gameobject.h"

namespace WhiteDeer {
namespace Engine {

AABB Renderer::GetLocalAABB() { return m_mesh->GetAABB(); }

AABB Renderer::GetWorldAABB() {
  auto localAABB = GetLocalAABB();
  auto transform = m_gameobject->GetComponent<Transform>();
  auto model = transform->GetModelMatrix();
  auto point = localAABB.center + localAABB.extent;
  localAABB.center = model * glm::vec4(localAABB.center, 1);
  point = model * glm::vec4(point, 1);
  localAABB.extent = glm::abs(point - localAABB.center);
  return localAABB;
}

void MeshRenderer::Render() { m_mesh->Draw(); }

}  // namespace Engine
}  // namespace WhiteDeer