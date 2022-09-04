#include "graphics/opengl/glrendertexture.h"

#include "graphics/opengl/glmesh.h"
#include "graphics/opengl/glshader.h"
#include "graphics/postprocess.h"
#include "log/log.h"

namespace WhiteDeer {
namespace Graphics {

shared_ptr<RenderTexture> RenderTexture::Create(int w, int h,
                                                RenderTextureType type,
                                                RenderTextureFormat format) {
  // create texture
  shared_ptr<RenderTexture> rt(new RenderTexture(w, h, type, format));
  Set(rt);
  return rt;
}

RenderTexture::RenderTexture(int w, int h, RenderTextureType type,
                             RenderTextureFormat format) {
  m_type = type;
  m_format = format;

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
  RefreshData();
}

void RenderTexture::SetFormat(RenderTextureFormat format) {
  if (m_format == format) {
    return;
  }

  m_format = format;
  RefreshData();
}

void RenderTexture::RefreshData() {
  glBindTexture(GL_TEXTURE_2D, m_handle);
  if (m_type == RT_COLOR) {
    glTexImage2D(GL_TEXTURE_2D, 0, m_format == RTF_AUTO ? GL_RGB : GL_RGB16F, m_width, m_height, 0, GL_RGB,
                 m_format == RTF_AUTO ? GL_UNSIGNED_BYTE : GL_FLOAT, NULL);
  } else if (m_type == RT_DEPTH_STENCIL) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0,
                 GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
  } else if (m_type == RT_DEPTH) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  } else {
    LOGE << "unknow RT type";
  }
}

void RenderTexture::CopyFrom(const Texture& from) {
  Resize(from.GetWidth(), from.GetHeight());
  auto postprocess = Postprocess::GetOrLoad(PostprocessType_None);
  postprocess->Render(*this, from);
}

}  // namespace Graphics
}  // namespace WhiteDeer