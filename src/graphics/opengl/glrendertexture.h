#pragma once

#include "graphics/opengl/glTexture.h"
#include "graphics/opengl/glframebuffer.h"
#include "utils/common/enum.h"

DECLARE_ENUM(RenderTextureType, RT_COLOR, RT_DEPTH_STENCIL, RT_DEPTH)
DECLARE_ENUM(RenderTextureFormat, RTF_AUTO, RTF_FLOAT)

namespace WhiteDeer {
namespace Graphics {

class RenderTexture : public Texture {
 public:
  static shared_ptr<RenderTexture> Create(int w, int h, RenderTextureType, RenderTextureFormat=RTF_AUTO);
  void Resize(int w, int h);
  void SetFormat(RenderTextureFormat);
  void CopyFrom(const Texture&);

 protected:
  RenderTexture(int, int, RenderTextureType, RenderTextureFormat);
  void RefreshData();

  RenderTextureType m_type;
  RenderTextureFormat m_format;
};

}  // namespace Graphics
}  // namespace WhiteDeer
