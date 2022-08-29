#pragma once

#include "graphics/opengl/glTexture.h"
#include "graphics/opengl/glmesh.h"
#include "utils/common/enum.h"
#include "utils/common/registry.h"

DECLARE_ENUM(PostprocessType, PostprocessType_None, PostprocessType_Inversion,
             PostprocessType_GrayScale, PostprocessType_Blur)

namespace WhiteDeer {
namespace Graphics {

using namespace WhiteDeer::Utils;

class Postprocess : public StaticNamedPool<PostprocessType, Postprocess> {
 public:
  Postprocess(PostprocessType);
  void Render(const Texture&);

 protected:
  PostprocessType m_type;
  shared_ptr<Mesh> m_quad;
  shared_ptr<Program> m_program;
};

}  // namespace Graphics
}  // namespace WhiteDeer