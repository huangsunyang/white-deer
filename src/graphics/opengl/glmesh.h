#pragma once

#include <GL/gl3w.h>

#include <string>
#include <vector>

#include "graphics/opengl/glvertexattrib.h"
#include "graphics/opengl/gltexture.h"
#include "utils/common/registry.h"

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
  Mesh() {}
  void load(const string& path);
  void loadPly(const string& path);
  void loadObj(const string& path);
  void prepareData(const vector<GLfloat>& vbo, const vector<GLuint>& ebo);
  void setVertexAttrib();

  GLuint m_vbo;
  GLuint m_ebo;
  GLuint m_facecount;

  VertexAttribList m_attribs;
  vector<shared_ptr<Texture>> m_textures;
};

}  // namespace Graphics
}  // namespace WhiteDeer
