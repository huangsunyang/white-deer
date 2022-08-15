#pragma once

#include <GL/gl3w.h>

#include <string>
#include <vector>

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
  void prepareData(const vector<GLfloat>& vbo, const vector<GLuint>& ebo);

  GLuint m_vbo;
  GLuint m_ebo;
  GLuint m_facecount;
};

}  // namespace Graphics
}  // namespace WhiteDeer
