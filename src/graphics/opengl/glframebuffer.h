#pragma once

#include "graphics/opengl/glTexture.h"

namespace WhiteDeer {
namespace Graphics {
class FrameBuffer {
 public:
  virtual ~FrameBuffer();
  void Bind() const;
  void UnBind() const;
  void Resize(int w, int h);

  int GetWidth() { return m_width; }
  int GetHeight() { return m_height; }
  GLuint GetTextureHandle() { return m_texture; }
  GLuint GetDepthTextureHandle() { return m_depth; }
  static shared_ptr<FrameBuffer> CreateFrameBuffer(int, int);

 protected:
  FrameBuffer(int, int);

  GLuint m_fbo;
  GLuint m_rbo;
  GLuint m_texture;
  GLuint m_depth;

  int m_width;
  int m_height;
};
}  // namespace Graphics
}  // namespace WhiteDeer
