#pragma once
#include <GL/gl3w.h>

#include <map>
#include <memory>
#include <string>

#include "utils/common/registry.h"

using std::map;
using std::shared_ptr;
using std::string;
using WhiteDeer::Utils::StaticNamedPool;

namespace WhiteDeer {
namespace Graphics {

class Texture : public StaticNamedPool<string, Texture> {
  friend class StaticNamedPool<string, Texture>;

 public:
  void Use(int index = 0) const {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, m_handle);
  }
  GLuint GetHandle() { return m_handle; }

 protected:
  Texture() { glGenTextures(1, &m_handle); }
  void load(const string& path);

  GLuint m_handle;
  string m_path;
};

}  // namespace Graphics
}  // namespace WhiteDeer