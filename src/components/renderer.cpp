#include "components/renderer.h"

#include "graphics/opengl/glmesh.h"

namespace WhiteDeer {
namespace Engine {

void MeshRenderer::Render() {
  if (m_mesh->m_useebo) {
    glBindBuffer(GL_ARRAY_BUFFER, m_mesh->m_vbo);
    m_mesh->setVertexAttrib();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh->m_ebo);
    glDrawElements(GL_TRIANGLES, m_mesh->m_vertexcount, GL_UNSIGNED_INT, 0);
  } else {
    glBindBuffer(GL_ARRAY_BUFFER, m_mesh->m_vbo);
    m_mesh->setVertexAttrib();
    glDrawArrays(GL_TRIANGLES, 0, m_mesh->m_vertexcount);
  }

  for (int i = 0; i < m_mesh->m_attribs.size(); i++) {
    glDisableVertexAttribArray(i);
  }
}
}  // namespace Engine
}  // namespace WhiteDeer