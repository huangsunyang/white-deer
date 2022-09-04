#pragma once

#include "graphics/opengl/glmesh.h"
#include "graphics/opengl/glrendertexture.h"
#include "utils/common/enum.h"
#include "utils/common/registry.h"

DECLARE_ENUM(PostprocessType, PostprocessType_None, PostprocessType_Inversion,
             PostprocessType_GrayScale, PostprocessType_Blur,
             PostprocessType_Gamma, PostprocessType_HDR,
             PostprocessType_ExtractLight, PostprocessType_BlurH, PostprocessType_BlurV,
             PostprocessType_Bloom)

namespace WhiteDeer {
namespace Graphics {

using namespace WhiteDeer::Utils;

class Postprocess : public StaticNamedPool<PostprocessType, Postprocess> {
 public:
  Postprocess(PostprocessType);

  template <typename... T>
  void Render(const RenderTexture& dest, const T&... from) {
    Render(dest, {&from...});
  }
  void Render(const RenderTexture& dest, std::initializer_list<const Texture *> froms);

 protected:
  PostprocessType m_type;
  shared_ptr<Mesh> m_quad;
  shared_ptr<Program> m_program;
};

}  // namespace Graphics
}  // namespace WhiteDeer