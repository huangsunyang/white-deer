#include "graphics/opengl/glcubemaptexture.h"

#include <stb_image.h>

#include "filesystem/localfilesystem.h"
#include "log/log.h"

using WhiteDeer::Engine::GetLocalFileSystem;

namespace WhiteDeer {
namespace Graphics {

shared_ptr<CubeMapTexture> CubeMapTexture::Create(
    std::initializer_list<string> paths) {
  shared_ptr<CubeMapTexture> ptr(new CubeMapTexture(paths));
  Set(ptr);
  return ptr;
}

CubeMapTexture::CubeMapTexture(std::initializer_list<string> _paths) {
  vector<string> paths(_paths);
  glGenTextures(1, &m_handle);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);
  for (int i = 0; i < paths.size(); i++) {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    auto abspath = GetLocalFileSystem()->ToAbsolute(paths[i]);
    auto data =
        stbi_load(abspath.string().c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
      LOGE << "load cubemap failed: " << paths[i];
      continue;
    }
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void CubeMapTexture::Use(int index) const {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);
}

}  // namespace Graphics
}  // namespace WhiteDeer