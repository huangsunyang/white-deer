#include <GL/gl3w.h>

#include "log/log.h"

namespace WhiteDeer {
namespace Graphics {

std::string CheckGLError_() {
  GLenum errorCode;
  std::string error;
  while ((errorCode = glGetError()) != GL_NO_ERROR) {
    switch (errorCode) {
      case GL_INVALID_ENUM:
        error = "INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        error = "INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        error = "INVALID_OPERATION";
        break;
      case GL_STACK_OVERFLOW:
        error = "STACK_OVERFLOW";
        break;
      case GL_STACK_UNDERFLOW:
        error = "STACK_UNDERFLOW";
        break;
      case GL_OUT_OF_MEMORY:
        error = "OUT_OF_MEMORY";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        error = "INVALID_FRAMEBUFFER_OPERATION";
        break;
    }
  }
  return error;
}

#define CheckGLError()                 \
  do {                                 \
    auto error = CheckGLError_();      \
    if (!error.empty()) LOGE << error; \
  } while (0)

}  // namespace Graphics
}  // namespace WhiteDeer
