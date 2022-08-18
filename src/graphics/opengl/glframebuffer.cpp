#include "graphics/opengl/glframebuffer.h"

#include "log/log.h"

namespace WhiteDeer {
namespace Graphics {

shared_ptr<FrameBuffer> FrameBuffer::CreateFrameBuffer(int w, int h) {
  shared_ptr<FrameBuffer> ret(new FrameBuffer(w, h));

  return ret;
}

FrameBuffer::FrameBuffer(int w, int h) {
  m_width = w;
  m_height = h;

  // create frame buffer
  glGenFramebuffers(1, &m_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    LOGE << "create framebuffer failed";
  }

  // create texture
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         m_texture, 0);

  // create depth buffer
  glGenTextures(1, &m_depth);
  glBindTexture(GL_TEXTURE_2D, m_depth);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, w, h, 0, GL_DEPTH_STENCIL,
               GL_UNSIGNED_INT_24_8, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                         GL_TEXTURE_2D, m_depth, 0);
}

FrameBuffer::~FrameBuffer() {
  glDeleteFramebuffers(1, &m_fbo);
  glDeleteRenderbuffers(1, &m_rbo);
}

void FrameBuffer::Resize(int w, int h) {
  m_width = w;
  m_height = h;

  // change texture size
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
               NULL);

  // change depth texture
  glBindTexture(GL_TEXTURE_2D, m_depth);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, w, h, 0, GL_DEPTH_STENCIL,
               GL_UNSIGNED_INT_24_8, NULL);
}

void FrameBuffer::Bind() const {glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); }

void FrameBuffer::UnBind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

}  // namespace Graphics
}  // namespace WhiteDeer