#pragma once
#include <GL/gl3w.h>

#include <vector>

using std::vector;

namespace WhiteDeer {
namespace Graphics {

struct VertexAttrib {
  VertexAttrib(int c, GLenum e) : count(c), type(e) {}

  int count;
  GLenum type;

  int GetTypeSize() const {
    return 4;  // todo
  };

  int GetSize() const { return count * GetTypeSize(); }
};

using VertexAttribList = vector<VertexAttrib>;

}  // namespace Graphics
}  // namespace WhiteDeer