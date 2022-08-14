#pragma once
#include <GL/gl3w.h>

#include <map>
#include <memory>
#include <string>

using std::map;
using std::shared_ptr;
using std::string;

namespace WhiteDeer {
namespace Graphics {

class Texture {
 public:
  static shared_ptr<Texture> Load(const string& path) {
    shared_ptr<Texture> p_texture(new Texture(path));
    s_textures[path] = p_texture;
    return p_texture;
  }

  void Use(int index = 0) const {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, m_handle);
  }
  GLuint GetHandle() { return m_handle; }

 protected:
  Texture() { glGenTextures(1, &m_handle); }
  Texture(const string& path) : Texture() { _Load(path); }
  void _Load(const string& path);

  GLuint m_handle;
  string m_path;

  static map<string, shared_ptr<Texture>> s_textures;
};

}  // namespace Graphics
}  // namespace WhiteDeer