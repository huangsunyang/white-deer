#include "graphics/opengl/glTexture.h"

namespace WhiteDeer {
namespace Graphics {

class CubeMapTexture : public Texture {
public:
  static shared_ptr<CubeMapTexture> Create(std::initializer_list<string>);

  // right left top bottom front back
  CubeMapTexture(std::initializer_list<string>);
  virtual void Use(int index = 0) const;

protected:

};
}  // namespace Graphics
}  // namespace WhiteDeer