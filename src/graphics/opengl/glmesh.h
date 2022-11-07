#pragma once

#include <GL/gl3w.h>

#include <string>
#include <vector>

#include "graphics/opengl/gltexture.h"
#include "graphics/opengl/glvertexattrib.h"
#include "utils/common/registry.h"
#include "utils/common/physics.h"
#include "components/renderer.h"

using std::string;
using std::vector;
using WhiteDeer::Utils::StaticNamedPool;

namespace WhiteDeer {
namespace Graphics {

class Mesh : public StaticNamedPool<string, Mesh> {
  friend class StaticNamedPool<string, Mesh>;

 public:
  void Draw();
 protected:
  Mesh(const string& path) { load(path); }
  Mesh(const string& noname, const vector<GLfloat>& points,
       const VertexAttribList& attribs) {
    prepareData(points, {}, attribs);
  }
  void load(const string& path);
  void loadPly(const string& path);
  void loadObj(const string& path);
  void loadCube();
  void loadQuad();
  void prepareData(const vector<GLfloat>& vbo, const vector<GLuint>& ebo,
                   const VertexAttribList& attribs);
  void setVertexAttrib();
  int GetVertexStride();
  int GetVertexSize();

  void CalculateAABB(const vector<GLfloat>&);

  GLuint m_vbo;
  GLuint m_ebo;
  GLuint m_vertexcount;
  bool m_useebo;

  VertexAttribList m_attribs;
  vector<shared_ptr<Texture>> m_textures;

  AABB m_aabb;
};

}  // namespace Graphics
}  // namespace WhiteDeer
