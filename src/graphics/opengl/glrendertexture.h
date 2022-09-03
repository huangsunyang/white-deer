#pragma once

#include "graphics/opengl/glTexture.h"
#include "graphics/opengl/glframebuffer.h"
#include "utils/common/enum.h"

DECLARE_ENUM(RenderTextureType, RT_COLOR, RT_DEPTH_STENCIL, RT_DEPTH)

namespace WhiteDeer {
namespace Graphics {

class RenderTexture : public Texture {
 public:
  static shared_ptr<RenderTexture> Create(int w, int h, RenderTextureType);
  void Resize(int w, int h);
  void CopyFrom(const Texture&);

 protected:
  RenderTexture(int, int, RenderTextureType);

  RenderTextureType m_type;
};

}  // namespace Graphics
}  // namespace WhiteDeer
