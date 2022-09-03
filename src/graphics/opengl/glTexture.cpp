
#include "graphics/opengl/glTexture.h"

#include <log/log.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "filesystem/localfilesystem.h"

using WhiteDeer::Engine::GetLocalFileSystem;

namespace WhiteDeer {

namespace Utils {

using Graphics::Texture;
map<string, shared_ptr<Texture>> Texture::s_entries;

}  // namespace Utils

namespace Graphics {

Texture::~Texture() { glDeleteTextures(1, &m_handle); }

void Texture::Use(int index) const {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture::load(const string& path) {
  auto abspath = GetLocalFileSystem()->ToAbsolute(path).string();

  int nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data =
      stbi_load(abspath.c_str(), &m_width, &m_height, &nrChannels, 0);
  if (!data) {
    LOGE << "Faild to load texture " << path;
    return;
  }

  glGenTextures(1, &m_handle);
  glBindTexture(GL_TEXTURE_2D, m_handle);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, m_width, m_height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

}  // namespace Graphics
}  // namespace WhiteDeer