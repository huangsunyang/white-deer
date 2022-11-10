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

void MeshRenderer::Render() {
  m_mesh->Draw();

  if (m_showBoundingBox) {
    if (!m_boundingMesh.get()) {
      m_boundingMesh = Mesh::GetOrLoad("cube");
    }

    // hack
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    auto transform = m_gameobject->GetComponent<Transform>();
    auto modelMatrix = transform->GetModelMatrix() * glm::scale(glm::mat4(1.0f), m_mesh->GetAABB().extent * 2.0f);
    m_material->GetProgram()->SetUniformMatrix4fv("model", modelMatrix);
    m_boundingMesh->Draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

}  // namespace Engine
}  // namespace WhiteDeer