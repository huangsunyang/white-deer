#include "components/renderer.h"

#include "graphics/opengl/glmesh.h"

namespace WhiteDeer {
namespace Engine {

void MeshRenderer::Render() {
  m_mesh->Draw();
}

}  // namespace Engine
}  // namespace WhiteDeer