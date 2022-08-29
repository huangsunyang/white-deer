#pragma once

#include <GL/gl3w.h>

#include <memory>

#include "utils/common/registry.h"

namespace WhiteDeer {
namespace Graphics {

class RenderTexture;
using std::shared_ptr;
using Utils::StaticNamedPool;

class FrameBuffer : public StaticNamedPool<string, FrameBuffer> {
  friend class StaticNamedPool<string, FrameBuffer>;

 public:
  virtual ~FrameBuffer();
  void Bind() const;
  void Bind(const RenderTexture&, const RenderTexture&) const;
  void BindDepth(const RenderTexture&) const;
  void UnBind() const;
  shared_ptr<RenderTexture> GetDefaultColorRT() { return m_defaultcolor; }
  shared_ptr<RenderTexture> GetDefaultDepthRT() { return m_defaultdepth; }

  static shared_ptr<FrameBuffer> GetDefault() {return FrameBuffer::GetOrLoad("default");}


 protected:
  FrameBuffer(const string& name);

  shared_ptr<RenderTexture> m_defaultcolor;
  shared_ptr<RenderTexture> m_defaultdepth;
  GLuint m_fbo;
  string m_name;
};

}  // namespace Graphics
}  // namespace WhiteDeer
