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
  virtual ~Texture();
  virtual void Use(int index = 0) const;
  GLuint GetHandle() const { return m_handle; }
  int GetWidth() const { return m_width; }
  int GetHeight() const { return m_height; }

 protected:
  Texture() {}
  Texture(const string& path) { load(path); }
  void load(const string& path);

  GLuint m_handle;
  string m_path;
  int m_width;
  int m_height;
};

}  // namespace Graphics
}  // namespace WhiteDeer