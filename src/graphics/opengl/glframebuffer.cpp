#include "graphics/opengl/glframebuffer.h"

#include "editor/gui/gamewindow.h"
#include "graphics/opengl/glrendertexture.h"
#include "log/log.h"

namespace WhiteDeer {

namespace Utils {
  using Graphics::FrameBuffer;
  map<string, shared_ptr<FrameBuffer>> FrameBuffer::s_entries;
}

namespace Graphics {

using Editor::GameWindow;

FrameBuffer::FrameBuffer(const string& name) {
  // create frame buffer
  m_name = name;
  glGenFramebuffers(1, &m_fbo);

  // create default texture
  m_defaultcolor = RenderTexture::Create(1920, 1080, RT_COLOR);
  m_defaultdepth = RenderTexture::Create(1920, 1080, RT_DEPTH_STENCIL);
}

FrameBuffer::~FrameBuffer() {
  // delete frame buffer
  glDeleteFramebuffers(1, &m_fbo);
}

void FrameBuffer::Bind() const {
  // bind default texture
  // resize to window size
  auto width = GameWindow::GetInstance()->GetWidth();
  auto height = GameWindow::GetInstance()->GetHeight();
  m_defaultcolor->Resize((int)width, (int)height);
  m_defaultdepth->Resize((int)width, (int)height);
  Bind(*m_defaultcolor, *m_defaultdepth);
}

void FrameBuffer::Bind(const RenderTexture& color,
                       const RenderTexture& depth) const {
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         color.GetHandle(), 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                         GL_TEXTURE_2D, depth.GetHandle(), 0);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void FrameBuffer::BindDepth(const RenderTexture& depth) const {
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         depth.GetHandle(), 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
}

void FrameBuffer::UnBind() const {
  // unbind frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}  // namespace Graphics
}  // namespace WhiteDeer