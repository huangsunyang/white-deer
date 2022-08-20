#pragma once

#include <GL/gl3w.h>

#include <memory>

#include "utils/common/singleton.h"

namespace WhiteDeer {
namespace Graphics {

class RenderTexture;
using std::shared_ptr;
using Utils::Singleton;

class FrameBuffer : public Singleton<FrameBuffer> {
  friend class Singleton<FrameBuffer>;

 public:
  virtual ~FrameBuffer();
  void Bind() const;
  void Bind(const RenderTexture&, const RenderTexture&) const;
  void UnBind() const;
  shared_ptr<RenderTexture> GetDefaultColorRT() { return m_defaultcolor; }
  shared_ptr<RenderTexture> GetDefaultDepthRT() { return m_defaultdepth; }

 protected:
  FrameBuffer();

  shared_ptr<RenderTexture> m_defaultcolor;
  shared_ptr<RenderTexture> m_defaultdepth;
  GLuint m_fbo;
};

}  // namespace Graphics
}  // namespace WhiteDeer
