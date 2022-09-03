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
  void RawBind();
  void Bind(const RenderTexture&, const RenderTexture&);
  void BindScreen();
  void BindColor(const RenderTexture&);
  void BindDepth(const RenderTexture&);
  void UnBind();
  shared_ptr<RenderTexture> GetDefaultColorRT() const { return m_defaultcolor; }
  shared_ptr<RenderTexture> GetDefaultDepthRT() const { return m_defaultdepth; }
  shared_ptr<RenderTexture> GetCurrentColorRT() const { return m_currentcolor; }
  shared_ptr<RenderTexture> GetCurrentDepthRT() const { return m_currentdepth; }

  static shared_ptr<FrameBuffer> GetDefault() {
    return FrameBuffer::GetOrLoad("default");
  }
  static FrameBuffer * GetCurrent() { return s_current; }

 protected:
  FrameBuffer(const string& name);

  shared_ptr<RenderTexture> m_defaultcolor;
  shared_ptr<RenderTexture> m_defaultdepth;
  shared_ptr<RenderTexture> m_currentcolor;
  shared_ptr<RenderTexture> m_currentdepth;
  GLuint m_fbo;
  string m_name;

  static FrameBuffer * s_current;
};

}  // namespace Graphics
}  // namespace WhiteDeer
