#pragma once

#include "graphics/opengl/glTexture.h"
#include "graphics/opengl/glframebuffer.h"

namespace WhiteDeer {
namespace Graphics {

enum RenderTextureType {
  RT_COLOR,
  RT_DEPTH,
};

class RenderTexture : public Texture {
 public:
  static shared_ptr<RenderTexture> Create(int w, int h, RenderTextureType);
  void Resize(int w, int h);

 protected:
  RenderTexture(int, int, RenderTextureType);

  RenderTextureType m_type;
};

}  // namespace Graphics
}  // namespace WhiteDeer
