#pragma once

#include <cassert>

#ifdef SARIEL_GL_DEBUG
#include "glad.h"

inline void checkGLError(const char *label) {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR)
    std::cerr << "GL error [" << label << "]: 0x" << std::hex << err
              << std::endl;
}
#define GL_CHECK(label) checkGLError(label)
#else
#define GL_CHECK(label)
#endif

#define STR(x) #x

using f32 = float;
using f64 = double;
using uint = unsigned int;
