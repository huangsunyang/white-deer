#include "graphics/opengl/glrendertexture.h"

#include "log/log.h"

namespace WhiteDeer {
namespace Graphics {

shared_ptr<RenderTexture> RenderTexture::Create(int w, int h,
                                                RenderTextureType type) {
  // create texture
  shared_ptr<RenderTexture> rt(new RenderTexture(w, h, type));
  Set(rt);
  return rt;
}

RenderTexture::RenderTexture(int w, int h, RenderTextureType type) {
  m_type = type;

  glGenTextures(1, &m_handle);
  Resize(w, h);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void RenderTexture::Resize(int w, int h) {
  if (w <= 0 || h <= 0) {
    LOGW << "resize render texture " << w << ", " << h;
    return;
  }

  if (m_width == w && m_height == h) {
    return;
  }

  m_width = w;
  m_height = h;

  glBindTexture(GL_TEXTURE_2D, m_handle);
  if (m_type == RT_COLOR) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 NULL);
  } else if (m_type == RT_DEPTH_STENCIL) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, w, h, 0,
                 GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
  } else if (m_type == RT_DEPTH) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  } else {
    LOGE << "unknow RT type";
  }
}

}  // namespace Graphics
}  // namespace WhiteDeer